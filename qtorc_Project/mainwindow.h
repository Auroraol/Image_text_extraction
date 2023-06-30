#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ocrThread/ocrworker.h"
//#include "pyorc/orcpy.h"
#include <QLabel>
#include <QMainWindow>
#include <QPaintEvent>
#include <QVBoxLayout>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void showScreenshotPixmap(const QPixmap& pix);
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();

private slots:
    void handleImgFinished(const std::vector<std::string>& lines);
    void handleRCFinished(const std::vector<std::string>& lines);

    /* 方法1
    signals:
        void rcRun();
        void pixmapRun(const QPixmap& pixmap);
    */

private:
    QThread* ocrThread_;
    OcrWorker* ocrWorker_;

private:
    Ui::MainWindow* ui;
    void paintEvent(QPaintEvent* event);
    // 打开文件
    QWidget* widget;
    QVBoxLayout* layout;

    QWidget* widget_text;
    QVBoxLayout* layout_text;

    // 图
    QList<QPixmap> imageList; //

    // Orcpy* orc; //

    QLayoutItem* child;//


};
#endif // MAINWINDOW_H
