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

class ExtensionManager;
class AbstractFormEditor : public QObject
{
    Q_OBJECT
public:
    explicit AbstractFormEditor(QObject* parent = nullptr);
    virtual ~AbstractFormEditor();
    AbstractFormEditor(const AbstractFormEditor&) = delete;
    AbstractFormEditor& operator=(const AbstractFormEditor&) = delete;

    virtual ExtensionManager* extensionManager() const = 0;

    virtual QWidget* topLevel() const = 0;
    virtual void setTopLevel(QWidget* topLevel) = 0;

protected:
    virtual void setExtensionManager(ExtensionManager* extensionManager) = 0;
};
