/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <QtCore/QLocale>
#include <QtCore/QMetaEnum>
#include <QtWidgets/QSizePolicy>

class MetaEnumProvider
{
public:
    MetaEnumProvider();

    QStringList policyEnumNames() const;
    QStringList languageEnumNames() const;
    QStringList countryEnumNames(QLocale::Language language) const;

    QSizePolicy::Policy indexToSizePolicy(int index) const;
    int sizePolicyToIndex(QSizePolicy::Policy policy) const;

    void indexToLocale(int languageIndex, int countryIndex, QLocale::Language* language, QLocale::Country* country) const;
    void localeToIndex(QLocale::Language language, QLocale::Country country, int* languageIndex, int* countryIndex) const;

private:
    void initLocale();

    QStringList policyEnumNames_;
    QStringList languageEnumNames_;
    QMap<QLocale::Language, QStringList> countryEnumNames_;
    QMap<int, QLocale::Language> indexToLanguage_;
    QMap<QLocale::Language, int> languageToIndex_;
    QMap<int, QMap<int, QLocale::Country>> indexToCountry_;
    QMap<QLocale::Language, QMap<QLocale::Country, int>> countryToIndex_;
    QMetaEnum policyEnum_;
};

Q_GLOBAL_STATIC(MetaEnumProvider, metaEnumProvider)
