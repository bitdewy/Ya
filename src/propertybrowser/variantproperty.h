/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include "property.h"
#include <QtCore/QVariant>
#include <QtGui/QIcon>

using IconMap = QMap<int, QIcon>;

class VariantPropertyManager;
class VariantProperty : public Property
{
public:
    ~VariantProperty();
    QVariant value() const;
    QVariant attributeValue(const QString& attribute) const;
    int valueType() const;
    int propertyType() const;

    void setValue(const QVariant& value);
    void setAttribute(const QString& attribute, const QVariant& value);
protected:
    VariantProperty(VariantPropertyManager* manager);
private:
    friend class VariantPropertyManager;
    QScopedPointer<class VariantPropertyPrivate> impl_;
};

Q_DECLARE_METATYPE(QIcon)
Q_DECLARE_METATYPE(IconMap)
