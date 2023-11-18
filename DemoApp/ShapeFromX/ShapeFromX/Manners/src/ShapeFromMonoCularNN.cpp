#include <Manners/include/ShapeFromMonoCularNN.h>
#include <QElapsedTimer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QSurface3DSeries>
#include <QSurfaceDataArray>
#include <QTimer>
#include <QUrl>
#include <QtConcurrent>

class TimeMeasurer {
public:
  explicit TimeMeasurer(QString const &name) : _name(name) { timer.start(); }

  ~TimeMeasurer() {
    qint64 elapsedTime = timer.elapsed();
    qDebug() << _name << "Time elapsed: " << elapsedTime << "milliseconds";
  }

private:
  QString _name;
  QElapsedTimer timer;
};

using namespace QtDataVisualization;
Q_DECLARE_METATYPE(QSurface3DSeries *)

ShapeFromMonoCularNN::ShapeFromMonoCularNN(QObject *parent)
    : ShapeFromXBase{parent},
      _client(QUrl(QStringLiteral("ws://localhost:50777"))), _series(nullptr),
      _myData(nullptr), _lastRows(-1), _lastCols(-1) {

  qRegisterMetaType<QSurface3DSeries *>();
  qRegisterMetaType<QVector<QVector<double>>>("QVector<QVector<double> >");

  connect(&_client, &WebSocketClient::depthMapReceived, this,
          &ShapeFromMonoCularNN::onDepthMapreceived);
  connect(this, &ShapeFromMonoCularNN::calculatedDepthMapChanged, this,
          &ShapeFromMonoCularNN::outputCalculated, Qt::DirectConnection);
}

void ShapeFromMonoCularNN::calculateOutput() {
  // leave it nothing (because local server-process is doing that)
}

static QList<QVariantList>
convertToQVariantList(const QVector<QVector<double>> &depthMap) {
  QList<QVariantList> resultList;

  for (const auto &row : depthMap) {
    QVariantList rowList;
    for (double value : row) {
      rowList.append(value);
    }
    resultList.append(rowList);
  }

  return resultList;
}

void ShapeFromMonoCularNN::onDepthMapreceived(QList<QVariantList> const &data) {
  //  qDebug() << "Data:" << data;
  {
    TimeMeasurer measure("onDepthMapreceived");
    setCalculatedDepthMap(data);
    return;
  }

  if (_series && data.size()) {

    int newRowCount = data.size();
    int newColumnCount = 0;
    if (newRowCount > 0)
      newColumnCount = data.at(0).size();

    if (!_myData || _series->dataProxy()->rowCount() != newRowCount ||
        _series->dataProxy()->columnCount() != newColumnCount) {

      clearData();
      _myData = new QSurfaceDataArray();
      _myData->reserve(newRowCount);
      for (int i = 0; i < newRowCount; ++i)
        _myData->append(new QSurfaceDataRow(newColumnCount));
    }

    for (int i = 0; i < newRowCount; ++i) {
      for (int j = 0; j < newColumnCount; ++j) {
        _myData->at(i)->at(j).position().setZ(i * j * 200);
      }
    }

    _series->dataProxy()->resetArray(_myData);
  }
  emit outputCalculated();
}

void ShapeFromMonoCularNN::clearData() {
  _series->dataProxy()->resetArray(nullptr);
  if (nullptr == _myData)
    return;
  for (int i = 0; i < _myData->size(); ++i) {
    QSurfaceDataArray &array = _myData[i];
    for (int j(0); j < array.size(); j++)
      delete array[j];
    array.clear();
  }
  _lastRows = 0;
  _lastCols = 0;
}

QList<QVariantList> ShapeFromMonoCularNN::calculatedDepthMap() const {
  return _calculatedDepthMap;
}

void ShapeFromMonoCularNN::setCalculatedDepthMap(
    const QList<QVariantList> &newCalculatedDepthMap) {
  _calculatedDepthMap = newCalculatedDepthMap;
  emit calculatedDepthMapChanged();
}

QtDataVisualization::QSurface3DSeries *ShapeFromMonoCularNN::series() const {
  return _series;
}

void ShapeFromMonoCularNN::setSeries(
    QtDataVisualization::QSurface3DSeries *newSeries) {
  if (_series == newSeries)
    return;
  _series = newSeries;
  emit seriesChanged();
}

WebSocketClient::WebSocketClient(const QUrl &url, QObject *parent)
    : QObject(parent) {
  connect(&webSocket, &QWebSocket::connected, this,
          &WebSocketClient::onConnected);
  connect(&webSocket, &QWebSocket::disconnected, this,
          &WebSocketClient::closed);

  //  connect(&webSocket, &QWebSocket::disconnected, this, [this, url]() {
  //    QTimer::singleShot(3000, [this, url]() {
  //      qDebug() << "Retrying to connect(3s): " << url;
  //      if (QAbstractSocket::SocketState::ConnectedState == webSocket.state())
  //      {
  //        return;
  //      } else if (QAbstractSocket::SocketState::UnconnectedState ==
  //                 webSocket.state()) {
  //        webSocket.open(url);
  //      }
  //    });
  //  });
  webSocket.open(url);
}

void WebSocketClient::onConnected() {
  connect(&webSocket, &QWebSocket::textMessageReceived, this,
          &WebSocketClient::onTextMessageReceived);
}

void WebSocketClient::onTextMessageReceived(const QString &message) {
  QtConcurrent::run([this, message]() {
    TimeMeasurer measure("onTextMessageReceived");
    // simple inline parser
    QJsonDocument jsonDoc = QJsonDocument::fromJson(message.toUtf8());
    if (!jsonDoc.isArray())
      return;

    QJsonArray jsonArray = jsonDoc.array();
    QVector<QVector<double>> depthMap;

    for (const QJsonValue &row : jsonArray) {
      if (!row.isArray())
        continue;

      QJsonArray rowArray = row.toArray();
      QVector<double> rowData;

      for (const QJsonValue &value : rowArray) {
        rowData.append(value.toDouble());
      }

      depthMap.append(rowData);
    }

    emit depthMapReceived(convertToQVariantList(depthMap));
  });
}

void WebSocketClient::closed() {
  // Handle WebSocket closed event if needed
}
