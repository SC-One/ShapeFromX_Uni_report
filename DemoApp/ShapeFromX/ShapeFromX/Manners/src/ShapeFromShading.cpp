#include <QFile>
#include <QUuid>
#include <ShapeFromShading.h>
#include <Utilities.h>
#include <cmath>
#include <opencv2/opencv.hpp>

#include <QDebug>
#include <QImage>
#include <Utilities.h>

using namespace cv;
using namespace std;
static const char *NORMAL_POSTFIX = "_normal";
static const char *REVERSE_POSTFIX = "_reverse";
static const char *DEFAULT_IMAGE_EXTENSION = ".png";

static void saveAsRGB32GrayScaleInverse(cv::Mat const &normalZ,
                                        QString const &fileAddress) {
  QImage normalZImage(normalZ.cols, normalZ.rows, QImage::Format_RGB32);

  for (int y = 0; y < normalZ.rows; y++) {
    for (int x = 0; x < normalZ.cols; x++) {
      float value = normalZ.at<float>(y, x);
      int grayValue = static_cast<int>(value * 255);
      if (2 > grayValue) {
        normalZImage.setPixel(x, y, 255);
        continue;
      }
      // Invert the grayscale value
      int invertedGrayValue = 255 - grayValue;

      QRgb pixel =
          qRgb(invertedGrayValue, invertedGrayValue, invertedGrayValue);
      normalZImage.setPixel(x, y, pixel);
    }
  }

  if (!normalZImage.save(fileAddress)) {
    qDebug() << "isGrayscale isDetached isNull" << normalZImage.isGrayscale()
             << normalZImage.isDetached() << normalZImage.isNull();
  }
}

static void saveAsRGB32GrayScale(cv::Mat const &normalZ,
                                 QString const &fileAddress) {
  QImage normalZImage(normalZ.cols, normalZ.rows, QImage::Format_RGB32);

  for (int y = 0; y < normalZ.rows; y++) {
    for (int x = 0; x < normalZ.cols; x++) {
      float value = normalZ.at<float>(y, x);
      int grayValue = static_cast<int>(value * 255);
      if (254 < grayValue) {
        normalZImage.setPixel(x, y, 0);
        continue;
      }
      QRgb pixel = qRgb(grayValue, grayValue, grayValue);
      normalZImage.setPixel(x, y, pixel);
    }
  }
  if (!normalZImage.save(fileAddress)) {
    qDebug() << "isGrayscale isDetached isNull" << normalZImage.isGrayscale()
             << normalZImage.isDetached() << normalZImage.isNull();
  }
}

ShapeFromShading::ShapeFromShading(QObject *parent) : ShapeFromXBase(parent) {
  _outputFileName = QUuid::createUuid().toString();
}

void ShapeFromShading::calculateOutput() {
  qDebug() << "ShapeFromShading , new Input: " << currentImageURL();
  if (!currentImageURL().isValid() || currentImageURL().isEmpty())
    return;

  Mat shading = Utility::loadFromQrc(
      currentImageURL(), // test:
                         // https://www.pngall.com/face-png/download/99591
      IMREAD_GRAYSCALE);
  if (shading.empty()) {
    qDebug() << "Error: Could not load shading image.";
    return;
  }
  shading.convertTo(shading, CV_32F);
  normalize(shading, shading, 0, 1, NORM_MINMAX);
  Mat gradX, gradY;
  Sobel(shading, gradX, CV_32F, 1, 0, 3);
  Sobel(shading, gradY, CV_32F, 0, 1, 3);
  float *gradXArray = gradX.ptr<float>();
  float *gradYArray = gradY.ptr<float>();
  Mat normalZ = Mat::zeros(gradX.size(), CV_32F);
  float *normalZArray = normalZ.ptr<float>();
  for (int i = 0; i < gradX.total(); i++) {
    float norm =
        1.0f + gradXArray[i] * gradXArray[i] + gradYArray[i] * gradYArray[i];
    normalZArray[i] = 1.0f / std::sqrt(norm);
  }
  saveAsRGB32GrayScale(normalZ, fullOutNormalFileName());
  saveAsRGB32GrayScaleInverse(normalZ, fullOutInverseFileName());
  //  imshow("Surface Normals", normalZ); // to debug
  emit outputCalculated();
}

QString ShapeFromShading::outputFileName() const { return _outputFileName; }

QString ShapeFromShading::fullOutNormalFileName() const {
  return outputFileName() + normalPostFix() + defaultImageExtension();
}

QString ShapeFromShading::fullOutInverseFileName() const {
  return outputFileName() + reversePostFix() + defaultImageExtension();
}

QString ShapeFromShading::normalPostFix() { return NORMAL_POSTFIX; }

QString ShapeFromShading::reversePostFix() { return REVERSE_POSTFIX; }

QString ShapeFromShading::defaultImageExtension() {
  return DEFAULT_IMAGE_EXTENSION;
}
