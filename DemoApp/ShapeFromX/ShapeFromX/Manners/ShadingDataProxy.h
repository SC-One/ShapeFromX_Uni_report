#ifndef SHADINGDATAPROXY_H
#define SHADINGDATAPROXY_H
#include <QAbstractListModel>
#include <QPair>
#include <QPoint>
#include <QVector>

class ShadingDataProxy : public QAbstractListModel {
  Q_OBJECT

public:
  ShadingDataProxy(QObject *parent = nullptr);

  enum { PointRoleX = Qt::UserRole, PointRoleY, ValueRole };

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  QHash<int, QByteArray> roleNames() const override;

  void setMyData(const QVector<QPair<QPoint, int>> &data);

private:
  QVector<QPair<QPoint, int>> _pureData;
};

#endif // SHADINGDATAPROXY_H
