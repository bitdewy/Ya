/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include "extension/abstractextensionmanager.h"

class DynamicPropertySheetExtension
{
public:
    virtual ~DynamicPropertySheetExtension() {}

    virtual bool dynamicPropertiesAllowed() const = 0;
    virtual int addDynamicProperty(const QString& propertyName, const QVariant& value) = 0;
    virtual bool removeDynamicProperty(int index) = 0;
    virtual bool isDynamicProperty(int index) const = 0;
    virtual bool canAddDynamicProperty(const QString& propertyName) const = 0;
};

Y_DECLARE_EXTENSION_INTERFACE(DynamicPropertySheetExtension, "bitdewy.Ya.DynamicPropertySheet")
