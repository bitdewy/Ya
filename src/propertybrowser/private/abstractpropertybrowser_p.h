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
#include <QtCore/QMap>

class Property;
class BrowserItem;
class AbstractPropertyManager;
class AbstractPropertyBrowser;
class AbstractEditorFactoryBase;
class AbstractPropertyBrowserPrivate
{
    AbstractPropertyBrowser* abstractPropertyBrowserPtr_;
    friend class AbstractPropertyBrowser;
public:
    AbstractPropertyBrowserPrivate();
    AbstractPropertyBrowserPrivate(const AbstractPropertyBrowserPrivate&) = delete;
    AbstractPropertyBrowserPrivate& operator=(const AbstractPropertyBrowserPrivate&) = delete;

    void insertSubTree(Property* property,
            Property* parentProperty);
    void removeSubTree(Property* property,
            Property* parentProperty);
    void createBrowserIndexes(Property* property, Property* parentProperty, Property* afterProperty);
    void removeBrowserIndexes(Property* property, Property* parentProperty);
    BrowserItem* createBrowserIndex(Property* property, BrowserItem* parentIndex, BrowserItem* afterIndex);
    void removeBrowserIndex(BrowserItem* index);
    void clearIndex(BrowserItem* index);

    void slotPropertyInserted(Property* property,
            Property* parentProperty, Property* afterProperty);
    void slotPropertyRemoved(Property* property, Property* parentProperty);
    void slotPropertyDestroyed(Property* property);
    void slotPropertyDataChanged(Property* property);

    QList<Property*> subItems_;
    QMap<AbstractPropertyManager*, QList<Property*>> managerToProperties_;
    QMap<Property*, QList<Property*>> propertyToParents_;

    QMap<Property*, BrowserItem*> topLevelPropertyToIndex_;
    QList<BrowserItem*> topLevelIndexes_;
    QMap<Property*, QList<BrowserItem*>> propertyToIndexes_;

    BrowserItem* currentItem_;
};


using Map1 = QMap<AbstractPropertyBrowser*, QMap<AbstractPropertyManager*,
                            AbstractEditorFactoryBase*>>;
using Map2 = QMap<AbstractPropertyManager*, QMap<AbstractEditorFactoryBase*,
                            QList<AbstractPropertyBrowser*>>>;

Q_GLOBAL_STATIC(Map1, viewToManagerToFactory)
Q_GLOBAL_STATIC(Map2, managerToFactoryToViews)
