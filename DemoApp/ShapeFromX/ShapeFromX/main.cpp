#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QLocale>
#include <QTranslator>

#include "Utilities.h"
#include <Manners/ShapingManners.h>
#include <cmath>
#include <opencv2/opencv.hpp>

#ifdef TEST_OPENCV_BY_US
int test() {

  using namespace cv;
  using namespace std;
  // Load the shading image (grayscale)
  Mat shading = Utility::loadFromQrc(
      QUrl(":/test.png"), // test:
                          // https://www.pngall.com/face-png/download/99591
      IMREAD_GRAYSCALE);

  if (shading.empty()) {
    cerr << "Error: Could not load shading image." << endl;
    return -1;
  }

  // Convert shading image to floating-point
  shading.convertTo(shading, CV_32F);

  // Normalize the shading to the range [0, 1]
  normalize(shading, shading, 0, 1, NORM_MINMAX);

  // Calculate the gradients of the shading image
  Mat gradX, gradY;
  Sobel(shading, gradX, CV_32F, 1, 0, 3);
  Sobel(shading, gradY, CV_32F, 0, 1, 3);

  // Convert gradients to plain arrays
  float *gradXArray = gradX.ptr<float>();
  float *gradYArray = gradY.ptr<float>();

  // Calculate the surface normals using plain arrays
  Mat normalZ = Mat::zeros(gradX.size(), CV_32F);
  float *normalZArray = normalZ.ptr<float>();

  for (int i = 0; i < gradX.total(); i++) {
    float norm =
        1.0f + gradXArray[i] * gradXArray[i] + gradYArray[i] * gradYArray[i];
    normalZArray[i] = 1.0f / std::sqrt(norm);
  }

  // Display or save the resulting normal map
  imshow("Surface Normals", normalZ);
  waitKey(0);

  return 0;
}
#endif

int main(int argc, char *argv[]) {

#ifdef TEST_OPENCV_BY_US
  return test();
#endif

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

  QCoreApplication::setOrganizationName("Header Coding");
  QCoreApplication::setOrganizationDomain("HCoding.ir");

  QGuiApplication app(argc, argv);

  Manners::registerMannersToQuickEngine();
  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString &locale : uiLanguages) {
    const QString baseName = "ShapeFromX_" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName)) {
      app.installTranslator(&translator);
      break;
    }
  }

  QQmlApplicationEngine engine;
  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}
