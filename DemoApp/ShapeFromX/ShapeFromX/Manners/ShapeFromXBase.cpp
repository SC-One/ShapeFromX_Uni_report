#include "ShapeFromXBase.h"

ShapeFromXBase::ShapeFromXBase(QObject *parent) : QObject{parent} {
  connect(this, &ShapeFromXBase::currentImageURLChanged, this,
          &ShapeFromXBase::calculateOutput);
}

QUrl ShapeFromXBase::currentImageURL() const { return _currentImageURL; }

void ShapeFromXBase::setCurrentImageURL(const QUrl &newCurrentImageURL) {
  if (_currentImageURL == newCurrentImageURL)
    return;
  _currentImageURL = newCurrentImageURL;
  emit currentImageURLChanged();
}

void ShapeFromXBase::calculateOutput() {}
