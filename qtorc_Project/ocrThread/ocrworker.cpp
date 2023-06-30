#include "ocrworker.h"

OcrWorker::OcrWorker(QObject* parent) : QObject(parent)
{
    orc = new Orcpy();
}

void OcrWorker::processPixmap(const QPixmap& pixmap)
{
    std::vector<std::string> lines;
    lines = orc->runOrc(pixmap);
    emit ocrImgFinished(lines);
}

void OcrWorker::processRc()
{
    std::vector<std::string> lines;
    lines = orc->runOrc();
    emit ocrRCFinished(lines);
}
