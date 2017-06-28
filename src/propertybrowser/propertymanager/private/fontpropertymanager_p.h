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
#include <QtGui/QFontDatabase>

class QTimer;
class Property;
class IntPropertyManager;
class EnumPropertyManager;
class BoolPropertyManager;
class FontPropertyManager;
class FontPropertyManagerPrivate
{
    FontPropertyManager* fontPropertyManagerPtr_;
    friend class FontPropertyManager;
public:

    FontPropertyManagerPrivate();
    FontPropertyManagerPrivate(const FontPropertyManagerPrivate&) = delete;
    FontPropertyManagerPrivate& operator=(const FontPropertyManagerPrivate&) = delete;

    void slotIntChanged(Property* property, int value);
    void slotEnumChanged(Property* property, int value);
    void slotBoolChanged(Property* property, bool value);
    void slotPropertyDestroyed(Property* property);
    void slotFontDatabaseChanged();
    void slotFontDatabaseDelayedChange();

    QStringList familyNames_;

    using PropertyValueMap = QMap<const Property*, QFont>;
    PropertyValueMap values_;

    IntPropertyManager* intPropertyManager_;
    EnumPropertyManager* enumPropertyManager_;
    BoolPropertyManager* boolPropertyManager_;

    QMap<const Property*, Property*> propertyToFamily_;
    QMap<const Property*, Property*> propertyToPointSize_;
    QMap<const Property*, Property*> propertyToBold_;
    QMap<const Property*, Property*> propertyToItalic_;
    QMap<const Property*, Property*> propertyToUnderline_;
    QMap<const Property*, Property*> propertyToStrikeOut_;
    QMap<const Property*, Property*> propertyToKerning_;

    QMap<const Property*, Property*> familyToProperty_;
    QMap<const Property*, Property*> pointSizeToProperty_;
    QMap<const Property*, Property*> boldToProperty_;
    QMap<const Property*, Property*> italicToProperty_;
    QMap<const Property*, Property*> underlineToProperty_;
    QMap<const Property*, Property*> strikeOutToProperty_;
    QMap<const Property*, Property*> kerningToProperty_;

    bool settingValue_;
    QTimer* fontDatabaseChangeTimer_;
};

Q_GLOBAL_STATIC(QFontDatabase, fontDatabase)
