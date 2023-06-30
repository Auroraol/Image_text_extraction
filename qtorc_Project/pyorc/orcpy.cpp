#include "orcpy.h"

Orcpy::Orcpy()
{
    initOrc();
}

Orcpy::~Orcpy()
{
    Py_DECREF(pModule);
    Py_Finalize();
}


int Orcpy::initOrc()
{
    // 初始化Python解释器
    Py_Initialize();
    if (!Py_IsInitialized())
    {
        printf("初始化失败！");
        return 0;
    }
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('C:/Qt/code/Image_text_extraction/pyorc_Project')");
    // 加载Python模块
    pModule = PyImport_ImportModule("main");
    if (pModule == NULL)
    {
        printf("调用的模块不存在");
        Py_Finalize();
        return 0;
    }
    return 1;
}

/*
void processResult(const std::string& result)
{
    lines.clear();
    // 将结果按换行符拆分成多行文本
    size_t startPos = 0;
    size_t endPos = result.find('\n');
    while (endPos != std::string::npos)
    {
        std::string line = result.substr(startPos, endPos - startPos);
        lines.push_back(line);
        startPos = endPos + 1;
        endPos = result.find('\n', startPos);
    }
    // 处理最后一行
    std::string line = result.substr(startPos);
    lines.push_back(line);
    // 输出处理后的结果
//    for (const std::string& line : lines)
//    {
//        qDebug() << QString::fromStdString(line);
//    }
}
/
*/

//将QPixmap对象转换为字节流
QByteArray Orcpy::imageToByteArray(const QPixmap& pixmap)
{
    QByteArray byteArray;
    QImage image = pixmap.toImage();
    if (image.isNull())
    {
        qDebug() << "转换失败";
        return QByteArray();
    }
    else
    {
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG"); // 保存为PNG格式，可根据需要修改为其他格式
        buffer.close();
    }
    return byteArray;
}

// 图片识别
std::vector<std::string> Orcpy::runOrc(const QPixmap& pixmap)
{
    std::vector<std::string> lines;
//    QString projectPath = QCoreApplication::applicationDirPath();
//    qDebug() << "Project Path:" << projectPath;
    // 获取Python函数对象
    PyObject* pFunc = PyObject_GetAttrString(pModule, "RecognizeImage");
    if (!pFunc || !PyCallable_Check(pFunc))
    {
        printf("获取函数失败");
        return {};
    }
    // 加载图像
    QByteArray imageBytes = imageToByteArray(pixmap);
    // 将字节流转换为Python对象
    PyObject* pArgs = PyTuple_New(1);
    PyObject* pImageData = PyBytes_FromStringAndSize(imageBytes.constData(), imageBytes.size());
    PyTuple_SetItem(pArgs, 0, pImageData);
    // 调用Python函数
    PyObject* pResult = PyObject_CallObject(pFunc, pArgs);
    if (!pResult)
    {
        printf("调用函数失败");
        PyErr_Print();
        Py_DECREF(pArgs);
        Py_DECREF(pFunc);
        return {};
    }
    // 处理Python函数的结果
    char* res = nullptr;
    if (PyArg_Parse(pResult, "s", &res))
    {
        std::string resultString(res);
        //processResult(resultString);
        lines.clear();
        // 将结果按换行符拆分成多行文本
        size_t startPos = 0;
        size_t endPos = resultString.find('\n');
        while (endPos != std::string::npos)
        {
            std::string line = resultString.substr(startPos, endPos - startPos);
            lines.push_back(line);
            startPos = endPos + 1;
            endPos = resultString.find('\n', startPos);
        }
        // 处理最后一行
        std::string line = resultString.substr(startPos);
        lines.push_back(line);
    }
    // 释放Python对象和资源
    Py_DECREF(pArgs);
    Py_DECREF(pResult);
    Py_DECREF(pFunc);
    return lines;
}

// 剪切板识别
std::vector<std::string> Orcpy::runOrc()
{
    std::vector<std::string> lines;
    // 获取函数对象
    PyObject* startFunc = PyObject_GetAttrString(pModule, "RecognizeClipboard");
    if (!startFunc || !PyCallable_Check(startFunc))
    {
        qDebug() << "获取函数失败";
    }
    // 调用函数，并获取返回值
    PyObject* result = PyObject_CallObject(startFunc, NULL);
    if (!result)
    {
        qDebug() << "调用函数失败";
        PyErr_Print(); // 打印 Python 错误信息
        return {};
    }
    // 将返回值转换为 C 字符串
    char* res = nullptr;
    if (PyArg_Parse(result, "s", &res))
    {
        std::string resultString(res);
        //processResult(resultString);
        //lines.clear();
        // 将结果按换行符拆分成多行文本
        size_t startPos = 0;
        size_t endPos = resultString.find('\n');
        while (endPos != std::string::npos)
        {
            std::string line = resultString.substr(startPos, endPos - startPos);
            lines.push_back(line);
            startPos = endPos + 1;
            endPos = resultString.find('\n', startPos);
        }
        // 处理最后一行
        std::string line = resultString.substr(startPos);
        lines.push_back(line);
    }
    // 清理 Python 对象和资源
    Py_XDECREF(result);
    Py_XDECREF(startFunc);
    return lines;
}


