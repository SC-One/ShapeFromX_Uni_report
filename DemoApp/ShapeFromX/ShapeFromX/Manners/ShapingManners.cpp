#include "ShapingManners.h"
#include "ShapeMannerHeaders.h" // multiple includer

#include <QQmlContext>
#include <QQmlEngine>
#include <typeinfo>

#define COMPANY_URI "ir.HCoding.SocietyCleaner"
static constexpr auto MAJOR_VERSION = 1;
static constexpr auto MINOR_VERSION = 0;

#define ADD_THE_MANNER(MANNER)                                                 \
  qmlRegisterType<MANNER>(COMPANY_URI, MAJOR_VERSION, MINOR_VERSION, #MANNER);

namespace Manners {
void registerMannersToQuickEngine() { ADD_THE_MANNER(ShapeFromShading); }
} // namespace Manners
