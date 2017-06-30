/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include "abstracteditorfactory.h"
#include <QtCore/QList>
#include <QtWidgets/QWidget>

class Property;
class BrowserItem;
class AbstractPropertyBrowserPrivate;
class AbstractPropertyBrowser : public QWidget
{
    Q_OBJECT
public:

    explicit AbstractPropertyBrowser(QWidget* parent = nullptr);
    ~AbstractPropertyBrowser();

    QList<Property*> properties() const;
    QList<BrowserItem*> items(Property* property) const;
    BrowserItem* topLevelItem(Property* property) const;
    QList<BrowserItem*> topLevelItems() const;
    void clear();

    template <class PropertyManager>
    void setFactoryForManager(PropertyManager* manager,
                    AbstractEditorFactory<PropertyManager>* factory)
    {
        auto abstractManager = manager;
        auto abstractFactory = factory;

        if (addFactory(abstractManager, abstractFactory))
        {
            factory->addPropertyManager(manager);
        }
    }

    void unsetFactoryForManager(AbstractPropertyManager* manager);

    BrowserItem* currentItem() const;
    void setCurrentItem(BrowserItem*);

Q_SIGNALS:
    void currentItemChanged(BrowserItem*);

public:

    BrowserItem* addProperty(Property* property);
    BrowserItem* insertProperty(Property* property, Property* afterProperty);
    void removeProperty(Property* property);

protected:

    virtual void itemInserted(BrowserItem* item, BrowserItem* afterItem) = 0;
    virtual void itemRemoved(BrowserItem* item) = 0;
    virtual void itemChanged(BrowserItem* item) = 0;

    virtual QWidget* createEditor(Property* property, QWidget* parent);
private:
    void slotPropertyInserted(Property* property,
        Property* parentProperty, Property* afterProperty);
    void slotPropertyRemoved(Property* property, Property* parentProperty);
    void slotPropertyDestroyed(Property* property);
    void slotPropertyDataChanged(Property* property);

    bool addFactory(AbstractPropertyManager* abstractManager,
        AbstractEditorFactoryBase* abstractFactory);

    QScopedPointer<AbstractPropertyBrowserPrivate> impl_;
    friend class AbstractPropertyBrowserPrivate;
};
