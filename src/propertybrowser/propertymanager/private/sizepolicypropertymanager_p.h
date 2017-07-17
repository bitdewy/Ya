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

class QSizePolicy;
class Property;
class IntPropertyManager;
class EnumPropertyManager;
class SizePolicyPropertyManager;
class SizePolicyPropertyManagerPrivate
{
    SizePolicyPropertyManager* sizePolicyPropertyManager_;
    friend class SizePolicyPropertyManager;
public:

    SizePolicyPropertyManagerPrivate();
    SizePolicyPropertyManagerPrivate(const SizePolicyPropertyManagerPrivate&) = delete;
    SizePolicyPropertyManagerPrivate operator=(const SizePolicyPropertyManagerPrivate&) = delete;

    void slotIntChanged(Property* property, int value);
    void slotEnumChanged(Property* property, int value);
    void slotPropertyDestroyed(Property* property);

    using PropertyValueMap = QMap<const Property*, QSizePolicy>;
    PropertyValueMap values_;

    IntPropertyManager* intPropertyManager_;
    EnumPropertyManager* enumPropertyManager_;

    QMap<const Property*, Property*> propertyToHPolicy_;
    QMap<const Property*, Property*> propertyToVPolicy_;
    QMap<const Property*, Property*> propertyToHStretch_;
    QMap<const Property*, Property*> propertyToVStretch_;

    QMap<const Property*, Property*> hPolicyToProperty_;
    QMap<const Property*, Property*> vPolicyToProperty_;
    QMap<const Property*, Property*> hStretchToProperty_;
    QMap<const Property*, Property*> vStretchToProperty_;
};
