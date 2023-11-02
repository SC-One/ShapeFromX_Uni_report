#include "ShapeFromXBase.h"
#include <QDebug>

ShapeFromXBase::ShapeFromXBase(QObject *parent) : QObject{parent} {
  connect(this, &ShapeFromXBase::currentImageURLChanged, this,
          [this]() { calculateOutput(); });
}

QUrl ShapeFromXBase::currentImageURL() const { return _currentImageURL; }

void ShapeFromXBase::setCurrentImageURL(const QUrl &newCurrentImageURL) {
  if (_currentImageURL == newCurrentImageURL)
    return;
  _currentImageURL = newCurrentImageURL;
  emit currentImageURLChanged();
}

void ShapeFromXBase::reset(const QUrl &newCurrentImageURL) {
  setCurrentImageURL(QUrl());
  setCurrentImageURL(newCurrentImageURL);
}

void ShapeFromXBase::reCalculate(const QUrl &newCurrentImageURL) {
  reset(currentImageURL());
}

void ShapeFromXBase::calculateOutput() {
  qDebug() << "if you are watching this log, it means you forgot to "
              "re-implement subclass :|";
}
