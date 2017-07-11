/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "colorpropertymanager.h"
#include "property.h"
#include "propertymanager/intpropertymanager.h"
#include "private/colorpropertymanager_p.h"
#include "private/propertybrowserutils_p.h"
#include <QtGui/QColor>
#include <QtGui/QIcon>
#include <QtGui/QBrush>

ColorPropertyManager::ColorPropertyManager(QObject* parent) :
    AbstractPropertyManager(parent),
    impl_(new ColorPropertyManagerPrivate)
{
    impl_->colorPropertyManagerPtr_ = this;

    impl_->intPropertyManager_ = new IntPropertyManager(this);
    connect(impl_->intPropertyManager_, &IntPropertyManager::valueChanged,
        this, [this](Property* property, int value) { impl_->slotIntChanged(property, value); });

    connect(impl_->intPropertyManager_, &IntPropertyManager::propertyDestroyed,
        this, [this](Property* property) { impl_->slotPropertyDestroyed(property); });
}

ColorPropertyManager::~ColorPropertyManager()
{
    clear();
}

IntPropertyManager *ColorPropertyManager::subIntPropertyManager() const
{
    return impl_->intPropertyManager_;
}

QColor ColorPropertyManager::value(const Property* property) const
{
    return impl_->values_.value(property, QColor());
}

QString ColorPropertyManager::valueText(const Property* property) const
{
    const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QString();
    }
    return PropertyBrowserUtils::colorValueText(it.value());
}

QIcon ColorPropertyManager::valueIcon(const Property* property) const
{
    const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QIcon();
    }
    return PropertyBrowserUtils::brushValueIcon(QBrush(it.value()));
}

void ColorPropertyManager::setValue(Property* property, const QColor& val)
{
    const auto it = impl_->values_.find(property);
    if (it == impl_->values_.end())
    {
        return;
    }
    if (it.value() == val)
    {
        return;
    }
    it.value() = val;

    impl_->intPropertyManager_->setValue(impl_->propertyToR_[property], val.red());
    impl_->intPropertyManager_->setValue(impl_->propertyToG_[property], val.green());
    impl_->intPropertyManager_->setValue(impl_->propertyToB_[property], val.blue());
    impl_->intPropertyManager_->setValue(impl_->propertyToA_[property], val.alpha());

    emit propertyChanged(property);
    emit valueChanged(property, val);
}

void ColorPropertyManager::initializeProperty(Property* property)
{
    QColor val;
    impl_->values_[property] = val;

    auto rProp = impl_->intPropertyManager_->addProperty();
    rProp->setPropertyName(tr("Red"));
    impl_->intPropertyManager_->setValue(rProp, val.red());
    impl_->intPropertyManager_->setRange(rProp, 0, 0xFF);
    impl_->propertyToR_[property] = rProp;
    impl_->rToProperty_[rProp] = property;
    property->addSubProperty(rProp);

    auto gProp = impl_->intPropertyManager_->addProperty();
    gProp->setPropertyName(tr("Green"));
    impl_->intPropertyManager_->setValue(gProp, val.green());
    impl_->intPropertyManager_->setRange(gProp, 0, 0xFF);
    impl_->propertyToG_[property] = gProp;
    impl_->gToProperty_[gProp] = property;
    property->addSubProperty(gProp);

    auto bProp = impl_->intPropertyManager_->addProperty();
    bProp->setPropertyName(tr("Blue"));
    impl_->intPropertyManager_->setValue(bProp, val.blue());
    impl_->intPropertyManager_->setRange(bProp, 0, 0xFF);
    impl_->propertyToB_[property] = bProp;
    impl_->bToProperty_[bProp] = property;
    property->addSubProperty(bProp);

    auto aProp = impl_->intPropertyManager_->addProperty();
    aProp->setPropertyName(tr("Alpha"));
    impl_->intPropertyManager_->setValue(aProp, val.alpha());
    impl_->intPropertyManager_->setRange(aProp, 0, 0xFF);
    impl_->propertyToA_[property] = aProp;
    impl_->aToProperty_[aProp] = property;
    property->addSubProperty(aProp);
}

void ColorPropertyManager::uninitializeProperty(Property* property)
{
    auto rProp = impl_->propertyToR_[property];
    if (rProp)
    {
        impl_->rToProperty_.remove(rProp);
        delete rProp;
    }
    impl_->propertyToR_.remove(property);

    auto gProp = impl_->propertyToG_[property];
    if (gProp)
    {
        impl_->gToProperty_.remove(gProp);
        delete gProp;
    }
    impl_->propertyToG_.remove(property);

    auto bProp = impl_->propertyToB_[property];
    if (bProp)
    {
        impl_->bToProperty_.remove(bProp);
        delete bProp;
    }
    impl_->propertyToB_.remove(property);

    auto aProp = impl_->propertyToA_[property];
    if (aProp)
    {
        impl_->aToProperty_.remove(aProp);
        delete aProp;
    }
    impl_->propertyToA_.remove(property);

    impl_->values_.remove(property);
}
