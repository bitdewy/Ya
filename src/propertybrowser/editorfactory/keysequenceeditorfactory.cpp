/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "keysequenceeditorfactory.h"
#include "private/keysequenceeditorfactory_p.h"

KeySequenceEditorFactory::KeySequenceEditorFactory(QObject *parent) :
    AbstractEditorFactory<KeySequencePropertyManager>(parent),
    impl_(new KeySequenceEditorFactoryPrivate())
{
    impl_->keySequenceEditorFactoryPtr_ = this;

}

KeySequenceEditorFactory::~KeySequenceEditorFactory()
{
    qDeleteAll(impl_->editorToProperty_.keys());
}

void KeySequenceEditorFactory::connectPropertyManager(KeySequencePropertyManager* manager)
{
    connect(manager, &KeySequencePropertyManager::valueChanged,
        this, &KeySequenceEditorFactory::slotPropertyChanged);
}

QWidget* KeySequenceEditorFactory::createEditor(KeySequencePropertyManager* manager,
    Property* property, QWidget* parent)
{
    auto editor = impl_->createEditor(property, parent);
    editor->setKeySequence(manager->value(property));

    connect(editor, &QKeySequenceEdit::keySequenceChanged,
        this, [this](const QKeySequence& sequence) { impl_->slotSetValue(sequence); });
    connect(editor, &QKeySequenceEdit::destroyed,
        this, [this](QObject* o) { impl_->slotEditorDestroyed(o); });
    return editor;
}

void KeySequenceEditorFactory::disconnectPropertyManager(KeySequencePropertyManager* manager)
{
    disconnect(manager, &KeySequencePropertyManager::valueChanged,
        this, &KeySequenceEditorFactory::slotPropertyChanged);
}

void KeySequenceEditorFactory::slotPropertyChanged(Property* property, const QKeySequence& value)
{
    impl_->slotPropertyChanged(property, value);
}
