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
#include "propertymanager/fontpropertymanager.h"

class FontEditorFactoryPrivate;
class FontEditorFactory : public AbstractEditorFactory<FontPropertyManager>
{
    Q_OBJECT
public:
    explicit FontEditorFactory(QObject* parent = nullptr);
    ~FontEditorFactory();

protected:
    void connectPropertyManager(FontPropertyManager* manager);
    QWidget* createEditor(FontPropertyManager* manager, Property* property, QWidget* parent);
    void disconnectPropertyManager(FontPropertyManager* manager);

private:
    void slotPropertyChanged(Property* property, const QFont& value);

    QScopedPointer<FontEditorFactoryPrivate> impl_;
    friend class FontEditorFactoryPrivate;
};
