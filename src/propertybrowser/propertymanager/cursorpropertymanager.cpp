/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "cursorpropertymanager.h"
#include "cursordatabase.h"
#include "private/cursorpropertymanager_p.h"
#include <QtCore/QCoreApplication>
#include <QtGui/QCursor>

static void clearCursorDatabase();

namespace
{
struct CursorDatabaseImpl : public CursorDatabase
{
    CursorDatabaseImpl()
    {
        qAddPostRoutine(clearCursorDatabase);
    }
};
}

Q_GLOBAL_STATIC(CursorDatabase, cursorDatabase)

static void clearCursorDatabase()
{
    cursorDatabase()->clear();
}

CursorPropertyManager::CursorPropertyManager(QObject* parent) :
    AbstractPropertyManager(parent),
    impl_(new CursorPropertyManagerPrivate)
{
    impl_->cursorPropertyManagerPtr_ = this;
}

CursorPropertyManager::~CursorPropertyManager()
{
    clear();
}

QCursor CursorPropertyManager::value(const Property* property) const
{
    return impl_->values_.value(property, QCursor());
}

QString CursorPropertyManager::valueText(const Property* property) const
{
   const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QString();
    }
    return cursorDatabase()->cursorToShapeName(it.value());
}

QIcon CursorPropertyManager::valueIcon(const Property* property) const
{
    const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QIcon();
    }
    return cursorDatabase()->cursorToShapeIcon(it.value());
}

void CursorPropertyManager::setValue(Property* property, const QCursor& value)
{
    const auto it = impl_->values_.find(property);
    if (it == impl_->values_.end())
    {
        return;
    }
    if (it.value().shape() == value.shape() && value.shape() != Qt::BitmapCursor)
    {
        return;
    }
    it.value() = value;

    emit propertyChanged(property);
    emit valueChanged(property, value);
}

void CursorPropertyManager::initializeProperty(Property *property)
{
    impl_->values_[property] = {};
}

void CursorPropertyManager::uninitializeProperty(Property* property)
{
    impl_->values_.remove(property);
}
