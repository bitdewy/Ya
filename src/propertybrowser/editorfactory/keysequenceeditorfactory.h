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
#include "propertymanager/keysequencepropertymanager.h"

class KeySequenceEditorFactoryPrivate;
class KeySequenceEditorFactory : public AbstractEditorFactory<KeySequencePropertyManager>
{
    Q_OBJECT
public:
    explicit KeySequenceEditorFactory(QObject* parent = nullptr);
    ~KeySequenceEditorFactory();
protected:
    void connectPropertyManager(KeySequencePropertyManager* manager);
    QWidget* createEditor(KeySequencePropertyManager* manager, Property* property, QWidget* parent);
    void disconnectPropertyManager(KeySequencePropertyManager* manager);
private:
    void slotPropertyChanged(Property* property, const QKeySequence& value);
    QScopedPointer<KeySequenceEditorFactoryPrivate> impl_;
    friend class KeySequenceEditorFactoryPrivate;
};
