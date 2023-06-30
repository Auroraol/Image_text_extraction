#ifndef ORCPY_H
#define ORCPY_H

#undef slots
#include <Python.h>
#define slots Q_SLOTS
#include <QBuffer>
#include <QCoreApplication>
#include <QDebug>
#include <QPixmap>
#include <iostream>

class Orcpy
{
public:
    Orcpy();
    ~Orcpy();
    std::vector<std::string> runOrc(const QPixmap& pixmap);
    std::vector<std::string> runOrc();
private:
    PyObject* pModule;
    int initOrc();
    QByteArray imageToByteArray(const QPixmap& pixmap);
};

#endif // ORCPY_H
