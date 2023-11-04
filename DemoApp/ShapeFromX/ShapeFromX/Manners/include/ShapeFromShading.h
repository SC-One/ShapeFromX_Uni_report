#ifndef SHAPEFROMSHADING_H
#define SHAPEFROMSHADING_H
#include <Manners/ShadingDataProxy.h>
#include <Manners/ShapeFromXBase.h>
#include <QScopedPointer>
#include <QString>

class ShapeFromShading : public ShapeFromXBase {
  Q_OBJECT
  Q_PROPERTY(QAbstractListModel *myProxyData READ myProxyData NOTIFY
                 myProxyDataChanged FINAL)
public:
  explicit ShapeFromShading(QObject *parent = nullptr);

  virtual void calculateOutput() override;

  Q_INVOKABLE QString outputFileName() const;

  Q_INVOKABLE QString fullOutNormalFileName() const;
  Q_INVOKABLE QString fullOutInverseFileName() const;

  Q_INVOKABLE static QString normalPostFix();
  Q_INVOKABLE static QString reversePostFix();
  Q_INVOKABLE static QString defaultImageExtension();

  QAbstractListModel *myProxyData() const;

signals:
  void myProxyDataChanged();

private:
  // TODO: save output
  QString _outputFileName;
  ShadingDataProxy *_myProxyData;
};

#endif // SHAPEFROMSHADING_H
