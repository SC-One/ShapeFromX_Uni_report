#include <Manners/include/ShapeFromMonoCularNN.h>
#include <QDateTime>
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
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

class TimeMeasurer {
public:
  explicit TimeMeasurer(QString const &name) : _name(name) {
    started = QDateTime::currentMSecsSinceEpoch();
  }

  ~TimeMeasurer() {
    qint64 elapsedTime = QDateTime::currentMSecsSinceEpoch() - started;
    qDebug() << _name << "Time elapsed: " << elapsedTime << "milliseconds";
  }

private:
  QString _name;
  qint64 started;
};

using namespace QtDataVisualization;
Q_DECLARE_METATYPE(QSurface3DSeries *)

ShapeFromMonoCularNN::ShapeFromMonoCularNN(QObject *parent)
    : ShapeFromXBase{parent},
      _client(QUrl(QStringLiteral("ws://localhost:50777"))), _series(nullptr),
      _myData(nullptr), _lastRows(-1), _lastCols(-1) {

  qRegisterMetaType<QSurface3DSeries *>();
  qRegisterMetaType<QList<QVector<int>>>("QVector<QVector<int> >");

  connect(&_client, &WebSocketClient::depthMapReceived, this,
          &ShapeFromMonoCularNN::onDepthMapreceived);
}

void ShapeFromMonoCularNN::calculateOutput() {
  // leave it nothing (because local server-process is doing that)
}

static QList<QVariantList>
convertToQVariantList(const QList<QVector<int>> &depthMap) {
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

void ShapeFromMonoCularNN::onDepthMapreceived(QList<QVector<int>> const &data) {
  //  qDebug() << "Data:" << data;
  //  TimeMeasurer measure("onDepthMapreceived");

  if (_series && !data.isEmpty()) {

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

      for (int i = 0; i < newRowCount; ++i) {
        QSurfaceDataRow &row = *(*_myData)[i];
        for (int j = 0; j < newColumnCount; ++j) {
          row[j].setPosition({static_cast<float>(j), static_cast<float>(0),
                              static_cast<float>(i)});
        }
      }
    }

    for (int i = 0; i < newRowCount; ++i) {
      QSurfaceDataRow &row = *(*_myData)[i];
      for (int j = 0; j < newColumnCount; ++j) {
        qreal zPosition = data[i][j];
        row[j].setY(zPosition);
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
  _pool.setMaxThreadCount(30);
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

  //  TimeMeasurer measure("onTextMessageReceived");

  // Convert QString to std::string
  std::string utf8 = message.toStdString();

  // Parse JSON using RapidJSON
  rapidjson::Document document;
  document.Parse(utf8.c_str());

  if (!document.HasParseError() && document.IsArray()) {
    const rapidjson::Value &depthMapArray = document;

    // Process the depth map more efficiently without deeply nested loops
    QList<QVector<int>> depthData;
    for (rapidjson::SizeType i = 0; i < depthMapArray.Size(); ++i) {
      const rapidjson::Value &rowValue = depthMapArray[i];
      if (rowValue.IsArray()) {
        const rapidjson::Value &rowArray = rowValue;

        QVector<int> depthRow;
        for (rapidjson::SizeType j = 0; j < rowArray.Size(); ++j) {
          if (rowArray[j].IsInt()) {
            depthRow.append(rowArray[j].GetInt());
          }
        }
        depthData.append(depthRow);
      }
    }

    if (!depthData.isEmpty()) {
      emit depthMapReceived(depthData);
    }
  }
}

void WebSocketClient::closed() {
  // Handle WebSocket closed event if needed
}
