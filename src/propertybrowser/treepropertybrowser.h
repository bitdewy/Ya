/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include "abstractpropertybrowser.h"

class QTreeWidgetItem;
class TreePropertyBrowserPrivate;

class TreePropertyBrowser : public AbstractPropertyBrowser
{
    Q_OBJECT
    Q_ENUMS(ResizeMode)
    Q_PROPERTY(int indentation READ indentation WRITE setIndentation)
    Q_PROPERTY(bool rootIsDecorated READ rootIsDecorated WRITE setRootIsDecorated)
    Q_PROPERTY(bool alternatingRowColors READ alternatingRowColors WRITE setAlternatingRowColors)
    Q_PROPERTY(bool headerVisible READ isHeaderVisible WRITE setHeaderVisible)
    Q_PROPERTY(ResizeMode resizeMode READ resizeMode WRITE setResizeMode)
    Q_PROPERTY(int splitterPosition READ splitterPosition WRITE setSplitterPosition)
    Q_PROPERTY(bool propertiesWithoutValueMarked READ propertiesWithoutValueMarked WRITE setPropertiesWithoutValueMarked)
public:

    enum ResizeMode
    {
        Interactive,
        Stretch,
        Fixed,
        ResizeToContents
    };

    explicit TreePropertyBrowser(QWidget* parent = nullptr);
    ~TreePropertyBrowser();

    int indentation() const;
    void setIndentation(int i);

    bool rootIsDecorated() const;
    void setRootIsDecorated(bool show);

    bool alternatingRowColors() const;
    void setAlternatingRowColors(bool enable);

    bool isHeaderVisible() const;
    void setHeaderVisible(bool visible);

    ResizeMode resizeMode() const;
    void setResizeMode(ResizeMode mode);

    int splitterPosition() const;
    void setSplitterPosition(int position);

    void setExpanded(BrowserItem* item, bool expanded);
    bool isExpanded(BrowserItem* item) const;

    bool isItemVisible(BrowserItem* item) const;
    void setItemVisible(BrowserItem* item, bool visible);

    void setBackgroundColor(BrowserItem* item, const QColor& color);
    QColor backgroundColor(BrowserItem* item) const;
    QColor calculatedBackgroundColor(BrowserItem* item) const;

    void setPropertiesWithoutValueMarked(bool mark);
    bool propertiesWithoutValueMarked() const;

    void editItem(BrowserItem* item);

Q_SIGNALS:

    void collapsed(BrowserItem* item);
    void expanded(BrowserItem* item);

protected:
    virtual void itemInserted(BrowserItem* item, BrowserItem* afterItem);
    virtual void itemRemoved(BrowserItem* item);
    virtual void itemChanged(BrowserItem* item);

private:

    QScopedPointer<TreePropertyBrowserPrivate> impl_;
    friend class TreePropertyBrowserPrivate;
};
