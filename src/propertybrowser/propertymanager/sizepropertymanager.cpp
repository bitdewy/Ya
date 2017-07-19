/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "sizepropertymanager.h"
#include "private/sizepropertymanager_p.h"
#include "propertymanager/intpropertymanager.h"
#include "propertymanager/private/propertymanagerutils_p.h"
#include "property.h"

SizePropertyManager::SizePropertyManager(QObject* parent) :
    AbstractPropertyManager(parent),
    impl_(new SizePropertyManagerPrivate)
{
    impl_->q_ptr = this;

    impl_->intPropertyManager_ = new IntPropertyManager(this);
    connect(impl_->intPropertyManager_, &IntPropertyManager::valueChanged,
        this, [this](Property* property, int value) { impl_->slotIntChanged(property, value); });
    connect(impl_->intPropertyManager_, &IntPropertyManager::propertyDestroyed,
        this, [this](Property* property) {impl_->slotPropertyDestroyed(property); });
}

SizePropertyManager::~SizePropertyManager()
{
    clear();
}

IntPropertyManager* SizePropertyManager::subIntPropertyManager() const
{
    return impl_->intPropertyManager_;
}

QSize SizePropertyManager::value(const Property* property) const
{
    return getValue<QSize>(impl_->values_, property);
}

QSize SizePropertyManager::minimum(const Property* property) const
{
    return getMinimum<QSize>(impl_->values_, property);
}

QSize SizePropertyManager::maximum(const Property *property) const
{
    return getMaximum<QSize>(impl_->values_, property);
}

QString SizePropertyManager::valueText(const Property *property) const
{
    const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QString();
    }
    const auto v = it.value().val;
    return tr("%1 x %2").arg(QString::number(v.width()))
                        .arg(QString::number(v.height()));
}

void SizePropertyManager::setValue(Property* property, const QSize& val)
{
    setValueInRange<const QSize&, SizePropertyManagerPrivate, SizePropertyManager, const QSize>(this, impl_.data(),
        &SizePropertyManager::propertyChanged,
        &SizePropertyManager::valueChanged,
        property, val, &SizePropertyManagerPrivate::setValue);
}

void SizePropertyManager::setMinimum(Property* property, const QSize& minVal)
{
    setBorderValue<const QSize&, SizePropertyManagerPrivate, SizePropertyManager, QSize, SizePropertyManagerPrivate::Data>(this, impl_.data(),
        &SizePropertyManager::propertyChanged,
        &SizePropertyManager::valueChanged,
        &SizePropertyManager::rangeChanged,
        property,
        &SizePropertyManagerPrivate::Data::minimumValue,
        &SizePropertyManagerPrivate::Data::setMinimumValue,
        minVal, &SizePropertyManagerPrivate::setRange);
}

void SizePropertyManager::setMaximum(Property *property, const QSize &maxVal)
{
    setBorderValue<const QSize&, SizePropertyManagerPrivate, SizePropertyManager, QSize, SizePropertyManagerPrivate::Data>(this, impl_.data(),
        &SizePropertyManager::propertyChanged,
        &SizePropertyManager::valueChanged,
        &SizePropertyManager::rangeChanged,
        property,
        &SizePropertyManagerPrivate::Data::maximumValue,
        &SizePropertyManagerPrivate::Data::setMaximumValue,
        maxVal, &SizePropertyManagerPrivate::setRange);
}

void SizePropertyManager::setRange(Property* property, const QSize& minVal, const QSize& maxVal)
{
    setBorderValues<const QSize&, SizePropertyManagerPrivate, SizePropertyManager, QSize>(this, impl_.data(),
        &SizePropertyManager::propertyChanged,
        &SizePropertyManager::valueChanged,
        &SizePropertyManager::rangeChanged,
        property, minVal, maxVal, &SizePropertyManagerPrivate::setRange);
}

void SizePropertyManager::initializeProperty(Property* property)
{
    impl_->values_[property] = {};

    auto wProp = impl_->intPropertyManager_->addProperty();
    wProp->setPropertyName(tr("Width"));
    impl_->intPropertyManager_->setValue(wProp, 0);
    impl_->intPropertyManager_->setMinimum(wProp, 0);
    impl_->propertyToW_[property] = wProp;
    impl_->wToProperty_[wProp] = property;
    property->addSubProperty(wProp);

    auto hProp = impl_->intPropertyManager_->addProperty();
    hProp->setPropertyName(tr("Height"));
    impl_->intPropertyManager_->setValue(hProp, 0);
    impl_->intPropertyManager_->setMinimum(hProp, 0);
    impl_->propertyToH_[property] = hProp;
    impl_->hToProperty_[hProp] = property;
    property->addSubProperty(hProp);
}

void SizePropertyManager::uninitializeProperty(Property* property)
{
    auto wProp = impl_->propertyToW_[property];
    if (wProp)
    {
        impl_->wToProperty_.remove(wProp);
        delete wProp;
    }
    impl_->propertyToW_.remove(property);

    auto hProp = impl_->propertyToH_[property];
    if (hProp)
    {
        impl_->hToProperty_.remove(hProp);
        delete hProp;
    }
    impl_->propertyToH_.remove(property);

    impl_->values_.remove(property);
}
