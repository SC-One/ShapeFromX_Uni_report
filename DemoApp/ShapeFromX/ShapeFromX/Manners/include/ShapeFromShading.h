#ifndef SHAPEFROMSHADING_H
#define SHAPEFROMSHADING_H
#include <Manners/ShapeFromXBase.h>
#include <QScopedPointer>
#include <QString>

class ShapeFromShading : public ShapeFromXBase {
  Q_OBJECT
public:
  explicit ShapeFromShading(QObject *parent = nullptr);

  virtual void calculateOutput() override;

  Q_INVOKABLE QString outputFileName() const;

private:
  // TODO: save output
  QString _outputFileName;
};

#endif // SHAPEFROMSHADING_H
