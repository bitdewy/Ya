/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "variantpropertymanager_p.h"
#include "property.h"
#include "variantproperty.h"
#include "propertymanager/variantpropertymanager.h"
#include "propertymanager/intpropertymanager.h"
#include "propertymanager/enumpropertymanager.h"
#include "propertymanager/boolpropertymanager.h"
#include "propertymanager/doublepropertymanager.h"
#include <QtCore/QVariant>
#include <QtGui/QFont>
#include <QtGui/QCursor>
#include <QtGui/QKeySequence>
#include <QtWidgets/QSizePolicy>

Property* wrappedProperty(Property* property)
{
    return propertyToWrappedProperty()->value(property, 0);
}

VariantPropertyManagerPrivate::VariantPropertyManagerPrivate() :
    constraintAttribute_(QLatin1String("constraint")),
    singleStepAttribute_(QLatin1String("singleStep")),
    decimalsAttribute_(QLatin1String("decimals")),
    enumIconsAttribute_(QLatin1String("enumIcons")),
    enumNamesAttribute_(QLatin1String("enumNames")),
    flagNamesAttribute_(QLatin1String("flagNames")),
    maximumAttribute_(QLatin1String("maximum")),
    minimumAttribute_(QLatin1String("minimum")),
    regExpAttribute_(QLatin1String("regExp"))
{
}

int VariantPropertyManagerPrivate::internalPropertyToType(Property* property) const
{
    auto type = 0;
    auto internPropertyManager = property->propertyManager();
    if (qobject_cast<IntPropertyManager*>(internPropertyManager))
    {
        type = QVariant::Int;
    }
    else if (qobject_cast<EnumPropertyManager*>(internPropertyManager))
    {
        type = VariantPropertyManager::enumTypeId();
    }
    else if (qobject_cast<BoolPropertyManager*>(internPropertyManager))
    {
        type = QVariant::Bool;
    }
    else if (qobject_cast<DoublePropertyManager*>(internPropertyManager))
    {
        type = QVariant::Double;
    }
    return type;
}

VariantProperty* VariantPropertyManagerPrivate::createSubProperty(VariantProperty* parent,
    VariantProperty* after, Property* internal)
{
    int type = internalPropertyToType(internal);
    if (!type)
    {
        return 0;
    }
    auto wasCreatingSubProperties = creatingSubProperties_;
    creatingSubProperties_ = true;

    auto varChild = variantPropertyManagerPtr_->addProperty(type, internal->propertyName());

    creatingSubProperties_ = wasCreatingSubProperties;

    varChild->setPropertyName(internal->propertyName());
    varChild->setToolTip(internal->toolTip());
    varChild->setStatusTip(internal->statusTip());
    varChild->setWhatsThis(internal->whatsThis());

    parent->insertSubProperty(varChild, after);

    internalToProperty_[internal] = varChild;
    propertyToWrappedProperty()->insert(varChild, internal);
    return varChild;
}

void VariantPropertyManagerPrivate::removeSubProperty(VariantProperty* property)
{
    auto internChild = wrappedProperty(property);
    auto wasDestroyingSubProperties = destroyingSubProperties_;
    destroyingSubProperties_ = true;
    delete property;
    destroyingSubProperties_ = wasDestroyingSubProperties;
    internalToProperty_.remove(internChild);
    propertyToWrappedProperty()->remove(property);
}

void VariantPropertyManagerPrivate::slotPropertyInserted(Property* property,
    Property* parent, Property* after)
{
    if (creatingProperty_)
    {
        return;
    }
    auto varParent = internalToProperty_.value(parent, 0);
    if (!varParent)
    {
        return;
    }
    VariantProperty* varAfter = nullptr;
    if (after)
    {
        varAfter = internalToProperty_.value(after, 0);
        if (!varAfter)
        {
            return;
        }
    }

    createSubProperty(varParent, varAfter, property);
}

void VariantPropertyManagerPrivate::slotPropertyRemoved(Property* property, Property* parent)
{
    Q_UNUSED(parent)

    auto varProperty = internalToProperty_.value(property, 0);
    if (!varProperty)
    {
        return;
    }
    removeSubProperty(varProperty);
}

void VariantPropertyManagerPrivate::valueChanged(Property* property, const QVariant& val)
{
    auto varProp = internalToProperty_.value(property, 0);
    if (!varProp)
    {
        return;
    }
    emit variantPropertyManagerPtr_->valueChanged(varProp, val);
    emit variantPropertyManagerPtr_->propertyChanged(varProp);
}

void VariantPropertyManagerPrivate::slotValueChanged(Property* property, int val)
{
    valueChanged(property, QVariant(val));
}

void VariantPropertyManagerPrivate::slotRangeChanged(Property* property, int min, int max)
{
    if (auto varProp = internalToProperty_.value(property, 0))
    {
        emit variantPropertyManagerPtr_->attributeChanged(varProp, minimumAttribute_, QVariant(min));
        emit variantPropertyManagerPtr_->attributeChanged(varProp, maximumAttribute_, QVariant(max));
    }
}

void VariantPropertyManagerPrivate::slotSingleStepChanged(Property* property, int step)
{
    if (auto varProp = internalToProperty_.value(property, 0))
    {
        emit variantPropertyManagerPtr_->attributeChanged(varProp, singleStepAttribute_, QVariant(step));
    }
}

void VariantPropertyManagerPrivate::slotValueChanged(Property* property, double val)
{
    valueChanged(property, QVariant(val));
}

void VariantPropertyManagerPrivate::slotRangeChanged(Property* property, double min, double max)
{
    if (auto varProp = internalToProperty_.value(property, 0))
    {
        emit variantPropertyManagerPtr_->attributeChanged(varProp, minimumAttribute_, QVariant(min));
        emit variantPropertyManagerPtr_->attributeChanged(varProp, maximumAttribute_, QVariant(max));
    }
}

void VariantPropertyManagerPrivate::slotSingleStepChanged(Property* property, double step)
{
    if (VariantProperty *varProp = internalToProperty_.value(property, 0))
    {
        emit variantPropertyManagerPtr_->attributeChanged(varProp, singleStepAttribute_, QVariant(step));
    }
}

void VariantPropertyManagerPrivate::slotDecimalsChanged(Property* property, int prec)
{
    if (VariantProperty *varProp = internalToProperty_.value(property, 0))
    {
        emit variantPropertyManagerPtr_->attributeChanged(varProp, decimalsAttribute_, QVariant(prec));
    }
}

void VariantPropertyManagerPrivate::slotValueChanged(Property* property, bool val)
{
    valueChanged(property, QVariant(val));
}

void VariantPropertyManagerPrivate::slotValueChanged(Property* property, const QString& val)
{
    valueChanged(property, QVariant(val));
}

void VariantPropertyManagerPrivate::slotRegExpChanged(Property* property, const QRegExp& regExp)
{
    if (auto varProp = internalToProperty_.value(property, 0))
    {
        emit variantPropertyManagerPtr_->attributeChanged(varProp, regExpAttribute_, QVariant(regExp));
    }
}

void VariantPropertyManagerPrivate::slotValueChanged(Property* property, const QDate& val)
{
    valueChanged(property, QVariant(val));
}

void VariantPropertyManagerPrivate::slotRangeChanged(Property* property, const QDate& min, const QDate& max)
{
    if (auto varProp = internalToProperty_.value(property, 0))
    {
        emit variantPropertyManagerPtr_->attributeChanged(varProp, minimumAttribute_, QVariant(min));
        emit variantPropertyManagerPtr_->attributeChanged(varProp, maximumAttribute_, QVariant(max));
    }
}

void VariantPropertyManagerPrivate::slotValueChanged(Property* property, const QTime& val)
{
    valueChanged(property, QVariant(val));
}

void VariantPropertyManagerPrivate::slotValueChanged(Property* property, const QDateTime& val)
{
    valueChanged(property, QVariant(val));
}

void VariantPropertyManagerPrivate::slotValueChanged(Property* property, const QKeySequence& val)
{
    QVariant v;
    v.setValue(val);
    valueChanged(property, v);
}

void VariantPropertyManagerPrivate::slotValueChanged(Property* property, const QChar& val)
{
    valueChanged(property, QVariant(val));
}

void VariantPropertyManagerPrivate::slotValueChanged(Property* property, const QLocale& val)
{
    valueChanged(property, QVariant(val));
}

void VariantPropertyManagerPrivate::slotValueChanged(Property* property, const QPoint& val)
{
    valueChanged(property, QVariant(val));
}

void VariantPropertyManagerPrivate::slotValueChanged(Property* property, const QPointF& val)
{
    valueChanged(property, QVariant(val));
}

void VariantPropertyManagerPrivate::slotValueChanged(Property* property, const QSize& val)
{
    valueChanged(property, QVariant(val));
}

void VariantPropertyManagerPrivate::slotRangeChanged(Property* property, const QSize& min, const QSize& max)
{
    if (auto varProp = internalToProperty_.value(property, 0))
    {
        emit variantPropertyManagerPtr_->attributeChanged(varProp, minimumAttribute_, QVariant(min));
        emit variantPropertyManagerPtr_->attributeChanged(varProp, maximumAttribute_, QVariant(max));
    }
}

void VariantPropertyManagerPrivate::slotValueChanged(Property* property, const QSizeF& val)
{
    valueChanged(property, QVariant(val));
}

void VariantPropertyManagerPrivate::slotRangeChanged(Property* property, const QSizeF& min, const QSizeF& max)
{
    if (VariantProperty *varProp = internalToProperty_.value(property, 0))
    {
        emit variantPropertyManagerPtr_->attributeChanged(varProp, minimumAttribute_, QVariant(min));
        emit variantPropertyManagerPtr_->attributeChanged(varProp, maximumAttribute_, QVariant(max));
    }
}

void VariantPropertyManagerPrivate::slotValueChanged(Property* property, const QRect& val)
{
    valueChanged(property, QVariant(val));
}

void VariantPropertyManagerPrivate::slotConstraintChanged(Property* property, const QRect& constraint)
{
    if (auto varProp = internalToProperty_.value(property, 0))
    {
        emit variantPropertyManagerPtr_->attributeChanged(varProp, constraintAttribute_, QVariant(constraint));
    }
}

void VariantPropertyManagerPrivate::slotValueChanged(Property* property, const QRectF& val)
{
    valueChanged(property, QVariant(val));
}

void VariantPropertyManagerPrivate::slotConstraintChanged(Property* property, const QRectF& constraint)
{
    if (auto varProp = internalToProperty_.value(property, 0))
    {
        emit variantPropertyManagerPtr_->attributeChanged(varProp, constraintAttribute_, QVariant(constraint));
    }
}

void VariantPropertyManagerPrivate::slotValueChanged(Property* property, const QColor& val)
{
    valueChanged(property, QVariant(val));
}

void VariantPropertyManagerPrivate::slotEnumNamesChanged(Property* property, const QStringList& enumNames)
{
    if (auto varProp = internalToProperty_.value(property, 0))
    {
        emit variantPropertyManagerPtr_->attributeChanged(varProp, enumNamesAttribute_, QVariant(enumNames));
    }
}

void VariantPropertyManagerPrivate::slotEnumIconsChanged(Property* property, const QMap<int, QIcon>& enumIcons)
{
    if (auto varProp = internalToProperty_.value(property, 0))
    {
        QVariant v;
        v.setValue(enumIcons);
        emit variantPropertyManagerPtr_->attributeChanged(varProp, enumIconsAttribute_, v);
    }
}

void VariantPropertyManagerPrivate::slotValueChanged(Property* property, const QSizePolicy& val)
{
    valueChanged(property, QVariant(val));
}

void VariantPropertyManagerPrivate::slotValueChanged(Property* property, const QFont& val)
{
    valueChanged(property, QVariant(val));
}

void VariantPropertyManagerPrivate::slotValueChanged(Property* property, const QCursor& val)
{
    valueChanged(property, QVariant(val));
}

void VariantPropertyManagerPrivate::slotFlagNamesChanged(Property* property, const QStringList& flagNames)
{
    if (auto varProp = internalToProperty_.value(property, 0))
    {
        emit variantPropertyManagerPtr_->attributeChanged(varProp, flagNamesAttribute_, QVariant(flagNames));
    }
}
