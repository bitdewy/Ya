/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "variantpropertymanager.h"
#include "private/variantpropertymanager_p.h"
#include "variantproperty.h"
#include "propertymanager/intpropertymanager.h"
#include "propertymanager/fontpropertymanager.h"
#include "propertymanager/enumpropertymanager.h"
#include "propertymanager/boolpropertymanager.h"
#include "propertymanager/charpropertymanager.h"
#include "propertymanager/doublepropertymanager.h"
#include "propertymanager/stringpropertymanager.h"
#include "propertymanager/grouppropertymanager.h"
#include "propertymanager/datepropertymanager.h"
#include "propertymanager/timepropertymanager.h"
#include "propertymanager/datetimepropertymanager.h"
#include "propertymanager/keysequencepropertymanager.h"
#include "propertymanager/cursorpropertymanager.h"
#include "propertymanager/colorpropertymanager.h"
#include "propertymanager/localepropertymanager.h"
#include "propertymanager/pointpropertymanager.h"
#include "propertymanager/sizepolicypropertymanager.h"
#include "propertymanager/flagpropertymanager.h"
#include "propertymanager/sizepropertymanager.h"
#include "propertymanager/rectpropertymanager.h"
#include "propertymanager/pointfpropertymanager.h"
#include "propertymanager/sizefpropertymanager.h"
#include "propertymanager/rectfpropertymanager.h"
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QLocale>
#include <QtGui/QKeySequence>
#include <QtGui/QFont>
#include <QtGui/QCursor>
#include <QtWidgets/QSizePolicy>

class EnumPropertyType
{
};

class FlagPropertyType
{
};


class GroupPropertyType
{
};

Q_DECLARE_METATYPE(EnumPropertyType)
Q_DECLARE_METATYPE(FlagPropertyType)
Q_DECLARE_METATYPE(GroupPropertyType)

int VariantPropertyManager::enumTypeId()
{
    return qMetaTypeId<EnumPropertyType>();
}

int VariantPropertyManager::flagTypeId()
{
    return qMetaTypeId<FlagPropertyType>();
}

int VariantPropertyManager::groupTypeId()
{
    return qMetaTypeId<GroupPropertyType>();
}

int VariantPropertyManager::iconMapTypeId()
{
    return qMetaTypeId<IconMap>();
}

VariantPropertyManager::VariantPropertyManager(QObject* parent) :
    AbstractPropertyManager(parent),
    impl_(new VariantPropertyManagerPrivate)
{
    impl_->variantPropertyManagerPtr_ = this;

    impl_->creatingProperty_ = false;
    impl_->creatingSubProperties_ = false;
    impl_->destroyingSubProperties_ = false;
    impl_->propertyType_ = 0;

    auto intPropertyManager = new IntPropertyManager(this);
    impl_->typeToPropertyManager_[QVariant::Int] = intPropertyManager;
    impl_->typeToAttributeToAttributeType_[QVariant::Int][impl_->minimumAttribute_] = QVariant::Int;
    impl_->typeToAttributeToAttributeType_[QVariant::Int][impl_->maximumAttribute_] = QVariant::Int;
    impl_->typeToAttributeToAttributeType_[QVariant::Int][impl_->singleStepAttribute_] = QVariant::Int;
    impl_->typeToValueType_[QVariant::Int] = QVariant::Int;
    connect(intPropertyManager, &IntPropertyManager::valueChanged,
        this, [this](Property* property, int value) { impl_->slotValueChanged(property, value); });
    connect(intPropertyManager, &IntPropertyManager::rangeChanged,
        this, [this](Property* property, int min, int max) { impl_->slotRangeChanged(property, min, max); });
    connect(intPropertyManager, &IntPropertyManager::singleStepChanged,
        this, [this](Property* property, int step) { impl_->slotSingleStepChanged(property, step); });

    auto doublePropertyManager = new DoublePropertyManager(this);
    impl_->typeToPropertyManager_[QVariant::Double] = doublePropertyManager;
    impl_->typeToAttributeToAttributeType_[QVariant::Double][impl_->minimumAttribute_] = QVariant::Double;
    impl_->typeToAttributeToAttributeType_[QVariant::Double][impl_->maximumAttribute_] = QVariant::Double;
    impl_->typeToAttributeToAttributeType_[QVariant::Double][impl_->singleStepAttribute_] = QVariant::Double;
    impl_->typeToAttributeToAttributeType_[QVariant::Double][impl_->decimalsAttribute_] = QVariant::Int;
    impl_->typeToValueType_[QVariant::Double] = QVariant::Double;
    connect(doublePropertyManager, &DoublePropertyManager::valueChanged,
        this, [this](Property* property, double value) { impl_->slotValueChanged(property, value); });
    connect(doublePropertyManager, &DoublePropertyManager::rangeChanged,
        this, [this](Property* property, double min, double max) { impl_->slotRangeChanged(property, min, max); });
    connect(doublePropertyManager, &DoublePropertyManager::singleStepChanged,
        this, [this](Property* property, double step) { impl_->slotSingleStepChanged(property, step); });
    connect(doublePropertyManager, &DoublePropertyManager::decimalsChanged,
        this, [this](Property* property, int value) { impl_->slotDecimalsChanged(property, value); });

    auto boolPropertyManager = new BoolPropertyManager(this);
    impl_->typeToPropertyManager_[QVariant::Bool] = boolPropertyManager;
    impl_->typeToValueType_[QVariant::Bool] = QVariant::Bool;
    connect(boolPropertyManager, &BoolPropertyManager::valueChanged,
        this, [this](Property* property, bool value) { impl_->slotValueChanged(property, value); });

    auto stringPropertyManager = new StringPropertyManager(this);
    impl_->typeToPropertyManager_[QVariant::String] = stringPropertyManager;
    impl_->typeToValueType_[QVariant::String] = QVariant::String;
    impl_->typeToAttributeToAttributeType_[QVariant::String][impl_->regExpAttribute_] = QVariant::RegExp;
    connect(stringPropertyManager, &StringPropertyManager::valueChanged,
        this, [this](Property* property, const QString& value) { impl_->slotValueChanged(property, value); });
    connect(stringPropertyManager, &StringPropertyManager::regExpChanged,
        this, [this](Property* property, const QRegExp& regExp) { impl_->slotRegExpChanged(property, regExp); });

    auto datePropertyManager = new DatePropertyManager(this);
    impl_->typeToPropertyManager_[QVariant::Date] = datePropertyManager;
    impl_->typeToValueType_[QVariant::Date] = QVariant::Date;
    impl_->typeToAttributeToAttributeType_[QVariant::Date][impl_->minimumAttribute_] = QVariant::Date;
    impl_->typeToAttributeToAttributeType_[QVariant::Date][impl_->maximumAttribute_] = QVariant::Date;
    connect(datePropertyManager, &DatePropertyManager::valueChanged,
        this, [this](Property* property, const QDate& date) { impl_->slotValueChanged(property, date); });
    connect(datePropertyManager, &DatePropertyManager::rangeChanged,
        this, [this](Property* property, const QDate& min, const QDate& max) { impl_->slotRangeChanged(property, min, max); });

    auto timePropertyManager = new TimePropertyManager(this);
    impl_->typeToPropertyManager_[QVariant::Time] = timePropertyManager;
    impl_->typeToValueType_[QVariant::Time] = QVariant::Time;
    connect(timePropertyManager, &TimePropertyManager::valueChanged,
        this, [this](Property* property, const QTime& time) { impl_->slotValueChanged(property, time); });

    auto dateTimePropertyManager = new DateTimePropertyManager(this);
    impl_->typeToPropertyManager_[QVariant::DateTime] = dateTimePropertyManager;
    impl_->typeToValueType_[QVariant::DateTime] = QVariant::DateTime;
    connect(dateTimePropertyManager, &DateTimePropertyManager::valueChanged,
        this, [this](Property* property, const QDateTime& datetime) { impl_->slotValueChanged(property, datetime); });

    auto keySequencePropertyManager = new KeySequencePropertyManager(this);
    impl_->typeToPropertyManager_[QVariant::KeySequence] = keySequencePropertyManager;
    impl_->typeToValueType_[QVariant::KeySequence] = QVariant::KeySequence;
    connect(keySequencePropertyManager, &KeySequencePropertyManager::valueChanged,
        this, [this](Property* property, const QKeySequence& sequence) { impl_->slotValueChanged(property, sequence); });


    auto charPropertyManager = new CharPropertyManager(this);
    impl_->typeToPropertyManager_[QVariant::Char] = charPropertyManager;
    impl_->typeToValueType_[QVariant::Char] = QVariant::Char;
    connect(charPropertyManager, &CharPropertyManager::valueChanged,
        this, [this](Property* property, const QChar& c) { impl_->slotValueChanged(property, c); });

    auto localePropertyManager = new LocalePropertyManager(this);
    impl_->typeToPropertyManager_[QVariant::Locale] = localePropertyManager;
    impl_->typeToValueType_[QVariant::Locale] = QVariant::Locale;
    connect(localePropertyManager, &LocalePropertyManager::valueChanged,
        this, [this](Property* property, const QLocale& locale) { impl_->slotValueChanged(property, locale); });
    connect(localePropertyManager->subEnumPropertyManager(), &EnumPropertyManager::valueChanged,
        this, [this](Property* property, int value) { impl_->slotValueChanged(property, value); });
    connect(localePropertyManager, &LocalePropertyManager::propertyInserted,
        this, [this](Property* property, Property* parent, Property* after) { impl_->slotPropertyInserted(property, parent, after); });
    connect(localePropertyManager, &LocalePropertyManager::propertyRemoved,
        this, [this](Property* property, Property* parent) { impl_->slotPropertyRemoved(property, parent); });

    auto pointPropertyManager = new PointPropertyManager(this);
    impl_->typeToPropertyManager_[QVariant::Point] = pointPropertyManager;
    impl_->typeToValueType_[QVariant::Point] = QVariant::Point;
    connect(pointPropertyManager, &PointPropertyManager::valueChanged,
        this, [this](Property* property, const QPoint& point) { impl_->slotValueChanged(property, point); });
    connect(pointPropertyManager->subIntPropertyManager(), &IntPropertyManager::valueChanged,
        this, [this](Property* property, int value) { impl_->slotValueChanged(property, value); });
    connect(pointPropertyManager, &PointPropertyManager::propertyInserted,
        this, [this](Property* property, Property* parent, Property* after) { impl_->slotPropertyInserted(property, parent, after); });
    connect(pointPropertyManager, &PointPropertyManager::propertyRemoved,
        this, [this](Property* property, Property* parent) { impl_->slotPropertyRemoved(property, parent); });

    auto pointFPropertyManager = new PointFPropertyManager(this);
    impl_->typeToPropertyManager_[QVariant::PointF] = pointFPropertyManager;
    impl_->typeToValueType_[QVariant::PointF] = QVariant::PointF;
    impl_->typeToAttributeToAttributeType_[QVariant::PointF][impl_->decimalsAttribute_] = QVariant::Int;
    connect(pointFPropertyManager, &PointFPropertyManager::valueChanged,
        this, [this](Property* property, const QPointF& p) { impl_->slotValueChanged(property, p); });
    connect(pointFPropertyManager, &PointFPropertyManager::decimalsChanged,
        this, [this](Property* property, int value) { impl_->slotDecimalsChanged(property, value); });
    connect(pointFPropertyManager->subDoublePropertyManager(), &DoublePropertyManager::valueChanged,
        this, [this](Property* property, double value) { impl_->slotValueChanged(property, value); });
    connect(pointFPropertyManager, &PointFPropertyManager::propertyInserted,
        this, [this](Property* property, Property* parent, Property* after) { impl_->slotPropertyInserted(property, parent, after); });
    connect(pointFPropertyManager, &PointFPropertyManager::propertyRemoved,
        this, [this](Property* property, Property* parent) { impl_->slotPropertyRemoved(property, parent); });

    auto sizePropertyManager = new SizePropertyManager(this);
    impl_->typeToPropertyManager_[QVariant::Size] = sizePropertyManager;
    impl_->typeToValueType_[QVariant::Size] = QVariant::Size;
    impl_->typeToAttributeToAttributeType_[QVariant::Size][impl_->minimumAttribute_] = QVariant::Size;
    impl_->typeToAttributeToAttributeType_[QVariant::Size][impl_->maximumAttribute_] = QVariant::Size;
    connect(sizePropertyManager, &SizePropertyManager::valueChanged,
        this, [this](Property* property, const QSize& size) { impl_->slotValueChanged(property, size); });
    connect(sizePropertyManager, &SizePropertyManager::rangeChanged,
        this, [this](Property* property, const QSize& min, const QSize& max) { impl_->slotRangeChanged(property, min, max); });
    connect(sizePropertyManager->subIntPropertyManager(), &IntPropertyManager::valueChanged,
        this, [this](Property* property, int value) { impl_->slotValueChanged(property, value); });
    connect(sizePropertyManager->subIntPropertyManager(), &IntPropertyManager::rangeChanged,
        this, [this](Property* property, int min, int max) { impl_->slotRangeChanged(property, min, max); });
    connect(sizePropertyManager, &SizePropertyManager::propertyInserted,
        this, [this](Property* property, Property* parent, Property* after) { impl_->slotPropertyInserted(property, parent, after); });
    connect(sizePropertyManager, &SizePropertyManager::propertyRemoved,
        this, [this](Property* property, Property* parent) { impl_->slotPropertyRemoved(property, parent); });

    auto sizeFPropertyManager = new SizeFPropertyManager(this);
    impl_->typeToPropertyManager_[QVariant::SizeF] = sizeFPropertyManager;
    impl_->typeToValueType_[QVariant::SizeF] = QVariant::SizeF;
    impl_->typeToAttributeToAttributeType_[QVariant::SizeF][impl_->minimumAttribute_] = QVariant::SizeF;
    impl_->typeToAttributeToAttributeType_[QVariant::SizeF][impl_->maximumAttribute_] = QVariant::SizeF;
    impl_->typeToAttributeToAttributeType_[QVariant::SizeF][impl_->decimalsAttribute_] = QVariant::Int;
    connect(sizeFPropertyManager, &SizeFPropertyManager::valueChanged,
        this, [this](Property* property, const QSizeF& size) { impl_->slotValueChanged(property, size); });
    connect(sizeFPropertyManager, &SizeFPropertyManager::rangeChanged,
        this, [this](Property* property, const QSizeF& min, const QSizeF& max) { impl_->slotRangeChanged(property, min, max); });
    connect(sizeFPropertyManager, &SizeFPropertyManager::decimalsChanged,
        this, [this](Property* property, int value) { impl_->slotDecimalsChanged(property, value); });
    connect(sizeFPropertyManager->subDoublePropertyManager(), &DoublePropertyManager::valueChanged,
        this, [this](Property* property, double value) { impl_->slotValueChanged(property, value); });
    connect(sizeFPropertyManager->subDoublePropertyManager(), &DoublePropertyManager::rangeChanged,
        this, [this](Property* property, double min, double max) { impl_->slotRangeChanged(property, min, max); });
    connect(sizeFPropertyManager, &SizeFPropertyManager::propertyInserted,
        this, [this](Property* property, Property* parent, Property* after) { impl_->slotPropertyInserted(property, parent, after); });
    connect(sizeFPropertyManager, &SizeFPropertyManager::propertyRemoved,
        this, [this](Property* property, Property* parent) { impl_->slotPropertyRemoved(property, parent); });

    auto rectPropertyManager = new RectPropertyManager(this);
    impl_->typeToPropertyManager_[QVariant::Rect] = rectPropertyManager;
    impl_->typeToValueType_[QVariant::Rect] = QVariant::Rect;
    impl_->typeToAttributeToAttributeType_[QVariant::Rect][impl_->constraintAttribute_] = QVariant::Rect;
    connect(rectPropertyManager, &RectPropertyManager::valueChanged,
        this, [this](Property* property, const QRect& val) { impl_->slotValueChanged(property, val); });
    connect(rectPropertyManager, &RectPropertyManager::constraintChanged,
        this, [this](Property* property, const QRect& rect) { impl_->slotConstraintChanged(property, rect); });
    connect(rectPropertyManager->subIntPropertyManager(), &IntPropertyManager::valueChanged,
        this, [this](Property* property, int val) { impl_->slotValueChanged(property, val); });
    connect(rectPropertyManager->subIntPropertyManager(), &IntPropertyManager::rangeChanged,
        this, [this](Property* property, int min, int max) { impl_->slotRangeChanged(property, min, max); });
    connect(rectPropertyManager, &RectPropertyManager::propertyInserted,
        this, [this](Property* property, Property* parent, Property* after) { impl_->slotPropertyInserted(property, parent, after); });
    connect(rectPropertyManager, &RectPropertyManager::propertyRemoved,
        this, [this](Property* property, Property* parent) { impl_->slotPropertyRemoved(property, parent); });

    auto rectFPropertyManager = new RectFPropertyManager(this);
    impl_->typeToPropertyManager_[QVariant::RectF] = rectFPropertyManager;
    impl_->typeToValueType_[QVariant::RectF] = QVariant::RectF;
    impl_->typeToAttributeToAttributeType_[QVariant::RectF][impl_->constraintAttribute_] = QVariant::RectF;
    impl_->typeToAttributeToAttributeType_[QVariant::RectF][impl_->decimalsAttribute_] = QVariant::Int;
    connect(rectFPropertyManager, &RectFPropertyManager::valueChanged,
        this, [this](Property* property, const QRectF& rect) { impl_->slotValueChanged(property, rect); });
    connect(rectFPropertyManager, &RectFPropertyManager::constraintChanged,
        this, [this](Property* property, const QRectF& rect) { impl_->slotConstraintChanged(property, rect); });
    connect(rectFPropertyManager, &RectFPropertyManager::decimalsChanged,
        this, [this](Property* property, int value) { impl_->slotDecimalsChanged(property, value); });
    connect(rectFPropertyManager->subDoublePropertyManager(), &DoublePropertyManager::valueChanged,
        this, [this](Property* property, double value) { impl_->slotValueChanged(property, value); });
    connect(rectFPropertyManager->subDoublePropertyManager(), &DoublePropertyManager::rangeChanged,
        this, [this](Property* property, double min, double max) { impl_->slotRangeChanged(property, min, max); });
    connect(rectFPropertyManager, &RectFPropertyManager::propertyInserted,
        this, [this](Property* property, Property* parent, Property* after) { impl_->slotPropertyInserted(property, parent, after); });
    connect(rectFPropertyManager, &RectFPropertyManager::propertyRemoved,
        this, [this](Property* property, Property* parent) { impl_->slotPropertyRemoved(property, parent); });

    auto colorPropertyManager = new ColorPropertyManager(this);
    impl_->typeToPropertyManager_[QVariant::Color] = colorPropertyManager;
    impl_->typeToValueType_[QVariant::Color] = QVariant::Color;
    connect(colorPropertyManager, &ColorPropertyManager::valueChanged,
        this, [this](Property* property, const QColor& color) { impl_->slotValueChanged(property, color); });
    connect(colorPropertyManager->subIntPropertyManager(), &IntPropertyManager::valueChanged,
        this, [this](Property* property, int value) { impl_->slotValueChanged(property, value); });
    connect(colorPropertyManager, &ColorPropertyManager::propertyInserted,
        this, [this](Property* property, Property* parent, Property* after) { impl_->slotPropertyInserted(property, parent, after); });
    connect(colorPropertyManager, &ColorPropertyManager::propertyRemoved,
        this, [this](Property* property, Property* parent) { impl_->slotPropertyRemoved(property, parent); });

    int enumId = enumTypeId();
    auto enumPropertyManager = new EnumPropertyManager(this);
    impl_->typeToPropertyManager_[enumId] = enumPropertyManager;
    impl_->typeToValueType_[enumId] = QVariant::Int;
    impl_->typeToAttributeToAttributeType_[enumId][impl_->enumNamesAttribute_] = QVariant::StringList;
    impl_->typeToAttributeToAttributeType_[enumId][impl_->enumIconsAttribute_] = iconMapTypeId();
    connect(enumPropertyManager, &EnumPropertyManager::valueChanged,
        this, [this](Property* property, int value) { impl_->slotValueChanged(property, value); });
    connect(enumPropertyManager, &EnumPropertyManager::enumNamesChanged,
        this, [this](Property* property, const QStringList& names) { impl_->slotEnumNamesChanged(property, names); });
    connect(enumPropertyManager, &EnumPropertyManager::enumIconsChanged,
        this, [this](Property* property, const QMap<int,QIcon>& icons) { impl_->slotEnumIconsChanged(property,icons); });

    auto sizePolicyPropertyManager = new SizePolicyPropertyManager(this);
    impl_->typeToPropertyManager_[QVariant::SizePolicy] = sizePolicyPropertyManager;
    impl_->typeToValueType_[QVariant::SizePolicy] = QVariant::SizePolicy;
    connect(sizePolicyPropertyManager, &SizePolicyPropertyManager::valueChanged,
        this, [this](Property* property, const QSizePolicy& policy) { impl_->slotValueChanged(property, policy); });
    connect(sizePolicyPropertyManager->subIntPropertyManager(), &IntPropertyManager::valueChanged,
        this, [this](Property* property, int value) { impl_->slotValueChanged(property, value); });
    connect(sizePolicyPropertyManager->subIntPropertyManager(), &IntPropertyManager::rangeChanged,
        this, [this](Property* property, int min, int max) { impl_->slotRangeChanged(property, min, max); });
    connect(sizePolicyPropertyManager->subEnumPropertyManager(), &EnumPropertyManager::valueChanged,
        this, [this](Property* property, int value) { impl_->slotValueChanged(property, value); });
    connect(sizePolicyPropertyManager->subEnumPropertyManager(), &EnumPropertyManager::enumNamesChanged,
        this, [this](Property* property, const QStringList& names) { impl_->slotEnumNamesChanged(property, names); });
    connect(sizePolicyPropertyManager, &SizePolicyPropertyManager::propertyInserted,
        this, [this](Property* property, Property* parent, Property* after) { impl_->slotPropertyInserted(property, parent, after); });
    connect(sizePolicyPropertyManager, &SizePolicyPropertyManager::propertyRemoved,
        this, [this](Property* property, Property* parent) { impl_->slotPropertyRemoved(property, parent); });

    auto fontPropertyManager = new FontPropertyManager(this);
    impl_->typeToPropertyManager_[QVariant::Font] = fontPropertyManager;
    impl_->typeToValueType_[QVariant::Font] = QVariant::Font;
    connect(fontPropertyManager, &FontPropertyManager::valueChanged,
        this, [this](Property* property, const QFont& font) { impl_->slotValueChanged(property, font); });
    connect(fontPropertyManager->subIntPropertyManager(), &IntPropertyManager::valueChanged,
        this, [this](Property* property, int value) { impl_->slotValueChanged(property, value); });
    connect(fontPropertyManager->subIntPropertyManager(), &IntPropertyManager::rangeChanged,
        this, [this](Property* property, int min, int max) { impl_->slotRangeChanged(property, min, max); });
    connect(fontPropertyManager->subEnumPropertyManager(), &EnumPropertyManager::valueChanged,
        this, [this](Property* property, int value) { impl_->slotValueChanged(property, value); });
    connect(fontPropertyManager->subEnumPropertyManager(), &EnumPropertyManager::enumNamesChanged,
        this, [this](Property* property, const QStringList& names) {impl_->slotEnumNamesChanged(property, names); });
    connect(fontPropertyManager->subBoolPropertyManager(), &BoolPropertyManager::valueChanged,
        this, [this](Property* property, bool value) { impl_->slotValueChanged(property, value); });
    connect(fontPropertyManager, &FontPropertyManager::propertyInserted,
        this, [this](Property* property, Property* parent, Property* after) { impl_->slotPropertyInserted(property, parent, after); });
    connect(fontPropertyManager, &FontPropertyManager::propertyRemoved,
        this, [this](Property* property, Property* parent) { impl_->slotPropertyRemoved(property, parent); });

    auto cursorPropertyManager = new CursorPropertyManager(this);
    impl_->typeToPropertyManager_[QVariant::Cursor] = cursorPropertyManager;
    impl_->typeToValueType_[QVariant::Cursor] = QVariant::Cursor;
    connect(cursorPropertyManager, &CursorPropertyManager::valueChanged,
        this, [this](Property* property, const QCursor& cursor) { impl_->slotValueChanged(property, cursor); });

    int flagId = flagTypeId();
    auto flagPropertyManager = new FlagPropertyManager(this);
    impl_->typeToPropertyManager_[flagId] = flagPropertyManager;
    impl_->typeToValueType_[flagId] = QVariant::Int;
    impl_->typeToAttributeToAttributeType_[flagId][impl_->flagNamesAttribute_] = QVariant::StringList;
    connect(flagPropertyManager, &FlagPropertyManager::valueChanged,
        this, [this](Property* property, int value) { impl_->slotValueChanged(property, value); });
    connect(flagPropertyManager, &FlagPropertyManager::flagNamesChanged,
        this, [this](Property* property, const QStringList& flagNames){ impl_->slotFlagNamesChanged(property, flagNames); });
    connect(flagPropertyManager->subBoolPropertyManager(), &BoolPropertyManager::valueChanged,
        this, [this](Property* property, bool value) { impl_->slotValueChanged(property, value); });
    connect(flagPropertyManager, &FlagPropertyManager::propertyInserted,
        this, [this](Property* property, Property* parent, Property* after) { impl_->slotPropertyInserted(property, parent, after); });
    connect(flagPropertyManager, &FlagPropertyManager::propertyRemoved,
        this, [this](Property* property, Property* parent) { impl_->slotPropertyRemoved(property, parent); });

    int groupId = groupTypeId();
    auto groupPropertyManager = new GroupPropertyManager(this);
    impl_->typeToPropertyManager_[groupId] = groupPropertyManager;
    impl_->typeToValueType_[groupId] = QVariant::Invalid;
}

VariantPropertyManager::~VariantPropertyManager()
{
    clear();
}

VariantProperty *VariantPropertyManager::variantProperty(const Property* property) const
{
    const auto it = impl_->propertyToType_.constFind(property);
    if (it == impl_->propertyToType_.constEnd())
    {
        return nullptr;
    }
    return it.value().first;
}

bool VariantPropertyManager::isPropertyTypeSupported(int propertyType) const
{
    return impl_->typeToValueType_.contains(propertyType);
}

VariantProperty* VariantPropertyManager::addProperty(int propertyType, const QString& name)
{
    if (!isPropertyTypeSupported(propertyType))
    {
        return nullptr;
    }
    auto wasCreating = impl_->creatingProperty_;
    impl_->creatingProperty_ = true;
    impl_->propertyType_ = propertyType;
    auto property = AbstractPropertyManager::addProperty(name);
    impl_->creatingProperty_ = wasCreating;
    impl_->propertyType_ = 0;

    if (!property)
    {
        return nullptr;
    }
    return variantProperty(property);
}

QVariant VariantPropertyManager::value(const Property* property) const
{
    auto internProp = propertyToWrappedProperty()->value(property, 0);
    if (!internProp)
    {
        return QVariant();
    }
    auto manager = internProp->propertyManager();
    if (auto intManager = qobject_cast<IntPropertyManager*>(manager))
    {
        return intManager->value(internProp);
    }
    else if (auto doubleManager = qobject_cast<DoublePropertyManager*>(manager))
    {
        return doubleManager->value(internProp);
    }
    else if (auto boolManager = qobject_cast<BoolPropertyManager*>(manager))
    {
        return boolManager->value(internProp);
    }
    else if (auto stringManager = qobject_cast<StringPropertyManager*>(manager))
    {
        return stringManager->value(internProp);
    }
    else if (auto dateManager = qobject_cast<DatePropertyManager*>(manager))
    {
        return dateManager->value(internProp);
    }
    else if (auto timeManager = qobject_cast<TimePropertyManager*>(manager))
    {
        return timeManager->value(internProp);
    }
    else if (auto dateTimeManager = qobject_cast<DateTimePropertyManager *>(manager))
    {
        return dateTimeManager->value(internProp);
    }
    else if (auto keySequenceManager = qobject_cast<KeySequencePropertyManager*>(manager))
    {
        return QVariant::fromValue(keySequenceManager->value(internProp));
    }
    else if (auto charManager = qobject_cast<CharPropertyManager*>(manager))
    {
        return charManager->value(internProp);
    }
    else if (auto localeManager = qobject_cast<LocalePropertyManager*>(manager))
    {
        return localeManager->value(internProp);
    }
    else if (auto pointManager = qobject_cast<PointPropertyManager*>(manager))
    {
        return pointManager->value(internProp);
    }
    else if (auto pointFManager = qobject_cast<PointFPropertyManager*>(manager))
    {
        return pointFManager->value(internProp);
    }
    else if (auto sizeManager = qobject_cast<SizePropertyManager*>(manager))
    {
        return sizeManager->value(internProp);
    }
    else if (auto sizeFManager = qobject_cast<SizeFPropertyManager*>(manager))
    {
        return sizeFManager->value(internProp);
    }
    else if (auto rectManager = qobject_cast<RectPropertyManager*>(manager))
    {
        return rectManager->value(internProp);
    }
    else if (auto rectFManager = qobject_cast<RectFPropertyManager*>(manager))
    {
        return rectFManager->value(internProp);
    }
    else if (auto colorManager = qobject_cast<ColorPropertyManager*>(manager))
    {
        return colorManager->value(internProp);
    }
    else if (auto enumManager = qobject_cast<EnumPropertyManager*>(manager))
    {
        return enumManager->value(internProp);
    }
    else if (auto sizePolicyManager = qobject_cast<SizePolicyPropertyManager*>(manager))
    {
        return sizePolicyManager->value(internProp);
    }
    else if (auto fontManager = qobject_cast<FontPropertyManager*>(manager))
    {
        return fontManager->value(internProp);
    }
    else if (auto cursorManager = qobject_cast<CursorPropertyManager*>(manager))
    {
        return cursorManager->value(internProp);

    }
    else if (auto flagManager = qobject_cast<FlagPropertyManager*>(manager))
    {
        return flagManager->value(internProp);
    }
    return QVariant();
}

int VariantPropertyManager::valueType(const Property* property) const
{
    int propType = propertyType(property);
    return valueType(propType);
}

int VariantPropertyManager::valueType(int propertyType) const
{
    if (impl_->typeToValueType_.contains(propertyType))
    {
        return impl_->typeToValueType_[propertyType];
    }
    return 0;
}

int VariantPropertyManager::propertyType(const Property* property) const
{
    const auto it = impl_->propertyToType_.constFind(property);
    if (it == impl_->propertyToType_.constEnd())
    {
        return 0;
    }
    return it.value().second;
}

QVariant VariantPropertyManager::attributeValue(const Property* property, const QString& attribute) const
{
    auto propType = propertyType(property);
    if (!propType)
    {
        return QVariant();
    }
    auto it = impl_->typeToAttributeToAttributeType_.find(propType);
    if (it == impl_->typeToAttributeToAttributeType_.constEnd())
    {
        return QVariant();
    }
    auto attributes = it.value();
    auto itAttr = attributes.find(attribute);
    if (itAttr == attributes.constEnd())
    {
        return QVariant();
    }
    auto internProp = propertyToWrappedProperty()->value(property, 0);
    if (!internProp)
    {
        return QVariant();
    }
    auto manager = internProp->propertyManager();
    if (auto intManager = qobject_cast<IntPropertyManager*>(manager))
    {
        if (attribute == impl_->maximumAttribute_)
        {
            return intManager->maximum(internProp);
        }
        if (attribute == impl_->minimumAttribute_)
        {
            return intManager->minimum(internProp);
        }
        if (attribute == impl_->singleStepAttribute_)
        {
            return intManager->singleStep(internProp);
        }
        return QVariant();
    }
    else if (auto doubleManager = qobject_cast<DoublePropertyManager*>(manager))
    {
        if (attribute == impl_->maximumAttribute_)
        {
            return doubleManager->maximum(internProp);
        }
        if (attribute == impl_->minimumAttribute_)
        {
            return doubleManager->minimum(internProp);
        }
        if (attribute == impl_->singleStepAttribute_)
        {
            return doubleManager->singleStep(internProp);
        }
        if (attribute == impl_->decimalsAttribute_)
        {
            return doubleManager->decimals(internProp);
        }
        return QVariant();
    }
    else if (auto stringManager = qobject_cast<StringPropertyManager*>(manager))
    {
        if (attribute == impl_->regExpAttribute_)
        {
            return stringManager->regExp(internProp);
        }
        return QVariant();
    }
    else if (auto dateManager = qobject_cast<DatePropertyManager*>(manager))
    {
        if (attribute == impl_->maximumAttribute_)
        {
            return dateManager->maximum(internProp);
        }
        if (attribute == impl_->minimumAttribute_)
        {
            return dateManager->minimum(internProp);
        }
        return QVariant();
    }
    else if (auto pointFManager = qobject_cast<PointFPropertyManager*>(manager))
    {
        if (attribute == impl_->decimalsAttribute_)
        {
            return pointFManager->decimals(internProp);
        }
        return QVariant();
    }
    else if (auto sizeManager = qobject_cast<SizePropertyManager*>(manager))
    {
        if (attribute == impl_->maximumAttribute_)
        {
            return sizeManager->maximum(internProp);
        }
        if (attribute == impl_->minimumAttribute_)
        {
            return sizeManager->minimum(internProp);
        }
        return QVariant();
    }
    else if (auto sizeFManager = qobject_cast<SizeFPropertyManager*>(manager))
    {
        if (attribute == impl_->maximumAttribute_)
        {
            return sizeFManager->maximum(internProp);
        }
        if (attribute == impl_->minimumAttribute_)
        {
            return sizeFManager->minimum(internProp);
        }
        if (attribute == impl_->decimalsAttribute_)
        {
            return sizeFManager->decimals(internProp);
        }
        return QVariant();
    }
    else if (auto rectManager = qobject_cast<RectPropertyManager*>(manager))
    {
        if (attribute == impl_->constraintAttribute_)
        {
            return rectManager->constraint(internProp);
        }
        return QVariant();
    }
    else if (auto rectFManager = qobject_cast<RectFPropertyManager*>(manager))
    {
        if (attribute == impl_->constraintAttribute_)
        {
            return rectFManager->constraint(internProp);
        }
        if (attribute == impl_->decimalsAttribute_)
        {
            return rectFManager->decimals(internProp);
        }
        return QVariant();
    }
    else if (auto enumManager = qobject_cast<EnumPropertyManager*>(manager))
    {
        if (attribute == impl_->enumNamesAttribute_)
        {
            return enumManager->enumNames(internProp);
        }
        if (attribute == impl_->enumIconsAttribute_)
        {
            QVariant v;
            v.setValue(enumManager->enumIcons(internProp));
            return v;
        }
        return QVariant();
    }
    else if (auto flagManager = qobject_cast<FlagPropertyManager*>(manager))
    {
        if (attribute == impl_->flagNamesAttribute_)
        {
            return flagManager->flagNames(internProp);
        }
        return QVariant();
    }
    return QVariant();
}

QStringList VariantPropertyManager::attributes(int propertyType) const
{
    auto it = impl_->typeToAttributeToAttributeType_.find(propertyType);
    if (it == impl_->typeToAttributeToAttributeType_.constEnd())
    {
        return QStringList();
    }
    return it.value().keys();
}

int VariantPropertyManager::attributeType(int propertyType, const QString& attribute) const
{
    auto it = impl_->typeToAttributeToAttributeType_.find(propertyType);
    if (it == impl_->typeToAttributeToAttributeType_.constEnd())
    {
        return 0;
    }
    auto attributes = it.value();
    auto itAttr = attributes.find(attribute);
    if (itAttr == attributes.constEnd())
    {
        return 0;
    }
    return itAttr.value();
}

void VariantPropertyManager::setValue(Property* property, const QVariant& val)
{
    auto propType = val.userType();
    if (!propType)
    {
        return;
    }
    auto valType = valueType(property);

    if (propType != valType && !val.canConvert(static_cast<QVariant::Type>(valType)))
    {
        return;
    }
    auto internProp = propertyToWrappedProperty()->value(property, 0);
    if (!internProp)
    {
        return;
    }

    auto manager = internProp->propertyManager();
    if (auto intManager = qobject_cast<IntPropertyManager*>(manager))
    {
        intManager->setValue(internProp, qvariant_cast<int>(val));
        return;
    }
    else if (auto doubleManager = qobject_cast<DoublePropertyManager*>(manager))
    {
        doubleManager->setValue(internProp, qvariant_cast<double>(val));
        return;
    }
    else if (auto boolManager = qobject_cast<BoolPropertyManager*>(manager))
    {
        boolManager->setValue(internProp, qvariant_cast<bool>(val));
        return;
    } else if (auto stringManager = qobject_cast<StringPropertyManager*>(manager))
    {
        stringManager->setValue(internProp, qvariant_cast<QString>(val));
        return;
    }
    else if (auto dateManager = qobject_cast<DatePropertyManager*>(manager))
    {
        dateManager->setValue(internProp, qvariant_cast<QDate>(val));
        return;
    }
    else if (auto timeManager = qobject_cast<TimePropertyManager*>(manager))
    {
        timeManager->setValue(internProp, qvariant_cast<QTime>(val));
        return;
    }
    else if (auto dateTimeManager = qobject_cast<DateTimePropertyManager*>(manager))
    {
        dateTimeManager->setValue(internProp, qvariant_cast<QDateTime>(val));
        return;
    }
    else if (auto keySequenceManager = qobject_cast<KeySequencePropertyManager*>(manager))
    {
        keySequenceManager->setValue(internProp, qvariant_cast<QKeySequence>(val));
        return;
    }
    else if (auto charManager = qobject_cast<CharPropertyManager*>(manager))
    {
        charManager->setValue(internProp, qvariant_cast<QChar>(val));
        return;
    }
    else if (auto localeManager = qobject_cast<LocalePropertyManager*>(manager))
    {
        localeManager->setValue(internProp, qvariant_cast<QLocale>(val));
        return;
    }
    else if (auto pointManager = qobject_cast<PointPropertyManager*>(manager))
    {
        pointManager->setValue(internProp, qvariant_cast<QPoint>(val));
        return;
    }
    else if (auto pointFManager = qobject_cast<PointFPropertyManager*>(manager))
    {
        pointFManager->setValue(internProp, qvariant_cast<QPointF>(val));
        return;
    }
    else if (auto sizeManager = qobject_cast<SizePropertyManager*>(manager))
    {
        sizeManager->setValue(internProp, qvariant_cast<QSize>(val));
        return;
    }
    else if (auto sizeFManager = qobject_cast<SizeFPropertyManager*>(manager))
    {
        sizeFManager->setValue(internProp, qvariant_cast<QSizeF>(val));
        return;
    }
    else if (auto rectManager = qobject_cast<RectPropertyManager*>(manager))
    {
        rectManager->setValue(internProp, qvariant_cast<QRect>(val));
        return;
    }
    else if (auto rectFManager = qobject_cast<RectFPropertyManager*>(manager))
    {
        rectFManager->setValue(internProp, qvariant_cast<QRectF>(val));
        return;
    }
    else if (auto colorManager = qobject_cast<ColorPropertyManager*>(manager))
    {
        colorManager->setValue(internProp, qvariant_cast<QColor>(val));
        return;
    }
    else if (auto enumManager = qobject_cast<EnumPropertyManager*>(manager))
    {
        enumManager->setValue(internProp, qvariant_cast<int>(val));
        return;
    }
    else if (auto sizePolicyManager = qobject_cast<SizePolicyPropertyManager*>(manager))
    {
        sizePolicyManager->setValue(internProp, qvariant_cast<QSizePolicy>(val));
        return;
    }
    else if (auto fontManager = qobject_cast<FontPropertyManager*>(manager))
    {
        fontManager->setValue(internProp, qvariant_cast<QFont>(val));
        return;
    }
    else if (auto cursorManager = qobject_cast<CursorPropertyManager*>(manager))
    {
        cursorManager->setValue(internProp, qvariant_cast<QCursor>(val));
        return;
    }
    else if (auto flagManager = qobject_cast<FlagPropertyManager*>(manager))
    {
        flagManager->setValue(internProp, qvariant_cast<int>(val));
        return;
    }
}

void VariantPropertyManager::setAttribute(Property* property,
    const QString& attribute, const QVariant& value)
{
    auto oldAttr = attributeValue(property, attribute);
    if (!oldAttr.isValid())
    {
        return;
    }
    int attrType = value.userType();
    if (!attrType)
    {
        return;
    }
    if (attrType != attributeType(propertyType(property), attribute) &&
        !value.canConvert((QVariant::Type)attrType))
    {
        return;
    }
    auto internProp = propertyToWrappedProperty()->value(property, 0);
    if (!internProp)
    {
        return;
    }
    auto manager = internProp->propertyManager();
    if (auto intManager = qobject_cast<IntPropertyManager*>(manager))
    {
        if (attribute == impl_->maximumAttribute_)
        {
            intManager->setMaximum(internProp, qvariant_cast<int>(value));
        }
        else if (attribute == impl_->minimumAttribute_)
        {
            intManager->setMinimum(internProp, qvariant_cast<int>(value));
        }
        else if (attribute == impl_->singleStepAttribute_)
        {
            intManager->setSingleStep(internProp, qvariant_cast<int>(value));
        }
        return;
    }
    else if (auto doubleManager = qobject_cast<DoublePropertyManager*>(manager))
    {
        if (attribute == impl_->maximumAttribute_)
        {
            doubleManager->setMaximum(internProp, qvariant_cast<double>(value));
        }
        if (attribute == impl_->minimumAttribute_)
        {
            doubleManager->setMinimum(internProp, qvariant_cast<double>(value));
        }
        if (attribute == impl_->singleStepAttribute_)
        {
            doubleManager->setSingleStep(internProp, qvariant_cast<double>(value));
        }
        if (attribute == impl_->decimalsAttribute_)
        {
            doubleManager->setDecimals(internProp, qvariant_cast<int>(value));
        }
        return;
    }
    else if (auto stringManager = qobject_cast<StringPropertyManager*>(manager))
    {
        if (attribute == impl_->regExpAttribute_)
        {
            stringManager->setRegExp(internProp, qvariant_cast<QRegExp>(value));
        }
        return;
    }
    else if (auto dateManager = qobject_cast<DatePropertyManager*>(manager))
    {
        if (attribute == impl_->maximumAttribute_)
        {
            dateManager->setMaximum(internProp, qvariant_cast<QDate>(value));
        }
        if (attribute == impl_->minimumAttribute_)
        {
            dateManager->setMinimum(internProp, qvariant_cast<QDate>(value));
        }
        return;
    }
    else if (auto pointFManager = qobject_cast<PointFPropertyManager*>(manager))
    {
        if (attribute == impl_->decimalsAttribute_)
        {
            pointFManager->setDecimals(internProp, qvariant_cast<int>(value));
        }
        return;
    }
    else if (auto sizeManager = qobject_cast<SizePropertyManager*>(manager))
    {
        if (attribute == impl_->maximumAttribute_)
        {
            sizeManager->setMaximum(internProp, qvariant_cast<QSize>(value));
        }
        if (attribute == impl_->minimumAttribute_)
        {
            sizeManager->setMinimum(internProp, qvariant_cast<QSize>(value));
        }
        return;
    }
    else if (auto sizeFManager = qobject_cast<SizeFPropertyManager*>(manager))
    {
        if (attribute == impl_->maximumAttribute_)
        {
            sizeFManager->setMaximum(internProp, qvariant_cast<QSizeF>(value));
        }
        if (attribute == impl_->minimumAttribute_)
        {
            sizeFManager->setMinimum(internProp, qvariant_cast<QSizeF>(value));
        }
        if (attribute == impl_->decimalsAttribute_)
        {
            sizeFManager->setDecimals(internProp, qvariant_cast<int>(value));
        }
        return;
    }
    else if (auto rectManager = qobject_cast<RectPropertyManager*>(manager))
    {
        if (attribute == impl_->constraintAttribute_)
        {
            rectManager->setConstraint(internProp, qvariant_cast<QRect>(value));
        }
        return;
    }
    else if (auto rectFManager = qobject_cast<RectFPropertyManager*>(manager))
    {
        if (attribute == impl_->constraintAttribute_)
        {
            rectFManager->setConstraint(internProp, qvariant_cast<QRectF>(value));
        }
        if (attribute == impl_->decimalsAttribute_)
        {
            rectFManager->setDecimals(internProp, qvariant_cast<int>(value));
        }
        return;
    }
    else if (auto enumManager = qobject_cast<EnumPropertyManager*>(manager))
    {
        if (attribute == impl_->enumNamesAttribute_)
        {
            enumManager->setEnumNames(internProp, qvariant_cast<QStringList>(value));
        }
        if (attribute == impl_->enumIconsAttribute_)
        {
            enumManager->setEnumIcons(internProp, qvariant_cast<IconMap>(value));
        }
        return;
    }
    else if (auto flagManager = qobject_cast<FlagPropertyManager*>(manager))
    {
        if (attribute == impl_->flagNamesAttribute_)
        {
            flagManager->setFlagNames(internProp, qvariant_cast<QStringList>(value));
        }
        return;
    }
}

bool VariantPropertyManager::hasValue(const Property* property) const
{
    return propertyType(property) != groupTypeId();
}

QString VariantPropertyManager::valueText(const Property *property) const
{
    const auto internProp = propertyToWrappedProperty()->value(property, 0);
    return internProp ? internProp->valueText() : QString();
}

QIcon VariantPropertyManager::valueIcon(const Property* property) const
{
    const auto internProp = propertyToWrappedProperty()->value(property, 0);
    return internProp ? internProp->valueIcon() : QIcon();
}

void VariantPropertyManager::initializeProperty(Property* property)
{
    auto varProp = variantProperty(property);
    if (!varProp)
    {
        return;
    }
    auto it = impl_->typeToPropertyManager_.find(impl_->propertyType_);
    if (it != impl_->typeToPropertyManager_.constEnd())
    {
        Property* internProp = nullptr;
        if (!impl_->creatingSubProperties_)
        {
            auto manager = it.value();
            internProp = manager->addProperty();
            impl_->internalToProperty_[internProp] = varProp;
        }
        propertyToWrappedProperty()->insert(varProp, internProp);
        if (internProp)
        {
            const auto children = internProp->subProperties();
            VariantProperty* lastProperty = nullptr;
            for (auto&& child : children)
            {
                auto prop = impl_->createSubProperty(varProp, lastProperty, child);
                lastProperty = prop ? prop : lastProperty;
            }
        }
    }
}

void VariantPropertyManager::uninitializeProperty(Property* property)
{
    const auto typeIt = impl_->propertyToType_.find(property);
    if (typeIt == impl_->propertyToType_.end())
    {
        return;
    }
    auto it = propertyToWrappedProperty()->find(property);
    if (it != propertyToWrappedProperty()->end())
    {
        auto internProp = it.value();
        if (internProp)
        {
            impl_->internalToProperty_.remove(internProp);
            if (!impl_->destroyingSubProperties_)
            {
                delete internProp;
            }
        }
        propertyToWrappedProperty()->erase(it);
    }
    impl_->propertyToType_.erase(typeIt);
}

Property* VariantPropertyManager::createProperty()
{
    if (!impl_->creatingProperty_)
    {
        return 0;
    }
    auto property = new VariantProperty(this);
    impl_->propertyToType_.insert(property, qMakePair(property, impl_->propertyType_));

    return property;
}

