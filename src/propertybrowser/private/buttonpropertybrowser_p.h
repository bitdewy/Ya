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

class QGridLayout;
class QLabel;
class QObject;
class QToolButton;
class QWidget;
class Property;
class BrowserItem;
class ButtonPropertyBrowser;
class ButtonPropertyBrowserPrivate
{
    friend class ButtonPropertyBrowser;
    ButtonPropertyBrowser* buttonPropertyBrowserPtr_;
public:
    ButtonPropertyBrowserPrivate();
    ~ButtonPropertyBrowserPrivate();
    ButtonPropertyBrowserPrivate(const ButtonPropertyBrowserPrivate&) = delete;
    ButtonPropertyBrowserPrivate& operator=(const ButtonPropertyBrowserPrivate&) = delete;

    void init(QWidget* parent);

    void propertyInserted(BrowserItem* index, BrowserItem* afterIndex);
    void propertyRemoved(BrowserItem *index);
    void propertyChanged(BrowserItem *index);
    QWidget* createEditor(Property* property, QWidget* parent) const;

    void slotEditorDestroyed();
    void slotUpdate();
    void slotToggled(bool checked);

    struct WidgetItem
    {
        QWidget* widget{nullptr};
        QLabel* label{nullptr};
        QLabel* widgetLabel{nullptr};
        QToolButton* button{nullptr};
        QWidget* container{nullptr};
        QGridLayout* layout{nullptr};
        WidgetItem* parent{nullptr};
        QList<WidgetItem*> children;
        bool expanded{false};
    };
private:
    void updateLater();
    void updateItem(WidgetItem* item);
    void insertRow(QGridLayout* layout, int row) const;
    void removeRow(QGridLayout* layout, int row) const;
    int gridRow(WidgetItem* item) const;
    int gridSpan(WidgetItem* item) const;
    void setExpanded(WidgetItem* item, bool expanded);
    QToolButton* createButton(QWidget* panret = nullptr) const;

    QMap<BrowserItem*, WidgetItem*> indexToItem_;
    QMap<WidgetItem *, BrowserItem*> itemToIndex_;
    QMap<QWidget*, WidgetItem*> widgetToItem_;
    QMap<QObject*, WidgetItem*> buttonToItem_;
    QGridLayout* mainLayout_;
    QList<WidgetItem*> children_;
    QList<WidgetItem*> recreateQueue_;
};
