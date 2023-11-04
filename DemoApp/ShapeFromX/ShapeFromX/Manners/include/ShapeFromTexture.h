#ifndef SHAPEFROMTEXTURE_H
#define SHAPEFROMTEXTURE_H
#include <Manners/ShapeFromXBase.h>

#include <QObject>

class ShapeFromTexture : public ShapeFromXBase {
  Q_OBJECT
public:
  explicit ShapeFromTexture(QObject *parent = nullptr);

  virtual void calculateOutput() override;
};

#endif // SHAPEFROMTEXTURE_H
