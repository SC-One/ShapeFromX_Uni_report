#ifndef SHAPEFROMXBASE_H
#define SHAPEFROMXBASE_H

#include <QObject>
#include <QUrl>

class ShapeFromXBase : public QObject {
  Q_OBJECT
  Q_PROPERTY(QUrl currentImageURL READ currentImageURL WRITE setCurrentImageURL
                 NOTIFY currentImageURLChanged FINAL)
public:
  explicit ShapeFromXBase(QObject *parent = nullptr);

  Q_INVOKABLE QUrl currentImageURL() const;
  Q_INVOKABLE void setCurrentImageURL(const QUrl &newCurrentImageURL);
  Q_INVOKABLE void reset(const QUrl &newCurrentImageURL);
  Q_INVOKABLE void reCalculate(const QUrl &newCurrentImageURL);

  /// default impl do nothing
  /// All derived should reimplement this; they can use threading or anything
  /// else, but they should be aware of their implementation vs. the whole
  /// design.
  virtual void calculateOutput();
signals:
  void currentImageURLChanged();
  void outputCalculated();

protected:
  QUrl _currentImageURL;
};

#endif // SHAPEFROMXBASE_H
