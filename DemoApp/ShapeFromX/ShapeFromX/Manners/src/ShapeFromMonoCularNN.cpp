#include <Manners/include/ShapeFromMonoCularNN.h>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QSurface3DSeries>
#include <QSurfaceDataArray>
#include <QTimer>
#include <QUrl>

using namespace QtDataVisualization;
Q_DECLARE_METATYPE(QSurface3DSeries *)

ShapeFromMonoCularNN::ShapeFromMonoCularNN(QObject *parent)
    : ShapeFromXBase{parent},
      _client(QUrl(QStringLiteral("ws://localhost:50777"))), _series(nullptr),
      _resetArray(nullptr), _lastRows(-1), _lastCols(-1) {

  qRegisterMetaType<QSurface3DSeries *>();

  connect(&_client, &WebSocketClient::depthMapReceived, this,
          &ShapeFromMonoCularNN::onDepthMapreceived);
  connect(this, &ShapeFromMonoCularNN::dataChanged, this,
          &ShapeFromMonoCularNN::outputCalculated, Qt::DirectConnection);
}

void ShapeFromMonoCularNN::calculateOutput() {
  // leave it nothing (because local server-process is doing that)
}

void ShapeFromMonoCularNN::onDepthMapreceived(
    QVector<QVector<double>> const &data) {
  //  qDebug() << "Data:" << data;

  {
    if (_series && _data.size()) {

      int newRowCount = data.size();
      int newColumnCount = 0;
      if (newRowCount > 0)
        newColumnCount = data.at(0).size();

      if (!_resetArray || _series->dataProxy()->rowCount() != newRowCount ||
          _series->dataProxy()->columnCount() != newColumnCount) {
        if (nullptr != _resetArray) {
          delete _resetArray;
        }
        _resetArray = new QSurfaceDataArray();
        _resetArray->reserve(newRowCount);
        for (int i(0); i < newRowCount; i++)
          _resetArray->append(new QSurfaceDataRow(newColumnCount));
      }

      // Copy items from our cache to the reset array
      for (int i = 0; i < newRowCount; ++i) {
        for (int j = 0; j < newColumnCount; ++j) {
          _resetArray->at(i)->at(j).position().setZ(data[i][j]);
        }
      }

      // Notify the proxy that data has changed
      _series->dataProxy()->resetArray(_resetArray);
    }
  }
}

void ShapeFromMonoCularNN::clearData() {
  for (int i = 0; i < _data.size(); ++i) {
    QSurfaceDataArray &array = _data[i];
    for (int j(0); j < array.size(); j++)
      delete array[j];
    array.clear();
  }
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

  connect(&webSocket, &QWebSocket::disconnected, this, [this, url]() {
    QTimer::singleShot(3000, [this, url]() {
      qDebug() << "Retrying to connect(3s): " << url;
      if (QAbstractSocket::SocketState::ConnectedState == webSocket.state()) {
        return;
      } else if (QAbstractSocket::SocketState::UnconnectedState ==
                 webSocket.state()) {
        webSocket.open(url);
      }
    });
  });
  webSocket.open(url);
}

void WebSocketClient::onConnected() {
  connect(&webSocket, &QWebSocket::textMessageReceived, this,
          &WebSocketClient::onTextMessageReceived);
}

void WebSocketClient::onTextMessageReceived(const QString &message) {
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

  emit depthMapReceived(depthMap);
}

void WebSocketClient::closed() {
  // Handle WebSocket closed event if needed
}
