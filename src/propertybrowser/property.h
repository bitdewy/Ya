/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <QtCore/QList>
#include <QtGui/QIcon>

class AbstractPropertyManager;
class PropertyPrivate;
class Property
{
public:
    virtual ~Property();

    QList<Property*> subProperties() const;

    AbstractPropertyManager* propertyManager() const;

    QString toolTip() const;
    QString descriptionToolTip() const;
    QString statusTip() const;
    QString whatsThis() const;
    QString propertyName() const;
    bool isEnabled() const;
    bool isModified() const;

    bool hasValue() const;
    QIcon valueIcon() const;
    QString valueText() const;

    void setToolTip(const QString& text);
    void setDescriptionToolTip(const QString& text);
    void setStatusTip(const QString& text);
    void setWhatsThis(const QString& text);
    void setPropertyName(const QString& text);
    void setEnabled(bool enable);
    void setModified(bool modified);

    void addSubProperty(Property* property);
    void insertSubProperty(Property* property, Property* after);
    void removeSubProperty(Property* property);

protected:
    explicit Property(AbstractPropertyManager* manager);
    void propertyChanged();

private:
    friend class AbstractPropertyManager;
    QScopedPointer<PropertyPrivate> impl_;
};
