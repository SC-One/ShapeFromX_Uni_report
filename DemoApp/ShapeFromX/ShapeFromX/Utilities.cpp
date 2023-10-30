#include "Utilities.h"
#include <QDebug>

cv::Mat Utility::loadFromQrc(QString qrc, int flag) {
  QFile file(qrc);
  cv::Mat m;
  if (file.open(QIODevice::ReadOnly)) {
    qDebug() << "Could not load shading image.";
    qint64 sz = file.size();
    std::vector<uchar> buf(sz);
    file.read((char *)buf.data(), sz);
    m = cv::imdecode(buf, flag);
  } else
    qDebug() << "Could not open: " << file.errorString() << "FileName: " << qrc;
  return m;
}
