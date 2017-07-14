/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <QtCore/QMap>

class QLocale;
class Property;
class EnumPropertyManager;
class LocalePropertyManager;
class LocalePropertyManagerPrivate
{
    LocalePropertyManager* localePropertyManagerPtr_;
    friend class LocalePropertyManager;
public:

    LocalePropertyManagerPrivate();
    LocalePropertyManagerPrivate(const LocalePropertyManagerPrivate&) = delete;
    LocalePropertyManagerPrivate operator=(const LocalePropertyManagerPrivate&) = delete;

    void slotEnumChanged(Property* property, int value);
    void slotPropertyDestroyed(Property* property);

    using PropertyValueMap = QMap<const Property*, QLocale>;
    PropertyValueMap values_;

    EnumPropertyManager* enumPropertyManager_;
    QMap<const Property*, Property*> propertyToLanguage_;
    QMap<const Property*, Property*> propertyToCountry_;

    QMap<const Property*, Property*> languageToProperty_;
    QMap<const Property*, Property*> countryToProperty_;
};
