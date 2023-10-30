#ifndef UTILITIES_H
#define UTILITIES_H
#include <QFile>
#include <QString>
#include <vector>

#include <opencv2/opencv.hpp>
namespace Utility {
cv::Mat loadFromQrc(QString qrc, int flag = cv::ImreadModes::IMREAD_GRAYSCALE);
} // namespace Utility
#endif // UTILITIES_H
