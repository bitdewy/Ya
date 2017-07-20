/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "sizefpropertymanager.h"
#include "private/sizefpropertymanager_p.h"
#include "propertymanager/doublepropertymanager.h"
#include "propertymanager/private/propertymanagerutils_p.h"
#include "property.h"

SizeFPropertyManager::SizeFPropertyManager(QObject *parent) :
    AbstractPropertyManager(parent),
    impl_(new SizeFPropertyManagerPrivate)
{
    impl_->sizeFPropertyManagerPtr_ = this;

    impl_->doublePropertyManager_ = new DoublePropertyManager(this);
    connect(impl_->doublePropertyManager_, &DoublePropertyManager::valueChanged,
        this, [this](Property* property, double value) { impl_->slotDoubleChanged(property, value); });
    connect(impl_->doublePropertyManager_, &DoublePropertyManager::propertyDestroyed,
        this, [this](Property* property) { impl_->slotPropertyDestroyed(property); });
}

SizeFPropertyManager::~SizeFPropertyManager()
{
    clear();
}

DoublePropertyManager* SizeFPropertyManager::subDoublePropertyManager() const
{
    return impl_->doublePropertyManager_;
}

QSizeF SizeFPropertyManager::value(const Property* property) const
{
    return getValue<QSizeF>(impl_->values_, property);
}

int SizeFPropertyManager::decimals(const Property* property) const
{
    return getData<int>(impl_->values_, &SizeFPropertyManagerPrivate::Data::decimals, property, 0);
}

QSizeF SizeFPropertyManager::minimum(const Property* property) const
{
    return getMinimum<QSizeF>(impl_->values_, property);
}

QSizeF SizeFPropertyManager::maximum(const Property* property) const
{
    return getMaximum<QSizeF>(impl_->values_, property);
}

QString SizeFPropertyManager::valueText(const Property* property) const
{
    const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QString();
    }
    const auto v = it.value().val;
    const auto dec = it.value().decimals;
    return tr("%1 x %2").arg(QString::number(v.width(), 'f', dec))
                        .arg(QString::number(v.height(), 'f', dec));
}

void SizeFPropertyManager::setValue(Property* property, const QSizeF& val)
{
    setValueInRange<const QSizeF&, SizeFPropertyManagerPrivate, SizeFPropertyManager, QSizeF>(this, impl_.data(),
        &SizeFPropertyManager::propertyChanged,
        &SizeFPropertyManager::valueChanged,
        property, val, &SizeFPropertyManagerPrivate::setValue);
}

void SizeFPropertyManager::setDecimals(Property* property, int prec)
{
    const auto it = impl_->values_.find(property);
    if (it == impl_->values_.end())
    {
        return;
    }
    auto data = it.value();

    if (prec > 13)
    {
        prec = 13;
    }
    else if (prec < 0)
    {
        prec = 0;
    }
    if (data.decimals == prec)
    {
        return;
    }
    data.decimals = prec;
    impl_->doublePropertyManager_->setDecimals(impl_->propertyToW_[property], prec);
    impl_->doublePropertyManager_->setDecimals(impl_->propertyToH_[property], prec);

    it.value() = data;

    emit decimalsChanged(property, data.decimals);
}

void SizeFPropertyManager::setMinimum(Property* property, const QSizeF& minVal)
{
    setBorderValue<const QSizeF&, SizeFPropertyManagerPrivate, SizeFPropertyManager, QSizeF, SizeFPropertyManagerPrivate::Data>(this, impl_.data(),
        &SizeFPropertyManager::propertyChanged,
        &SizeFPropertyManager::valueChanged,
        &SizeFPropertyManager::rangeChanged,
        property,
        &SizeFPropertyManagerPrivate::Data::minimumValue,
        &SizeFPropertyManagerPrivate::Data::setMinimumValue,
        minVal, &SizeFPropertyManagerPrivate::setRange);
}

void SizeFPropertyManager::setMaximum(Property* property, const QSizeF& maxVal)
{
    setBorderValue<const QSizeF&, SizeFPropertyManagerPrivate, SizeFPropertyManager, QSizeF, SizeFPropertyManagerPrivate::Data>(this, impl_.data(),
        &SizeFPropertyManager::propertyChanged,
        &SizeFPropertyManager::valueChanged,
        &SizeFPropertyManager::rangeChanged,
        property,
        &SizeFPropertyManagerPrivate::Data::maximumValue,
        &SizeFPropertyManagerPrivate::Data::setMaximumValue,
        maxVal, &SizeFPropertyManagerPrivate::setRange);
}

void SizeFPropertyManager::setRange(Property* property, const QSizeF& minVal, const QSizeF& maxVal)
{
    setBorderValues<const QSizeF&, SizeFPropertyManagerPrivate, SizeFPropertyManager, QSizeF>(this, impl_.data(),
        &SizeFPropertyManager::propertyChanged,
        &SizeFPropertyManager::valueChanged,
        &SizeFPropertyManager::rangeChanged,
        property, minVal, maxVal, &SizeFPropertyManagerPrivate::setRange);
}

void SizeFPropertyManager::initializeProperty(Property* property)
{
    impl_->values_[property] = {};

    auto wProp = impl_->doublePropertyManager_->addProperty();
    wProp->setPropertyName(tr("Width"));
    impl_->doublePropertyManager_->setDecimals(wProp, decimals(property));
    impl_->doublePropertyManager_->setValue(wProp, 0);
    impl_->doublePropertyManager_->setMinimum(wProp, 0);
    impl_->propertyToW_[property] = wProp;
    impl_->wToProperty_[wProp] = property;
    property->addSubProperty(wProp);

    auto hProp = impl_->doublePropertyManager_->addProperty();
    hProp->setPropertyName(tr("Height"));
    impl_->doublePropertyManager_->setDecimals(hProp, decimals(property));
    impl_->doublePropertyManager_->setValue(hProp, 0);
    impl_->doublePropertyManager_->setMinimum(hProp, 0);
    impl_->propertyToH_[property] = hProp;
    impl_->hToProperty_[hProp] = property;
    property->addSubProperty(hProp);
}

void SizeFPropertyManager::uninitializeProperty(Property* property)
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
