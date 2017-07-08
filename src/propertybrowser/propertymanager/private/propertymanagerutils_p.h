/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <QtCore/QSize>

class Property;

template <class PrivateData, class Value>
static void setSimpleMinimumData(PrivateData* data, const Value& minVal)
{
    data->minVal = minVal;
    if (data->maxVal < data->minVal)
    {
        data->maxVal = data->minVal;
    }

    if (data->val < data->minVal)
    {
        data->val = data->minVal;
    }
}

template <class PrivateData, class Value>
static void setSimpleMaximumData(PrivateData* data, const Value& maxVal)
{
    data->maxVal = maxVal;
    if (data->minVal > data->maxVal)
    {
        data->minVal = data->maxVal;
    }

    if (data->val > data->maxVal)
    {
        data->val = data->maxVal;
    }
}

template <class PrivateData, class Value>
static void setSizeMinimumData(PrivateData* data, const Value& newMinVal)
{
    data->minVal = newMinVal;
    if (data->maxVal.width() < data->minVal.width())
    {
        data->maxVal.setWidth(data->minVal.width());
    }
    if (data->maxVal.height() < data->minVal.height())
    {
        data->maxVal.setHeight(data->minVal.height());
    }

    if (data->val.width() < data->minVal.width())
    {
        data->val.setWidth(data->minVal.width());
    }
    if (data->val.height() < data->minVal.height())
    {
        data->val.setHeight(data->minVal.height());
    }
}

template <class PrivateData, class Value>
static void setSizeMaximumData(PrivateData* data, const Value& newMaxVal)
{
    data->maxVal = newMaxVal;
    if (data->minVal.width() > data->maxVal.width())
    {
        data->minVal.setWidth(data->maxVal.width());
    }
    if (data->minVal.height() > data->maxVal.height())
    {
        data->minVal.setHeight(data->maxVal.height());
    }

    if (data->val.width() > data->maxVal.width())
    {
        data->val.setWidth(data->maxVal.width());
    }
    if (data->val.height() > data->maxVal.height())
    {
        data->val.setHeight(data->maxVal.height());
    }
}

template <class SizeValue>
static SizeValue qBoundSize(const SizeValue& minVal, const SizeValue& val, const SizeValue& maxVal)
{
    auto croppedVal = val;
    if (minVal.width() > val.width())
    {
        croppedVal.setWidth(minVal.width());
    }
    else if (maxVal.width() < val.width())
    {
        croppedVal.setWidth(maxVal.width());
    }

    if (minVal.height() > val.height())
    {
        croppedVal.setHeight(minVal.height());
    }
    else if (maxVal.height() < val.height())
    {
        croppedVal.setHeight(maxVal.height());
    }
    return croppedVal;
}

static QSize qBound(QSize minVal, QSize val, QSize maxVal)
{
    return qBoundSize(minVal, val, maxVal);
}

static QSizeF qBound(QSizeF minVal, QSizeF val, QSizeF maxVal)
{
    return qBoundSize(minVal, val, maxVal);
}

template <class Value>
static void orderBorders(Value& minVal, Value& maxVal)
{
    if (minVal > maxVal)
    {
        qSwap(minVal, maxVal);
    }
}

template <class Value>
static void orderSizeBorders(Value& minVal, Value& maxVal)
{
    Value fromSize = minVal;
    Value toSize = maxVal;
    if (fromSize.width() > toSize.width())
    {
        fromSize.setWidth(maxVal.width());
        toSize.setWidth(minVal.width());
    }
    if (fromSize.height() > toSize.height())
    {
        fromSize.setHeight(maxVal.height());
        toSize.setHeight(minVal.height());
    }
    minVal = fromSize;
    maxVal = toSize;
}

static void orderBorders(QSize& minVal, QSize& maxVal)
{
    orderSizeBorders(minVal, maxVal);
}

static void orderBorders(QSizeF& minVal, QSizeF& maxVal)
{
    orderSizeBorders(minVal, maxVal);
}

template <class Value, class PrivateData>
static Value getData(const QMap<const Property*, PrivateData>& propertyMap,
    Value PrivateData::* data, const Property* property, const Value& defaultValue = Value())
{
    const auto it = propertyMap.constFind(property);
    if (it == propertyMap.constEnd())
    {
        return defaultValue;
    }
    return it.value().*data;
}

template <class Value, class PrivateData>
static Value getValue(const QMap<const Property*, PrivateData>& propertyMap,
    const Property* property, const Value& defaultValue = Value())
{
    return getData<Value>(propertyMap, &PrivateData::val, property, defaultValue);
}

template <class Value, class PrivateData>
static Value getMinimum(const QMap<const Property*, PrivateData>& propertyMap,
    const Property* property, const Value& defaultValue = Value())
{
    return getData<Value>(propertyMap, &PrivateData::minVal, property, defaultValue);
}

template <class Value, class PrivateData>
static Value getMaximum(const QMap<const Property*, PrivateData>& propertyMap,
    const Property* property, const Value& defaultValue = Value())
{
    return getData<Value>(propertyMap, &PrivateData::maxVal, property, defaultValue);
}

template <class ValueChangeParameter, class Value, class PropertyManager>
static void setSimpleValue(QMap<const Property*, Value> &propertyMap,
    PropertyManager* manager,
    void (PropertyManager::*propertyChangedSignal)(Property*),
    void (PropertyManager::*valueChangedSignal)(Property*, ValueChangeParameter),
    Property* property, const Value &val)
{
    const auto it = propertyMap.find(property);
    if (it == propertyMap.end())
    {
        return;
    }
    if (it.value() == val)
    {
        return;
    }
    it.value() = val;

    emit (manager->*propertyChangedSignal)(property);
    emit (manager->*valueChangedSignal)(property, val);
}

template <class ValueChangeParameter, class PropertyManagerPrivate, class PropertyManager, class Value>
static void setValueInRange(PropertyManager* manager, PropertyManagerPrivate* managerPrivate,
    void (PropertyManager::*propertyChangedSignal)(Property*),
    void (PropertyManager::*valueChangedSignal)(Property*, ValueChangeParameter),
    Property* property, const Value& val,
    void (PropertyManagerPrivate::*setSubPropertyValue)(Property*, ValueChangeParameter))
{
    const auto it = managerPrivate->values_.find(property);
    if (it == managerPrivate->values_.end())
    {
        return;
    }
    auto& data = it.value();

    if (data.val == val)
    {
        return;
    }
    const Value oldVal = data.val;

    data.val = qBound(data.minVal, val, data.maxVal);

    if (data.val == oldVal)
    {
        return;
    }
    if (setSubPropertyValue)
    {
        (managerPrivate->*setSubPropertyValue)(property, data.val);
    }
    emit (manager->*propertyChangedSignal)(property);
    emit (manager->*valueChangedSignal)(property, data.val);
}

template <class ValueChangeParameter, class PropertyManagerPrivate, class PropertyManager, class Value>
static void setBorderValues(PropertyManager* manager, PropertyManagerPrivate* managerPrivate,
    void (PropertyManager::*propertyChangedSignal)(Property*),
    void (PropertyManager::*valueChangedSignal)(Property*, ValueChangeParameter),
    void (PropertyManager::*rangeChangedSignal)(Property*, ValueChangeParameter, ValueChangeParameter),
    Property* property, const Value& minVal, const Value& maxVal,
    void (PropertyManagerPrivate::*setSubPropertyRange)(Property*, ValueChangeParameter, ValueChangeParameter, ValueChangeParameter))
{
    const auto it = managerPrivate->values_.find(property);
    if (it == managerPrivate->values_.end())
    {
        return;
    }
    Value fromVal = minVal;
    Value toVal = maxVal;
    orderBorders(fromVal, toVal);

    auto& data = it.value();

    if (data.minVal == fromVal && data.maxVal == toVal)
    {
        return;
    }
    const Value oldVal = data.val;

    data.setMinimumValue(fromVal);
    data.setMaximumValue(toVal);

    emit (manager->*rangeChangedSignal)(property, data.minVal, data.maxVal);

    if (setSubPropertyRange)
    {
        (managerPrivate->*setSubPropertyRange)(property, data.minVal, data.maxVal, data.val);
    }
    if (data.val == oldVal)
    {
        return;
    }
    emit (manager->*propertyChangedSignal)(property);
    emit (manager->*valueChangedSignal)(property, data.val);
}

template <class ValueChangeParameter, class PropertyManagerPrivate, class PropertyManager, class Value, class PrivateData>
static void setBorderValue(PropertyManager* manager, PropertyManagerPrivate* managerPrivate,
    void (PropertyManager::*propertyChangedSignal)(Property*),
    void (PropertyManager::*valueChangedSignal)(Property*, ValueChangeParameter),
    void (PropertyManager::*rangeChangedSignal)(Property*, ValueChangeParameter, ValueChangeParameter),
    Property* property,
    Value (PrivateData::*getRangeVal)() const,
    void (PrivateData::*setRangeVal)(ValueChangeParameter), const Value& borderVal,
    void (PropertyManagerPrivate::*setSubPropertyRange)(Property*, ValueChangeParameter, ValueChangeParameter, ValueChangeParameter))
{
    const auto it = managerPrivate->values_.find(property);
    if (it == managerPrivate->values_.end())
    {
        return;
    }
    auto& data = it.value();

    if ((data.*getRangeVal)() == borderVal)
    {
        return;
    }
    const Value oldVal = data.val;

    (data.*setRangeVal)(borderVal);

    emit (manager->*rangeChangedSignal)(property, data.minVal, data.maxVal);

    if (setSubPropertyRange)
    {
        (managerPrivate->*setSubPropertyRange)(property, data.minVal, data.maxVal, data.val);
    }
    if (data.val == oldVal)
    {
        return;
    }
    emit (manager->*propertyChangedSignal)(property);
    emit (manager->*valueChangedSignal)(property, data.val);
}

template <class ValueChangeParameter, class PropertyManagerPrivate, class PropertyManager, class Value, class PrivateData>
static void setMinimumValue(PropertyManager* manager, PropertyManagerPrivate* managerPrivate,
    void (PropertyManager::*propertyChangedSignal)(Property*),
    void (PropertyManager::*valueChangedSignal)(Property*, ValueChangeParameter),
    void (PropertyManager::*rangeChangedSignal)(Property*, ValueChangeParameter, ValueChangeParameter),
    Property* property, const Value& minVal)
{
    void (PropertyManagerPrivate::*setSubPropertyRange)(Property *, ValueChangeParameter, ValueChangeParameter, ValueChangeParameter) = 0;
    setBorderValue<ValueChangeParameter, PropertyManagerPrivate, PropertyManager, Value, PrivateData>(manager, managerPrivate,
        propertyChangedSignal, valueChangedSignal, rangeChangedSignal, property,
        &PropertyManagerPrivate::Data::minimumValue, &PropertyManagerPrivate::Data::setMinimumValue, minVal, setSubPropertyRange);
}

template <class ValueChangeParameter, class PropertyManagerPrivate, class PropertyManager, class Value, class PrivateData>
static void setMaximumValue(PropertyManager* manager, PropertyManagerPrivate* managerPrivate,
    void (PropertyManager::*propertyChangedSignal)(Property*),
    void (PropertyManager::*valueChangedSignal)(Property*, ValueChangeParameter),
    void (PropertyManager::*rangeChangedSignal)(Property*, ValueChangeParameter, ValueChangeParameter),
    Property *property, const Value& maxVal)
{
    void (PropertyManagerPrivate::*setSubPropertyRange)(Property*, ValueChangeParameter, ValueChangeParameter, ValueChangeParameter) = 0;
    setBorderValue<ValueChangeParameter, PropertyManagerPrivate, PropertyManager, Value, PrivateData>(manager, managerPrivate,
        propertyChangedSignal, valueChangedSignal, rangeChangedSignal, property,
        &PropertyManagerPrivate::Data::maximumValue, &PropertyManagerPrivate::Data::setMaximumValue, maxVal, setSubPropertyRange);
}
