/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include "abstracteditorfactorybase.h"
#include "property.h"

template <class PropertyManager>
class AbstractEditorFactory : public AbstractEditorFactoryBase
{
public:
    explicit AbstractEditorFactory(QObject* parent) :
        AbstractEditorFactoryBase(parent)
    {

    }

    QWidget* createEditor(Property* property, QWidget* parent)
    {
        for (const auto& manager : qAsConst(managers_))
        {
            if (manager == property->propertyManager())
            {
                return createEditor(manager, property, parent);
            }
        }
        return 0;
    }

    void addPropertyManager(PropertyManager* manager)
    {
        if (managers_.contains(manager))
        {
            return;
        }
        managers_.insert(manager);
        connectPropertyManager(manager);
        connect(manager, &PropertyManager::destroyed,
                this, &AbstractEditorFactory::managerDestroyed);
    }

    void removePropertyManager(PropertyManager* manager)
    {
        if (!managers_.contains(manager))
        {
            return;
        }
        disconnect(manager, &PropertyManager::destroyed,
                    this, &AbstractEditorFactory::managerDestroyed);
        disconnectPropertyManager(manager);
        managers_.remove(manager);
    }

    QSet<PropertyManager*> propertyManagers() const
    {
        return managers_;
    }

    PropertyManager *propertyManager(Property* property) const
    {
        auto manager = property->propertyManager();
        for (const auto& m : qAsConst(managers_))
        {
            if (m == manager)
            {
                return m;
            }
        }
        return 0;
    }

protected:
    virtual void connectPropertyManager(PropertyManager* manager) = 0;
    virtual QWidget* createEditor(PropertyManager* manager, Property* property,
                QWidget* parent) = 0;
    virtual void disconnectPropertyManager(PropertyManager* manager) = 0;
    void managerDestroyed(QObject* manager)
    {
        for (const auto& m : qAsConst(managers_))
        {
            if (m == manager)
            {
                managers_.remove(m);
                return;
            }
        }
    }
private:
    void breakConnection(AbstractPropertyManager* manager)
    {
        for (const auto& m : qAsConst(managers_))
        {
            if (m == manager)
            {
                removePropertyManager(m);
                return;
            }
        }
    }
private:
    QSet<PropertyManager*> managers_;
};
