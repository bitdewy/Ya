/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "variantproperty.h"
#include "propertymanager/variantpropertymanager.h"
#include "private/variantproperty_p.h"

VariantProperty::VariantProperty(VariantPropertyManager* manager) :
    Property(manager),
    impl_(new VariantPropertyPrivate(manager))
{
}

VariantProperty::~VariantProperty()
{
}

QVariant VariantProperty::value() const
{
    return impl_->manager->value(this);
}

QVariant VariantProperty::attributeValue(const QString& attribute) const
{
    return impl_->manager->attributeValue(this, attribute);
}

int VariantProperty::valueType() const
{
    return impl_->manager->valueType(this);
}

int VariantProperty::propertyType() const
{
    return impl_->manager->propertyType(this);
}

void VariantProperty::setValue(const QVariant& value)
{
    impl_->manager->setValue(this, value);
}

void VariantProperty::setAttribute(const QString& attribute, const QVariant& value)
{
    impl_->manager->setAttribute(this, attribute, value);
}
