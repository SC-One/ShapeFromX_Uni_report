#include "Utilities.h"
#include <QDebug>
cv::Mat Utility::loadFromQrc(QUrl const &qurl, int flag) {
  QString path;
  auto schema = qurl.scheme();
  if (schema == "qrc") {
    path = qurl.path().mid(1);
  } else if (schema.isEmpty()) {
    path = qurl.path().mid(2);
  } else {
    path = qurl.toLocalFile();
  }
  QFile file(path);
  cv::Mat m;
  if (file.open(QIODevice::ReadOnly)) {
    qDebug() << "reading: " << path << qurl;
    qint64 sz = file.size();
    std::vector<uchar> buf(sz);
    file.read((char *)buf.data(), sz);
    m = cv::imdecode(buf, flag);
  } else {
    qDebug() << "Could not open: " << file.errorString() << "FileName: " << qurl
             << path << qurl.path() << m.size.dims();
  }
  return m;
}
