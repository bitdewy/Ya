/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "fontpropertymanager_p.h"
#include "propertymanager/enumpropertymanager.h"
#include "propertymanager/fontpropertymanager.h"
#include <QtCore/QTimer>

FontPropertyManagerPrivate::FontPropertyManagerPrivate() :
    settingValue_(false),
    fontDatabaseChangeTimer_(nullptr)
{
}

void FontPropertyManagerPrivate::slotIntChanged(Property* property, int value)
{
    if (settingValue_)
    {
        return;
    }
    if (auto prop = pointSizeToProperty_.value(property, 0))
    {
        auto f = values_[prop];
        f.setPointSize(value);
        fontPropertyManagerPtr_->setValue(prop, f);
    }
}

void FontPropertyManagerPrivate::slotEnumChanged(Property* property, int value)
{
    if (settingValue_)
    {
        return;
    }
    if (auto prop = familyToProperty_.value(property, 0))
    {
        auto f = values_[prop];
        f.setFamily(familyNames_.at(value));
        fontPropertyManagerPtr_->setValue(prop, f);
    }
}

void FontPropertyManagerPrivate::slotBoolChanged(Property* property, bool value)
{
    if (settingValue_)
    {
        return;
    }
    if (auto prop = boldToProperty_.value(property, 0))
    {
        auto f = values_[prop];
        f.setBold(value);
        fontPropertyManagerPtr_->setValue(prop, f);
    }
    else if (auto prop = italicToProperty_.value(property, 0))
    {
        auto f = values_[prop];
        f.setItalic(value);
        fontPropertyManagerPtr_->setValue(prop, f);
    }
    else if (auto prop = underlineToProperty_.value(property, 0))
    {
        auto f = values_[prop];
        f.setUnderline(value);
        fontPropertyManagerPtr_->setValue(prop, f);
    }
    else if (auto prop = strikeOutToProperty_.value(property, 0))
    {
        auto f = values_[prop];
        f.setStrikeOut(value);
        fontPropertyManagerPtr_->setValue(prop, f);
    }
    else if (auto prop = kerningToProperty_.value(property, 0))
    {
        auto f = values_[prop];
        f.setKerning(value);
        fontPropertyManagerPtr_->setValue(prop, f);
    }
}

void FontPropertyManagerPrivate::slotPropertyDestroyed(Property* property)
{
    if (auto pointProp = pointSizeToProperty_.value(property, 0))
    {
        propertyToPointSize_[pointProp] = 0;
        pointSizeToProperty_.remove(property);
    }
    else if (auto pointProp = familyToProperty_.value(property, 0))
    {
        propertyToFamily_[pointProp] = 0;
        familyToProperty_.remove(property);
    }
    else if (auto pointProp = boldToProperty_.value(property, 0))
    {
        propertyToBold_[pointProp] = 0;
        boldToProperty_.remove(property);
    }
    else if (auto pointProp = italicToProperty_.value(property, 0))
    {
        propertyToItalic_[pointProp] = 0;
        italicToProperty_.remove(property);
    }
    else if (auto pointProp = underlineToProperty_.value(property, 0))
    {
        propertyToUnderline_[pointProp] = 0;
        underlineToProperty_.remove(property);
    }
    else if (auto pointProp = strikeOutToProperty_.value(property, 0))
    {
        propertyToStrikeOut_[pointProp] = 0;
        strikeOutToProperty_.remove(property);
    }
    else if (auto pointProp = kerningToProperty_.value(property, 0))
    {
        propertyToKerning_[pointProp] = 0;
        kerningToProperty_.remove(property);
    }
}

void  FontPropertyManagerPrivate::slotFontDatabaseChanged()
{
    if (!fontDatabaseChangeTimer_)
    {
        fontDatabaseChangeTimer_ = new QTimer(fontPropertyManagerPtr_);
        fontDatabaseChangeTimer_->setInterval(0);
        fontDatabaseChangeTimer_->setSingleShot(true);
        QObject::connect(fontDatabaseChangeTimer_, &QTimer::timeout,
            fontPropertyManagerPtr_, [this]() { slotFontDatabaseDelayedChange(); });
    }
    if (!fontDatabaseChangeTimer_->isActive())
    {
        fontDatabaseChangeTimer_->start();
    }
}

void FontPropertyManagerPrivate::slotFontDatabaseDelayedChange()
{
    const auto oldFamilies = familyNames_;
    familyNames_ = fontDatabase()->families();

    if (!propertyToFamily_.empty())
    {
        auto cend = propertyToFamily_.constEnd();
        for (auto it = propertyToFamily_.constBegin(); it != cend; ++it)
        {
            auto familyProp = it.value();
            const auto oldIdx = enumPropertyManager_->value(familyProp);
            auto newIdx = familyNames_.indexOf(oldFamilies.at(oldIdx));
            if (newIdx < 0)
            {
                newIdx = 0;
            }
            enumPropertyManager_->setEnumNames(familyProp, familyNames_);
            enumPropertyManager_->setValue(familyProp, newIdx);
        }
    }
}
