#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "screen/screen.h"
#include <QFileDialog>
#include <QDebug>
#include <QPainter>
#include <QBuffer>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //
    this->setFixedSize(841, 479);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);    //禁用水平滑动条
    widget = new QWidget(ui->scrollArea);
    ui->scrollArea->setWidget(widget);
    layout = new QVBoxLayout(widget);
    widget_text = new QWidget(ui->scrollArea_text);
    ui->scrollArea_text->setWidget(widget_text);
    layout_text = new QVBoxLayout(widget_text);
    // 多线程
    ocrWorker_ = new OcrWorker();
    ocrThread_ = new QThread(this);
    ocrWorker_->moveToThread(ocrThread_);
    connect(ocrThread_, &QThread::finished, ocrThread_, &QThread::deleteLater);
    connect(ocrWorker_, &OcrWorker::ocrRCFinished, this, &MainWindow::handleRCFinished);
    connect(ocrWorker_, &OcrWorker::ocrImgFinished, this, &MainWindow::handleImgFinished);
    //connect(this, &MainWindow::rcRun, ocrWorker_, &OcrWorker::processRc);
    //connect(this, &MainWindow::pixmapRun, ocrWorker_, &OcrWorker::processPixmap);
    ocrThread_->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    widget->deleteLater();
    layout->deleteLater();
}

//设置背景
void MainWindow::paintEvent(QPaintEvent* event)
{
    QPainter myPainter(this);
    myPainter.setOpacity(0.9);          //背景图片透明度
    myPainter.drawPixmap(0, 0, this->width(), this->height(),
                         QPixmap(":/resource/image/lb_background.png"));
}

// 打开文件
void MainWindow::on_pushButton_clicked()
{
    // 打开多个文件
    const QStringList list(QFileDialog::getOpenFileNames(this, "打开文件", "C:\\", "Image Files (*.png *.bmp *.jpg);;PDF Files (*.pdf)"));
    if(list.isEmpty())
    {
        return;
    }
    // 清空之前的控件
    QLayoutItem* child;
    while ((child = layout->takeAt(0)) != nullptr)
    {
        delete child->widget(); //删除该子控件
        delete child;           //删除 child 对象本身
    }
    imageList.clear();
    // 显示
    //const QString& path = list.front();
    for(int i = 0; i < list.count(); ++i)
    {
        QLabel* label = new QLabel();
        QPixmap pixmap;
        pixmap.load(list[i]);
        imageList << pixmap;
        label->setPixmap(pixmap.scaled(405, 405, Qt::KeepAspectRatio));
        layout->addWidget(label);
    }
}

// 截图
void MainWindow::showScreenshotPixmap(const QPixmap& pix)
{
    // 清空之前的控件
    QLayoutItem* child;
    while ((child = layout->takeAt(0)) != nullptr)
    {
        delete child->widget(); //删除该子控件
        delete child;           //删除 child 对象本身
    }
    imageList.clear();
    //
    QLabel* label = new QLabel();
    label->setPixmap(pix.scaled(405, 405, Qt::KeepAspectRatio));
    layout->addWidget(label);
    //
    imageList << pix;
}

void MainWindow::on_pushButton_2_clicked()
{
    ScreenWidget* screen = ScreenWidget::Instance();
    screen->showFullScreen();   //直接调用实例
    connect(screen, &ScreenWidget::screenshotPixmap, this, &MainWindow::showScreenshotPixmap);
}

// 识别
void MainWindow::on_pushButton_3_clicked()
{
    while ((child = layout_text->takeAt(0)) != nullptr)
    {
        delete child->widget(); //删除该子控件
        delete child;           //删除 child 对象本身
    }
    // Start the OCR thread with the selected images
    foreach (const QPixmap& pixmap, imageList)
    {
        //emit pixmapRun(pixmap);  // 方法1
        // 异步调用processPixmap槽
        QMetaObject::invokeMethod(ocrWorker_, "processPixmap", Qt::QueuedConnection, Q_ARG(QPixmap, pixmap));
    }
}

void MainWindow::handleImgFinished(const std::vector<std::string>& lines)
{
    QLabel* label_text = new QLabel();
    for (const std::string& line : lines)
    {
        QString text = QString::fromStdString(line);
        label_text->setText(label_text->text() + "<br>" + text);
    }
    label_text->setText(label_text->text() + "============================================================<br>");
    layout_text->addWidget(label_text);
}

// 清除
void MainWindow::on_pushButton_4_clicked()
{
    while ((child = layout->takeAt(0)) != nullptr)
    {
        delete child->widget(); //删除该子控件
        delete child;           //删除 child 对象本身
    }
    while ((child = layout_text->takeAt(0)) != nullptr)
    {
        delete child->widget(); //删除该子控件
        delete child;           //删除 child 对象本身
    }
    imageList.clear();
}

// 剪切板识别
void MainWindow::on_pushButton_5_clicked()
{
    while ((child = layout_text->takeAt(0)) != nullptr)
    {
        delete child->widget(); //删除该子控件
        delete child;           //删除 child 对象本身
    }
    //emit rcRun(); // 方法1
    // 异步调用processPixmap槽
    QMetaObject::invokeMethod(ocrWorker_, "processRc", Qt::QueuedConnection);
}

void MainWindow::handleRCFinished(const std::vector<std::string>& lines)
{
    QLabel* label = new QLabel();
    for (const std::string& line : lines)
    {
        QString text = QString::fromStdString(line);
        label->setText(label->text() + "<br>" + text);
    }
    layout_text->addWidget(label);
}

