/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "fontpropertymanager.h"
#include "property.h"
#include "propertymanager/enumpropertymanager.h"
#include "propertymanager/intpropertymanager.h"
#include "propertymanager/boolpropertymanager.h"
#include "private/fontpropertymanager_p.h"
#include "private/propertybrowserutils_p.h"
#include <QtWidgets/QApplication>
#include <QtGui/QIcon>

FontPropertyManager::FontPropertyManager(QObject* parent) :
    AbstractPropertyManager(parent),
    impl_(new FontPropertyManagerPrivate)
{
    impl_->fontPropertyManagerPtr_ = this;
    QObject::connect(qApp, &QApplication::fontDatabaseChanged,
        this, [this]() { impl_->slotFontDatabaseChanged(); });

    impl_->intPropertyManager_ = new IntPropertyManager(this);
    connect(impl_->intPropertyManager_, &IntPropertyManager::valueChanged,
        this, [this](Property* property, int value) { impl_->slotIntChanged(property, value); });
    impl_->enumPropertyManager_ = new EnumPropertyManager(this);
    connect(impl_->enumPropertyManager_, &EnumPropertyManager::valueChanged,
        this, [this](Property* property, int value) {impl_->slotEnumChanged(property, value); });
    impl_->boolPropertyManager_ = new BoolPropertyManager(this);
    connect(impl_->boolPropertyManager_, &BoolPropertyManager::valueChanged,
        this, [this](Property* property, bool value) { impl_->slotBoolChanged(property, value); });

    connect(impl_->intPropertyManager_, &IntPropertyManager::propertyDestroyed,
        this, [this](Property* property) { impl_->slotPropertyDestroyed(property); });
    connect(impl_->enumPropertyManager_, &EnumPropertyManager::propertyDestroyed,
        this, [this](Property* property) { impl_->slotPropertyDestroyed(property); });
    connect(impl_->boolPropertyManager_, &BoolPropertyManager::propertyDestroyed,
        this, [this](Property* property) { impl_->slotPropertyDestroyed(property); });
}

FontPropertyManager::~FontPropertyManager()
{
    clear();
}

IntPropertyManager* FontPropertyManager::subIntPropertyManager() const
{
    return impl_->intPropertyManager_;
}

EnumPropertyManager* FontPropertyManager::subEnumPropertyManager() const
{
    return impl_->enumPropertyManager_;
}

BoolPropertyManager* FontPropertyManager::subBoolPropertyManager() const
{
    return impl_->boolPropertyManager_;
}

QFont FontPropertyManager::value(const Property* property) const
{
    return impl_->values_.value(property, QFont());
}

QString FontPropertyManager::valueText(const Property* property) const
{
    const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QString();
    }
    return PropertyBrowserUtils::fontValueText(it.value());
}

QIcon FontPropertyManager::valueIcon(const Property* property) const
{
    const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QIcon();
    }
    return PropertyBrowserUtils::fontValueIcon(it.value());
}

void FontPropertyManager::setValue(Property* property, const QFont& val)
{
    const auto it = impl_->values_.find(property);
    if (it == impl_->values_.end())
    {
        return;
    }
    const auto oldVal = it.value();
    if (oldVal == val && oldVal.resolve() == val.resolve())
    {
        return;
    }
    it.value() = val;

    auto idx = impl_->familyNames_.indexOf(val.family());
    if (idx == -1)
    {
        idx = 0;
    }
    auto settingValue = impl_->settingValue_;
    impl_->settingValue_ = true;
    impl_->enumPropertyManager_->setValue(impl_->propertyToFamily_[property], idx);
    impl_->intPropertyManager_->setValue(impl_->propertyToPointSize_[property], val.pointSize());
    impl_->boolPropertyManager_->setValue(impl_->propertyToBold_[property], val.bold());
    impl_->boolPropertyManager_->setValue(impl_->propertyToItalic_[property], val.italic());
    impl_->boolPropertyManager_->setValue(impl_->propertyToUnderline_[property], val.underline());
    impl_->boolPropertyManager_->setValue(impl_->propertyToStrikeOut_[property], val.strikeOut());
    impl_->boolPropertyManager_->setValue(impl_->propertyToKerning_[property], val.kerning());
    impl_->settingValue_ = settingValue;

    emit propertyChanged(property);
    emit valueChanged(property, val);
}

void FontPropertyManager::initializeProperty(Property* property)
{
    QFont val;
    impl_->values_[property] = val;

    auto familyProp = impl_->enumPropertyManager_->addProperty();
    familyProp->setPropertyName(tr("Family"));
    if (impl_->familyNames_.empty())
    {
        impl_->familyNames_ = fontDatabase()->families();
    }
    impl_->enumPropertyManager_->setEnumNames(familyProp, impl_->familyNames_);
    int idx = impl_->familyNames_.indexOf(val.family());
    if (idx == -1)
    {
        idx = 0;
    }
    impl_->enumPropertyManager_->setValue(familyProp, idx);
    impl_->propertyToFamily_[property] = familyProp;
    impl_->familyToProperty_[familyProp] = property;
    property->addSubProperty(familyProp);

    auto pointSizeProp = impl_->intPropertyManager_->addProperty();
    pointSizeProp->setPropertyName(tr("Point Size"));
    impl_->intPropertyManager_->setValue(pointSizeProp, val.pointSize());
    impl_->intPropertyManager_->setMinimum(pointSizeProp, 1);
    impl_->propertyToPointSize_[property] = pointSizeProp;
    impl_->pointSizeToProperty_[pointSizeProp] = property;
    property->addSubProperty(pointSizeProp);

    auto boldProp = impl_->boolPropertyManager_->addProperty();
    boldProp->setPropertyName(tr("Bold"));
    impl_->boolPropertyManager_->setValue(boldProp, val.bold());
    impl_->propertyToBold_[property] = boldProp;
    impl_->boldToProperty_[boldProp] = property;
    property->addSubProperty(boldProp);

    auto italicProp = impl_->boolPropertyManager_->addProperty();
    italicProp->setPropertyName(tr("Italic"));
    impl_->boolPropertyManager_->setValue(italicProp, val.italic());
    impl_->propertyToItalic_[property] = italicProp;
    impl_->italicToProperty_[italicProp] = property;
    property->addSubProperty(italicProp);

    auto underlineProp = impl_->boolPropertyManager_->addProperty();
    underlineProp->setPropertyName(tr("Underline"));
    impl_->boolPropertyManager_->setValue(underlineProp, val.underline());
    impl_->propertyToUnderline_[property] = underlineProp;
    impl_->underlineToProperty_[underlineProp] = property;
    property->addSubProperty(underlineProp);

    auto strikeOutProp = impl_->boolPropertyManager_->addProperty();
    strikeOutProp->setPropertyName(tr("Strikeout"));
    impl_->boolPropertyManager_->setValue(strikeOutProp, val.strikeOut());
    impl_->propertyToStrikeOut_[property] = strikeOutProp;
    impl_->strikeOutToProperty_[strikeOutProp] = property;
    property->addSubProperty(strikeOutProp);

    auto kerningProp = impl_->boolPropertyManager_->addProperty();
    kerningProp->setPropertyName(tr("Kerning"));
    impl_->boolPropertyManager_->setValue(kerningProp, val.kerning());
    impl_->propertyToKerning_[property] = kerningProp;
    impl_->kerningToProperty_[kerningProp] = property;
    property->addSubProperty(kerningProp);
}

void FontPropertyManager::uninitializeProperty(Property* property)
{
    auto familyProp = impl_->propertyToFamily_[property];
    if (familyProp)
    {
        impl_->familyToProperty_.remove(familyProp);
        delete familyProp;
    }
    impl_->propertyToFamily_.remove(property);

    auto pointSizeProp = impl_->propertyToPointSize_[property];
    if (pointSizeProp)
    {
        impl_->pointSizeToProperty_.remove(pointSizeProp);
        delete pointSizeProp;
    }
    impl_->propertyToPointSize_.remove(property);

    auto boldProp = impl_->propertyToBold_[property];
    if (boldProp)
    {
        impl_->boldToProperty_.remove(boldProp);
        delete boldProp;
    }
    impl_->propertyToBold_.remove(property);

    auto italicProp = impl_->propertyToItalic_[property];
    if (italicProp)
    {
        impl_->italicToProperty_.remove(italicProp);
        delete italicProp;
    }
    impl_->propertyToItalic_.remove(property);

    auto underlineProp = impl_->propertyToUnderline_[property];
    if (underlineProp)
    {
        impl_->underlineToProperty_.remove(underlineProp);
        delete underlineProp;
    }
    impl_->propertyToUnderline_.remove(property);

    auto strikeOutProp = impl_->propertyToStrikeOut_[property];
    if (strikeOutProp)
    {
        impl_->strikeOutToProperty_.remove(strikeOutProp);
        delete strikeOutProp;
    }
    impl_->propertyToStrikeOut_.remove(property);

    auto kerningProp = impl_->propertyToKerning_[property];
    if (kerningProp)
    {
        impl_->kerningToProperty_.remove(kerningProp);
        delete kerningProp;
    }
    impl_->propertyToKerning_.remove(property);

    impl_->values_.remove(property);
}
