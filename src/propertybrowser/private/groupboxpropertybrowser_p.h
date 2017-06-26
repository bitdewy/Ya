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

class QFrame;
class QLabel;
class QGroupBox;
class QGridLayout;
class QWidget;
class BrowserItem;
class Property;
class GroupBoxPropertyBrowser;
class GroupBoxPropertyBrowserPrivate
{
    GroupBoxPropertyBrowser* groupBoxPropertyBrowserPtr_;
    friend class GroupBoxPropertyBrowser;
public:

    void init(QWidget* parent);

    void propertyInserted(BrowserItem* index, BrowserItem* afterIndex);
    void propertyRemoved(BrowserItem* index);
    void propertyChanged(BrowserItem* index);
    QWidget* createEditor(Property* property, QWidget* parent) const;

    void slotEditorDestroyed();
    void slotUpdate();

    struct WidgetItem
    {
        QWidget* widget{nullptr};
        QLabel* label{nullptr};
        QLabel* widgetLabel{nullptr};
        QGroupBox* groupBox{nullptr};
        QGridLayout* layout{nullptr};
        QFrame* line{nullptr};
        WidgetItem* parent{nullptr};
        QList<WidgetItem*> children;
    };
private:
    void updateLater();
    void updateItem(WidgetItem* item);
    void insertRow(QGridLayout* layout, int row) const;
    void removeRow(QGridLayout* layout, int row) const;

    bool hasHeader(WidgetItem* item) const;

    QMap<BrowserItem*, WidgetItem*> indexToItem_;
    QMap<WidgetItem*, BrowserItem*> itemToIndex_;
    QMap<QWidget*, WidgetItem*> widgetToItem_;
    QGridLayout* mainLayout_;
    QList<WidgetItem*> children_;
    QList<WidgetItem*> recreateQueue_;
};
