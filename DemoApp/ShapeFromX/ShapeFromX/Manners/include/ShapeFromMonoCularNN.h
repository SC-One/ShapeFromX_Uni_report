#ifndef SHAPEFROMMONOCULARNN_H
#define SHAPEFROMMONOCULARNN_H

#include <Manners/ShapeFromXBase.h>
#include <QObject>

#include <QSurface3DSeries>
#include <QSurfaceDataArray>
#include <QThreadPool>
#include <QWebSocket>

using namespace QtDataVisualization;

class WebSocketClient : public QObject {
  Q_OBJECT

public:
  explicit WebSocketClient(const QUrl &url, QObject *parent = nullptr);

signals:
  void depthMapReceived(const QList<QVector<int>> &depthMap);

private slots:
  void onConnected();

  void onTextMessageReceived(const QString &message);

  void closed();

private:
  QWebSocket webSocket;
  QThreadPool _pool;
};

class ShapeFromMonoCularNN : public ShapeFromXBase {
  Q_OBJECT
  Q_PROPERTY(QSurface3DSeries *mySurfaceSeries READ series WRITE setSeries
                 NOTIFY seriesChanged FINAL)

public:
  explicit ShapeFromMonoCularNN(QObject *parent = nullptr);

  virtual void calculateOutput() override final;

  Q_INVOKABLE QSurface3DSeries *series() const;
  Q_INVOKABLE void setSeries(QSurface3DSeries *newSeries);

signals:
  void seriesChanged();

private:
  void onDepthMapreceived(const QList<QVector<int>> &data);

  void clearData();

private:
  WebSocketClient _client;
  QSurface3DSeries *_series;
  QSurfaceDataArray *_myData;
  QList<QVariantList> _calculatedDepthMap;
  int _lastRows, _lastCols;
};

#endif // SHAPEFROMMONOCULARNN_H
