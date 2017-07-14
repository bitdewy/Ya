/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "localepropertymanager.h"
#include "metaenumprovider.h"
#include "property.h"
#include "propertymanager/enumpropertymanager.h"
#include "private/localepropertymanager_p.h"
#include <QtCore/QLocale>

LocalePropertyManager::LocalePropertyManager(QObject* parent) :
    AbstractPropertyManager(parent),
    impl_(new LocalePropertyManagerPrivate)
{
    impl_->localePropertyManagerPtr_ = this;

    impl_->enumPropertyManager_ = new EnumPropertyManager(this);
    connect(impl_->enumPropertyManager_, &EnumPropertyManager::valueChanged,
        this, [this](Property* property, int value) { impl_->slotEnumChanged(property, value); });

    connect(impl_->enumPropertyManager_, &EnumPropertyManager::propertyDestroyed,
        this, [this](Property* property) { impl_->slotPropertyDestroyed(property); });
}

LocalePropertyManager::~LocalePropertyManager()
{
    clear();
}

EnumPropertyManager *LocalePropertyManager::subEnumPropertyManager() const
{
    return impl_->enumPropertyManager_;
}

QLocale LocalePropertyManager::value(const Property* property) const
{
    return impl_->values_.value(property, QLocale());
}

QString LocalePropertyManager::valueText(const Property* property) const
{
    const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QString();
    }
    const auto loc = it.value();

    auto langIdx = 0;
    auto countryIdx = 0;
    const auto me = metaEnumProvider();
    me->localeToIndex(loc.language(), loc.country(), &langIdx, &countryIdx);
    if (langIdx < 0)
    {
        qWarning("QtLocalePropertyManager::valueText: Unknown language %d", loc.language());
        return tr("<Invalid>");
    }
    const auto languageName = me->languageEnumNames().at(langIdx);
    if (countryIdx < 0)
    {
        qWarning("QtLocalePropertyManager::valueText: Unknown country %d for %s", loc.country(), qPrintable(languageName));
        return languageName;
    }
    const auto countryName = me->countryEnumNames(loc.language()).at(countryIdx);
    return tr("%1, %2").arg(languageName, countryName);
}

void LocalePropertyManager::setValue(Property* property, const QLocale& val)
{
    const auto it = impl_->values_.find(property);
    if (it == impl_->values_.end())
    {
        return;
    }
    const auto loc = it.value();
    if (loc == val)
    {
        return;
    }
    it.value() = val;

    auto langIdx = 0;
    auto countryIdx = 0;
    metaEnumProvider()->localeToIndex(val.language(), val.country(), &langIdx, &countryIdx);
    if (loc.language() != val.language())
    {
        impl_->enumPropertyManager_->setValue(impl_->propertyToLanguage_.value(property), langIdx);
        impl_->enumPropertyManager_->setEnumNames(impl_->propertyToCountry_.value(property),
                    metaEnumProvider()->countryEnumNames(val.language()));
    }
    impl_->enumPropertyManager_->setValue(impl_->propertyToCountry_.value(property), countryIdx);

    emit propertyChanged(property);
    emit valueChanged(property, val);
}

void LocalePropertyManager::initializeProperty(Property* property)
{
    QLocale val;
    impl_->values_[property] = val;

    auto langIdx = 0;
    auto countryIdx = 0;
    metaEnumProvider()->localeToIndex(val.language(), val.country(), &langIdx, &countryIdx);

    auto languageProp = impl_->enumPropertyManager_->addProperty();
    languageProp->setPropertyName(tr("Language"));
    impl_->enumPropertyManager_->setEnumNames(languageProp, metaEnumProvider()->languageEnumNames());
    impl_->enumPropertyManager_->setValue(languageProp, langIdx);
    impl_->propertyToLanguage_[property] = languageProp;
    impl_->languageToProperty_[languageProp] = property;
    property->addSubProperty(languageProp);

    auto countryProp = impl_->enumPropertyManager_->addProperty();
    countryProp->setPropertyName(tr("Country"));
    impl_->enumPropertyManager_->setEnumNames(countryProp, metaEnumProvider()->countryEnumNames(val.language()));
    impl_->enumPropertyManager_->setValue(countryProp, countryIdx);
    impl_->propertyToCountry_[property] = countryProp;
    impl_->countryToProperty_[countryProp] = property;
    property->addSubProperty(countryProp);
}

void LocalePropertyManager::uninitializeProperty(Property* property)
{
    auto languageProp = impl_->propertyToLanguage_[property];
    if (languageProp)
    {
        impl_->languageToProperty_.remove(languageProp);
        delete languageProp;
    }
    impl_->propertyToLanguage_.remove(property);

    auto countryProp = impl_->propertyToCountry_[property];
    if (countryProp)
    {
        impl_->countryToProperty_.remove(countryProp);
        delete countryProp;
    }
    impl_->propertyToCountry_.remove(property);

    impl_->values_.remove(property);
}
