/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <QtCore/QObject>

class Property;
class AbstractPropertyManagerPrivate;
class AbstractPropertyManager : public QObject
{
    Q_OBJECT
public:

    explicit AbstractPropertyManager(QObject* parent = nullptr);
    ~AbstractPropertyManager();

    QSet<Property*> properties() const;
    void clear() const;

    Property* addProperty(const QString& name = QString());

Q_SIGNALS:
    void propertyInserted(Property* property, Property* parent, Property* after);
    void propertyChanged(Property* property);
    void propertyRemoved(Property* property, Property* parent);
    void propertyDestroyed(Property* property);

protected:
    virtual bool hasValue(const Property* property) const;
    virtual QIcon valueIcon(const Property* property) const;
    virtual QString valueText(const Property* property) const;
    virtual void initializeProperty(Property* property) = 0;
    virtual void uninitializeProperty(Property* property);
    virtual Property* createProperty();

private:
    friend class Property;
    friend class AbstractPropertyManagerPrivate;
    QScopedPointer<AbstractPropertyManagerPrivate> impl_;
};
