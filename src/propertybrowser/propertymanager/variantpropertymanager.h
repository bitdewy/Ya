/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include "abstractpropertymanager.h"

class VariantProperty;
class VariantPropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    explicit VariantPropertyManager(QObject* parent = nullptr);
    ~VariantPropertyManager();

    virtual VariantProperty* addProperty(int propertyType, const QString& name = QString());

    int propertyType(const Property* property) const;
    int valueType(const Property* property) const;
    VariantProperty* variantProperty(const Property* property) const;

    virtual bool isPropertyTypeSupported(int propertyType) const;
    virtual int valueType(int propertyType) const;
    virtual QStringList attributes(int propertyType) const;
    virtual int attributeType(int propertyType, const QString& attribute) const;

    virtual QVariant value(const Property* property) const;
    virtual QVariant attributeValue(const Property* property, const QString& attribute) const;

    static int enumTypeId();
    static int flagTypeId();
    static int groupTypeId();
    static int iconMapTypeId();

public Q_SLOTS:
    virtual void setValue(Property* property, const QVariant& val);
    virtual void setAttribute(Property* property,
        const QString& attribute, const QVariant& value);

Q_SIGNALS:
    void valueChanged(Property* property, const QVariant& val);
    void attributeChanged(Property* property,
        const QString& attribute, const QVariant& val);

protected:
    virtual bool hasValue(const Property* property) const;
    QString valueText(const Property* property) const;
    QIcon valueIcon(const Property* property) const;
    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
    virtual Property* createProperty();

private:
    QScopedPointer<class VariantPropertyManagerPrivate> impl_;
    friend class VariantPropertyManagerPrivate;
};
