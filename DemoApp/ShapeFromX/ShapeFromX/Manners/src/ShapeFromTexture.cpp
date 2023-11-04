#include <Manners/include/ShapeFromTexture.h>
#include <QDebug>
#include <Utilities.h>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;

ShapeFromTexture::ShapeFromTexture(QObject *parent) : ShapeFromXBase{parent} {}

void ShapeFromTexture::calculateOutput() {

  qDebug() << "ShapeFromShading , new Input: " << currentImageURL();
  if (!currentImageURL().isValid() || currentImageURL().isEmpty())
    return;

  cv::Mat image = Utility::loadFromQrc(
      currentImageURL(), // test:
                         // https://www.pngall.com/face-png/download/99591
      cv::IMREAD_GRAYSCALE);
  if (image.empty()) {
    qDebug() << "Error: Could not load shading image.";
    return;
  }
  if (image.empty()) {
    std::cerr << "Failed to load image." << std::endl;
    return;
  }

  // Compute texture gradients (Sobel operator is used here)
  cv::Mat gradX, gradY;
  cv::Sobel(image, gradX, CV_32F, 1, 0);
  cv::Sobel(image, gradY, CV_32F, 0, 1);

  // Estimate surface normals
  cv::Mat normalX = -gradX; // Negate for consistency with depth sign
  cv::Mat normalY = -gradY; // Negate for consistency with depth sign
  cv::Mat normalZ = cv::Mat::ones(image.size(), CV_32F);

  // Integrate the normals to estimate depth
  cv::Mat depthMap;
  //  cv::integrateNormals(normalX, normalY, normalZ, depthMap, 0.1f, 0.1f);

  // Visualize the depth map or save it
  cv::imshow("Depth Map", depthMap);
  cv::waitKey(0);

  return;
}
