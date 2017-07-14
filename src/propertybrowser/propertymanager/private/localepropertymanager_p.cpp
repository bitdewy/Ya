/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "localepropertymanager_p.h"
#include "propertymanager/metaenumprovider.h"
#include "propertymanager/localepropertymanager.h"
#include "propertymanager/enumpropertymanager.h"
#include <QtCore/QLocale>

LocalePropertyManagerPrivate::LocalePropertyManagerPrivate()
{
}

void LocalePropertyManagerPrivate::slotEnumChanged(Property* property, int value)
{
    if (auto prop = languageToProperty_.value(property, nullptr))
    {
        const auto loc = values_[prop];
        auto newLanguage = loc.language();
        auto newCountry = loc.country();
        metaEnumProvider()->indexToLocale(value, 0, &newLanguage, 0);
        QLocale newLoc(newLanguage, newCountry);
        localePropertyManagerPtr_->setValue(prop, newLoc);
    }
    else if (auto prop = countryToProperty_.value(property, nullptr))
    {
        const auto loc = values_[prop];
        auto newLanguage = loc.language();
        auto newCountry = loc.country();
        metaEnumProvider()->indexToLocale(enumPropertyManager_->value(propertyToLanguage_.value(prop)), value, &newLanguage, &newCountry);
        QLocale newLoc(newLanguage, newCountry);
        localePropertyManagerPtr_->setValue(prop, newLoc);
    }
}

void LocalePropertyManagerPrivate::slotPropertyDestroyed(Property* property)
{
    if (auto subProp = languageToProperty_.value(property, nullptr))
    {
        propertyToLanguage_[subProp] = nullptr;
        languageToProperty_.remove(property);
    }
    else if (auto subProp = countryToProperty_.value(property, nullptr))
    {
        propertyToCountry_[subProp] = nullptr;
        countryToProperty_.remove(property);
    }
}
