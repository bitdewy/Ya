/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "sizepolicypropertymanager.h"
#include "private/sizepolicypropertymanager_p.h"
#include "propertymanager/intpropertymanager.h"
#include "propertymanager/enumpropertymanager.h"
#include "propertymanager/metaenumprovider.h"
#include "property.h"
#include <QtWidgets/QSizePolicy>

SizePolicyPropertyManager::SizePolicyPropertyManager(QObject* parent)
    : AbstractPropertyManager(parent),
      impl_(new SizePolicyPropertyManagerPrivate)
{
    impl_->sizePolicyPropertyManager_ = this;

    impl_->intPropertyManager_ = new IntPropertyManager(this);
    connect(impl_->intPropertyManager_, &IntPropertyManager::valueChanged,
        this, [this](Property* property, int value) { impl_->slotIntChanged(property, value); });
    impl_->enumPropertyManager_ = new EnumPropertyManager(this);
    connect(impl_->enumPropertyManager_, &EnumPropertyManager::valueChanged,
        this, [this](Property* property, int value) { impl_->slotEnumChanged(property, value); });

    connect(impl_->intPropertyManager_, &IntPropertyManager::propertyDestroyed,
        this, [this](Property* property) { impl_->slotPropertyDestroyed(property); });
    connect(impl_->enumPropertyManager_, &EnumPropertyManager::propertyDestroyed,
        this, [this](Property* property) { impl_->slotPropertyDestroyed(property); });
}

SizePolicyPropertyManager::~SizePolicyPropertyManager()
{
    clear();
}

IntPropertyManager* SizePolicyPropertyManager::subIntPropertyManager() const
{
    return impl_->intPropertyManager_;
}

EnumPropertyManager* SizePolicyPropertyManager::subEnumPropertyManager() const
{
    return impl_->enumPropertyManager_;
}

QSizePolicy SizePolicyPropertyManager::value(const Property* property) const
{
    return impl_->values_.value(property, QSizePolicy());
}

QString SizePolicyPropertyManager::valueText(const Property* property) const
{
    const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return {};
    }
    const auto sp = it.value();
    const auto mep = metaEnumProvider();
    const auto hIndex = mep->sizePolicyToIndex(sp.horizontalPolicy());
    const auto vIndex = mep->sizePolicyToIndex(sp.verticalPolicy());

    const auto hPolicy = hIndex != -1 ? mep->policyEnumNames().at(hIndex) : tr("<Invalid>");
    const auto vPolicy = vIndex != -1 ? mep->policyEnumNames().at(vIndex) : tr("<Invalid>");
    const auto str = tr("[%1, %2, %3, %4]").arg(hPolicy, vPolicy).arg(sp.horizontalStretch()).arg(sp.verticalStretch());
    return str;
}

void SizePolicyPropertyManager::setValue(Property* property, const QSizePolicy& val)
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

    impl_->enumPropertyManager_->setValue(impl_->propertyToHPolicy_[property],
                metaEnumProvider()->sizePolicyToIndex(val.horizontalPolicy()));
    impl_->enumPropertyManager_->setValue(impl_->propertyToVPolicy_[property],
                metaEnumProvider()->sizePolicyToIndex(val.verticalPolicy()));
    impl_->intPropertyManager_->setValue(impl_->propertyToHStretch_[property],
                val.horizontalStretch());
    impl_->intPropertyManager_->setValue(impl_->propertyToVStretch_[property],
                val.verticalStretch());

    emit propertyChanged(property);
    emit valueChanged(property, val);
}

void SizePolicyPropertyManager::initializeProperty(Property* property)
{
    QSizePolicy val;
    impl_->values_[property] = val;

    auto hPolicyProp = impl_->enumPropertyManager_->addProperty();
    hPolicyProp->setPropertyName(tr("Horizontal Policy"));
    impl_->enumPropertyManager_->setEnumNames(hPolicyProp, metaEnumProvider()->policyEnumNames());
    impl_->enumPropertyManager_->setValue(hPolicyProp,
        metaEnumProvider()->sizePolicyToIndex(val.horizontalPolicy()));
    impl_->propertyToHPolicy_[property] = hPolicyProp;
    impl_->hPolicyToProperty_[hPolicyProp] = property;
    property->addSubProperty(hPolicyProp);

    auto vPolicyProp = impl_->enumPropertyManager_->addProperty();
    vPolicyProp->setPropertyName(tr("Vertical Policy"));
    impl_->enumPropertyManager_->setEnumNames(vPolicyProp, metaEnumProvider()->policyEnumNames());
    impl_->enumPropertyManager_->setValue(vPolicyProp,
        metaEnumProvider()->sizePolicyToIndex(val.verticalPolicy()));
    impl_->propertyToVPolicy_[property] = vPolicyProp;
    impl_->vPolicyToProperty_[vPolicyProp] = property;
    property->addSubProperty(vPolicyProp);

    auto hStretchProp = impl_->intPropertyManager_->addProperty();
    hStretchProp->setPropertyName(tr("Horizontal Stretch"));
    impl_->intPropertyManager_->setValue(hStretchProp, val.horizontalStretch());
    impl_->intPropertyManager_->setRange(hStretchProp, 0, 0xff);
    impl_->propertyToHStretch_[property] = hStretchProp;
    impl_->hStretchToProperty_[hStretchProp] = property;
    property->addSubProperty(hStretchProp);

    auto vStretchProp = impl_->intPropertyManager_->addProperty();
    vStretchProp->setPropertyName(tr("Vertical Stretch"));
    impl_->intPropertyManager_->setValue(vStretchProp, val.verticalStretch());
    impl_->intPropertyManager_->setRange(vStretchProp, 0, 0xff);
    impl_->propertyToVStretch_[property] = vStretchProp;
    impl_->vStretchToProperty_[vStretchProp] = property;
    property->addSubProperty(vStretchProp);

}

void SizePolicyPropertyManager::uninitializeProperty(Property* property)
{
    auto hPolicyProp = impl_->propertyToHPolicy_[property];
    if (hPolicyProp)
    {
        impl_->hPolicyToProperty_.remove(hPolicyProp);
        delete hPolicyProp;
    }
    impl_->propertyToHPolicy_.remove(property);

    auto vPolicyProp = impl_->propertyToVPolicy_[property];
    if (vPolicyProp)
    {
        impl_->vPolicyToProperty_.remove(vPolicyProp);
        delete vPolicyProp;
    }
    impl_->propertyToVPolicy_.remove(property);

    auto hStretchProp = impl_->propertyToHStretch_[property];
    if (hStretchProp)
    {
        impl_->hStretchToProperty_.remove(hStretchProp);
        delete hStretchProp;
    }
    impl_->propertyToHStretch_.remove(property);

    auto vStretchProp = impl_->propertyToVStretch_[property];
    if (vStretchProp)
    {
        impl_->vStretchToProperty_.remove(vStretchProp);
        delete vStretchProp;
    }
    impl_->propertyToVStretch_.remove(property);

    impl_->values_.remove(property);
}
