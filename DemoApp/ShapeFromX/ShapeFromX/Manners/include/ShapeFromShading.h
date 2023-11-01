#ifndef SHAPEFROMSHADING_H
#define SHAPEFROMSHADING_H

#include <Manners/ShapeFromXBase.h>

class ShapeFromShading : public ShapeFromXBase {
  Q_OBJECT
public:
  explicit ShapeFromShading(QObject *parent = nullptr);

  virtual void calculateOutput() override;

private:
  // TODO: save output
};

#endif // SHAPEFROMSHADING_H
