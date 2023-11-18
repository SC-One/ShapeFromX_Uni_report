#ifndef SHAPEFROMMONOCULARNN_H
#define SHAPEFROMMONOCULARNN_H

#include <Manners/ShapeFromXBase.h>
#include <QObject>

#include <QSurface3DSeries>
#include <QSurfaceDataArray>
#include <QWebSocket>

using namespace QtDataVisualization;

class WebSocketClient : public QObject {
  Q_OBJECT

public:
  explicit WebSocketClient(const QUrl &url, QObject *parent = nullptr);

signals:
  void depthMapReceived(const QList<QVariantList> &depthMap);

private slots:
  void onConnected();

  void onTextMessageReceived(const QString &message);

  void closed();

private:
  QWebSocket webSocket;
};

class ShapeFromMonoCularNN : public ShapeFromXBase {
  Q_OBJECT
  Q_PROPERTY(QSurface3DSeries *mySurfaceSeries READ series WRITE setSeries
                 NOTIFY seriesChanged FINAL)

  Q_PROPERTY(
      QList<QVariantList> calculatedDepthMap READ calculatedDepthMap WRITE
          setCalculatedDepthMap NOTIFY calculatedDepthMapChanged FINAL)
public:
  explicit ShapeFromMonoCularNN(QObject *parent = nullptr);

  virtual void calculateOutput() override final;

  Q_INVOKABLE QSurface3DSeries *series() const;
  Q_INVOKABLE void setSeries(QSurface3DSeries *newSeries);

  QList<QVariantList> calculatedDepthMap() const;
  void setCalculatedDepthMap(const QList<QVariantList> &newCalculatedDepthMap);

signals:

  void seriesChanged();

  void calculatedDepthMapChanged();

private:
  void onDepthMapreceived(const QList<QVariantList> &data);

  void clearData();

private:
  WebSocketClient _client;
  QSurface3DSeries *_series;
  QSurfaceDataArray *_myData;
  QList<QVariantList> _calculatedDepthMap;
  int _lastRows, _lastCols;
};

#endif // SHAPEFROMMONOCULARNN_H
