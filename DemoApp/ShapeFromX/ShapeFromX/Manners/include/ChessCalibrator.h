#ifndef CHESSCALIBRATOR_H
#define CHESSCALIBRATOR_H

#include <QObject>
#include <QQuickImageProvider>
class ChessImageProvider : public QQuickImageProvider {
public:
  ChessImageProvider() : QQuickImageProvider(QQuickImageProvider::Image) {}

  void registerNewImage(QString const &name, QImage const &image) {
    _name = name;
    _img = image;
  }
  QImage requestImage(const QString &id, QSize *size,
                      const QSize &requestedSize) override;

private:
  QString _name;
  QImage _img;
};

struct CalibrationResult {
  bool success = false;
  QImage paintedImage;
  QList<QVector<double>> cameraMatrix;
  QList<QVector<double>> distCoeffs;
  QList<QVector<double>> rotation;
  QList<QVector<double>> transformation;
  void print() const;
};
class ChessCalibrator : public QObject {
  Q_OBJECT

  Q_PROPERTY(QImage lastImage READ lastImage WRITE setLastImage NOTIFY
                 lastImageChanged FINAL)
  Q_PROPERTY(QString lastImageAddress READ lastImageAddress WRITE
                 setLastImageAddress NOTIFY lastImageAddressChanged FINAL)

  Q_PROPERTY(bool isProcessing READ isProcessing WRITE setIsProcessing NOTIFY
                 isProcessingChanged FINAL)

  Q_PROPERTY(int rows READ rows WRITE setRows NOTIFY rowsChanged FINAL)
  Q_PROPERTY(int cols READ cols WRITE setCols NOTIFY colsChanged FINAL)

public:
  explicit ChessCalibrator(QObject *parent = nullptr);
  Q_INVOKABLE void processImage(const QString &image);

  QImage lastImage() const;
  void setLastImage(const QImage &newLastImage);

  QString lastImageAddress() const;
  void setLastImageAddress(const QString &newLastImageAddress);

  bool isProcessing() const;
  void setIsProcessing(bool newIsProcessing);

  int rows() const;
  void setRows(int newRows);

  int cols() const;
  void setCols(int newCols);

signals:
  void lastImageChanged();

  void lastImageAddressChanged();

  void newResultReceived(CalibrationResult result);

  void isProcessingChanged();

  void rowsChanged();

  void colsChanged();

private:
  static CalibrationResult calibrateCheckerboard(int maxRows, int maxCols,
                                                 QImage inputImage);
  void resetNewImage(QImage const &img);

private:
  QImage _lastImage;
  ChessImageProvider *imgProvider;
  QString _lastImageAddress;
  bool _isProcessing = false;
  int _rows = 7;
  int _cols = 7;
};

#endif // CHESSCALIBRATOR_H
