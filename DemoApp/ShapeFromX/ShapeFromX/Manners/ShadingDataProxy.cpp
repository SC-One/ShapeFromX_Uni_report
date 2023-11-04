#include <Manners/ShadingDataProxy.h>
ShadingDataProxy::ShadingDataProxy(QObject *parent)
    : QAbstractListModel(parent) {}

int ShadingDataProxy::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
  return _pureData.size();
}

QVariant ShadingDataProxy::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() >= _pureData.size())
    return QVariant();

  const QPair<QPoint, int> &item = _pureData.at(index.row());

  if (item.second == 0)
    Q_ASSERT(false);
  if (role == PointRoleX)
    return QVariant(QString::number(item.first.x()));
  if (role == PointRoleY)
    return QVariant(QString::number(item.first.y()));
  else if (role == ValueRole)
    return QVariant(QString::number(item.second));

  return QVariant();
}

QHash<int, QByteArray> ShadingDataProxy::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[PointRoleX] = "pointRoleX";
  roles[PointRoleY] = "pointRoleY";
  roles[ValueRole] = "valueRole";
  return roles;
}

void ShadingDataProxy::setMyData(const QVector<QPair<QPoint, int>> &data) {
  beginResetModel();
  _pureData = data;
  endResetModel();
}
