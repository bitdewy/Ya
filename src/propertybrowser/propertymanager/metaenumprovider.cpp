/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "metaenumprovider.h"
#include "metaenumwrapper.h"

static QList<QLocale::Country> sortCountries(const QList<QLocale::Country>& countries)
{
    QMultiMap<QString, QLocale::Country> nameToCountry;
    for (const auto& country : countries)
    {
        nameToCountry.insert(QLocale::countryToString(country), country);
    }
    return nameToCountry.values();
}

void MetaEnumProvider::initLocale()
{
    QMultiMap<QString, QLocale::Language> nameToLanguage;
    for (int l = QLocale::C, last = QLocale::LastLanguage; l <= last; ++l)
    {
        const auto language = static_cast<QLocale::Language>(l);
        QLocale locale(language);
        if (locale.language() == language)
        {
            nameToLanguage.insert(QLocale::languageToString(language), language);
        }
    }

    const auto system = QLocale::system();
    if (!nameToLanguage.contains(QLocale::languageToString(system.language())))
    {
        nameToLanguage.insert(QLocale::languageToString(system.language()), system.language());
    }

    const auto languages = nameToLanguage.values();
    for (const auto& language : languages)
    {
        QList<QLocale::Country> countries;
        countries = QLocale::countriesForLanguage(language);
        if (countries.isEmpty() && language == system.language())
        {
            countries << system.country();
        }
        if (!countries.isEmpty() && !languageToIndex_.contains(language))
        {
            countries = sortCountries(countries);
            auto langIdx = languageEnumNames_.count();
            indexToLanguage_[langIdx] = language;
            languageToIndex_[language] = langIdx;
            QStringList countryNames;
            auto countryIdx = 0;
            for (const auto& country : qAsConst(countries))
            {
                countryNames << QLocale::countryToString(country);
                indexToCountry_[langIdx][countryIdx] = country;
                countryToIndex_[language][country] = countryIdx;
                ++countryIdx;
            }
            languageEnumNames_ << QLocale::languageToString(language);
            countryEnumNames_[language] = countryNames;
        }
    }
}

MetaEnumProvider::MetaEnumProvider()
{
    QMetaProperty p = MetaEnumWrapper::staticMetaObject.property(
        MetaEnumWrapper::staticMetaObject.propertyOffset() + 0);
    policyEnum_ = p.enumerator();
    const auto keyCount = policyEnum_.keyCount();
    for (auto i = 0; i < keyCount; i++)
    {
        policyEnumNames_ << QLatin1String(policyEnum_.key(i));
    }
    initLocale();
}

QStringList MetaEnumProvider::policyEnumNames() const
{
    return policyEnumNames_;
}

QStringList MetaEnumProvider::languageEnumNames() const
{
    return languageEnumNames_;
}

QStringList MetaEnumProvider::countryEnumNames(QLocale::Language language) const
{
    return countryEnumNames_.value(language);
}

QSizePolicy::Policy MetaEnumProvider::indexToSizePolicy(int index) const
{
    return static_cast<QSizePolicy::Policy>(policyEnum_.value(index));
}

int MetaEnumProvider::sizePolicyToIndex(QSizePolicy::Policy policy) const
{
    const auto keyCount = policyEnum_.keyCount();
    for (auto i = 0; i < keyCount; i++)
    {
        if (indexToSizePolicy(i) == policy)
        {
            return i;
        }
    }
    return -1;
}

void MetaEnumProvider::indexToLocale(int languageIndex, int countryIndex, QLocale::Language* language, QLocale::Country* country) const
{
    auto l = QLocale::C;
    auto c = QLocale::AnyCountry;
    if (indexToLanguage_.contains(languageIndex))
    {
        l = indexToLanguage_[languageIndex];
        if (indexToCountry_.contains(languageIndex) && indexToCountry_[languageIndex].contains(countryIndex))
        {
            c = indexToCountry_[languageIndex][countryIndex];
        }
    }
    if (language)
    {
        *language = l;
    }
    if (country)
    {
        *country = c;
    }
}

void MetaEnumProvider::localeToIndex(QLocale::Language language, QLocale::Country country, int *languageIndex, int *countryIndex) const
{
    auto l = -1;
    auto c = -1;
    if (languageToIndex_.contains(language))
    {
        l = languageToIndex_[language];
        if (countryToIndex_.contains(language) && countryToIndex_[language].contains(country))
        {
            c = countryToIndex_[language][country];
        }
    }

    if (languageIndex)
    {
        *languageIndex = l;
    }
    if (countryIndex)
    {
        *countryIndex = c;
    }
}
