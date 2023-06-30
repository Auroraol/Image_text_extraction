#ifndef OCRWORKER_H
#define OCRWORKER_H

#include "pyorc/orcpy.h"
#include <QObject>
#include <QPixmap>
#include <vector>
#include <string>

class OcrWorker : public QObject
{
    Q_OBJECT

public:
    explicit OcrWorker(QObject* parent = nullptr);

public slots:
    void processPixmap(const QPixmap& pixmap);
    void processRc();
signals:
    void ocrRCFinished(const std::vector<std::string>& lines);
    void ocrImgFinished(const std::vector<std::string>& lines);

private:
    Orcpy* orc;
};

#endif // OCRWORKER_H
