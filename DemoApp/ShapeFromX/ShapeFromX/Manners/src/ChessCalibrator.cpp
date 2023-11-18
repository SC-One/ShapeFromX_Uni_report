#include <Manners/include/ChessCalibrator.h>
#include <QDir>
#include <QImage>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlImageProviderBase>
#include <QQuickImageProvider>
#include <QUuid>

#include <QGuiApplication>
#include <QImage>
#include <QList>
#include <QVector>
#include <QtConcurrent>
#include <opencv2/opencv.hpp>

static auto const IMG_PROVIDER_NAME = "ChessImgProvider";
static QString const IMG_PROVIDER_FORMAT_ADDRESS = "image://%1/%2";

static QImage CvMatToQImage(const cv::Mat &mat) {
  QImage image;

  // Convert cv::Mat to QImage
  if (!mat.empty()) {
    cv::Mat temp; // Make a copy of the input mat
    switch (mat.type()) {
    case CV_8UC1:
      // Grayscale image
      image = QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step),
                     QImage::Format_Grayscale8)
                  .copy();
      break;
    case CV_8UC3:
      // Color image
      cv::cvtColor(mat, temp, cv::COLOR_BGR2RGB);
      image = QImage(temp.data, temp.cols, temp.rows,
                     static_cast<int>(temp.step), QImage::Format_RGB888)
                  .copy();
      break;
    case CV_8UC4:
      // Image with alpha channel
      cv::cvtColor(mat, temp, cv::COLOR_BGRA2RGBA);
      image = QImage(temp.data, temp.cols, temp.rows,
                     static_cast<int>(temp.step), QImage::Format_RGBA8888)
                  .copy();
      break;
    default:
      image = QImage();
      break;
    }
  }

  return image;
}

static cv::Mat QImageToCvMat(const QImage &image) {
  cv::Mat mat;

  // Convert QImage to cv::Mat
  switch (image.format()) {
  case QImage::Format_ARGB32:
  case QImage::Format_RGB32:
  case QImage::Format_ARGB32_Premultiplied:
    mat = cv::Mat(image.height(), image.width(), CV_8UC4,
                  (void *)image.constBits(), image.bytesPerLine());
    cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR);
    break;
  case QImage::Format_RGB888:
    mat = cv::Mat(image.height(), image.width(), CV_8UC3,
                  (void *)image.constBits(), image.bytesPerLine());
    cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
    break;
  default:
    mat = cv::Mat();
    break;
  }

  return mat.clone(); // Return a deep copy
}

static QList<QVector<double>> matToList(const cv::Mat &inputMat) {
  QList<QVector<double>> resultList;

  for (int i = 0; i < inputMat.rows; ++i) {
    QVector<double> rowData;
    const double *ptr = inputMat.ptr<double>(i);
    for (int j = 0; j < inputMat.cols; ++j) {
      rowData.append(ptr[j]);
    }
    resultList.append(rowData);
  }

  return resultList;
}

ChessCalibrator::ChessCalibrator(QObject *parent)
    : QObject{parent}, imgProvider(new ChessImageProvider) {
  QTimer::singleShot(5000, [this]() {
    QQmlEngine *engine = QQmlEngine::contextForObject(this)->engine();

    engine->addImageProvider(IMG_PROVIDER_NAME, imgProvider);
  });
  connect(
      this, &ChessCalibrator::newResultReceived, this,
      [this](CalibrationResult const result) {
        if (result.success) {
          resetNewImage(result.paintedImage);
        }
        setIsProcessing(false);
        result.print();
      },
      Qt::QueuedConnection);
}

void ChessCalibrator::processImage(const QString &path) {
  QUrl imageUrl(path);
  QQmlEngine *engine = QQmlEngine::contextForObject(this)->engine();
  QQmlImageProviderBase *imageProviderBase =
      engine->imageProvider(imageUrl.host());
  QQuickImageProvider *imageProvider =
      dynamic_cast<QQuickImageProvider *>(imageProviderBase);

  QSize imageSize;
  QString imageId = imageUrl.path().remove(0, 1);
  QImage image = imageProvider->requestImage(imageId, &imageSize, imageSize);
  //  qDebug() << " image.save" << QDir::currentPath();
  if (!image.isNull()) {
    setIsProcessing(true);
    resetNewImage(image);
    QtConcurrent::run([this, r = rows(), c = cols(), img = image]() {
      emit this->newResultReceived(calibrateCheckerboard(r, c, img));
    });
  }
}

QImage ChessCalibrator::lastImage() const { return _lastImage; }

void ChessCalibrator::setLastImage(const QImage &newLastImage) {
  if (_lastImage == newLastImage)
    return;
  _lastImage = newLastImage;
  emit lastImageChanged();
}

QString ChessCalibrator::lastImageAddress() const { return _lastImageAddress; }

void ChessCalibrator::setLastImageAddress(const QString &newLastImageAddress) {
  if (_lastImageAddress == newLastImageAddress)
    return;
  _lastImageAddress = newLastImageAddress;
  emit lastImageAddressChanged();
}

CalibrationResult ChessCalibrator::calibrateCheckerboard(int maxRows,
                                                         int maxCols,
                                                         QImage inputImage) {
  CalibrationResult result;

  // Convert QImage to cv::Mat
  cv::Mat frame, gray;
  frame = QImageToCvMat(inputImage);

  // Convert to grayscale
  cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

  // Creating vector to store vectors of 3D points for each checkerboard image
  std::vector<std::vector<cv::Point3f>> objpoints;

  // Creating vector to store vectors of 2D points for each checkerboard image
  std::vector<std::vector<cv::Point2f>> imgpoints;

  // Defining the world coordinates for 3D points
  std::vector<cv::Point3f> objp;
  for (int i = 0; i < maxRows; ++i) {
    for (int j = 0; j < maxCols; ++j)
      objp.push_back(cv::Point3f(j, i, 0));
  }

  // Detect corners
  std::vector<cv::Point2f> corner_pts;
  bool success = cv::findChessboardCorners(
      gray, cv::Size(maxCols, maxRows), corner_pts,
      cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK |
          cv::CALIB_CB_NORMALIZE_IMAGE);

  if (success) {
    result.success = true;
    cv::TermCriteria criteria(cv::TermCriteria::Type::EPS |
                                  cv::TermCriteria::Type::MAX_ITER,
                              30, 0.001);
    cv::cornerSubPix(gray, corner_pts, cv::Size(11, 11), cv::Size(-1, -1),
                     criteria);
    cv::drawChessboardCorners(frame, cv::Size(maxCols, maxRows), corner_pts,
                              success);

    // Fill objpoints and imgpoints
    objpoints.push_back(objp);
    imgpoints.push_back(corner_pts);

    // Calibrate camera
    cv::Mat cameraMatrix, distCoeffs, R, T;
    cv::calibrateCamera(objpoints, imgpoints, cv::Size(gray.rows, gray.cols),
                        cameraMatrix, distCoeffs, R, T);

    // Convert matrices to QList<QVector<double>>
    result.cameraMatrix = matToList(cameraMatrix);
    result.distCoeffs = matToList(distCoeffs);
    result.rotation = matToList(R);
    result.transformation = matToList(T);

    // Convert back to QImage
    result.paintedImage = CvMatToQImage(frame);
  } else
    return {};

  return result;
}

void ChessCalibrator::resetNewImage(const QImage &image) {
  static quint64 idImages = 0;
  auto const name =
      IMG_PROVIDER_FORMAT_ADDRESS.arg(IMG_PROVIDER_NAME).arg(++idImages);
  imgProvider->registerNewImage(QString::number(idImages), image);
  setLastImageAddress(name);
  setLastImage(image);
}

int ChessCalibrator::cols() const { return _cols; }

void ChessCalibrator::setCols(int newCols) {
  if (_cols == newCols)
    return;
  _cols = newCols;
  emit colsChanged();
}

int ChessCalibrator::rows() const { return _rows; }

void ChessCalibrator::setRows(int newRows) {
  if (_rows == newRows)
    return;
  _rows = newRows;
  emit rowsChanged();
}

bool ChessCalibrator::isProcessing() const { return _isProcessing; }

void ChessCalibrator::setIsProcessing(bool newIsProcessing) {
  if (_isProcessing == newIsProcessing)
    return;
  _isProcessing = newIsProcessing;
  emit isProcessingChanged();
}

QImage ChessImageProvider::requestImage(const QString &id, QSize *size,
                                        const QSize &requestedSize) {
  if (_name == id) {
    return _img;
  }
  return QQuickImageProvider::requestImage(id, size, requestedSize);
}

void CalibrationResult::print() const {

  qDebug() << "Found chess corners?" << success << cameraMatrix << distCoeffs
           << rotation << transformation;
}
