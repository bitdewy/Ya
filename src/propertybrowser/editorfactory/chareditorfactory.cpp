/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "chareditorfactory.h"
#include "private/chareditorfactory_p.h"

CharEditorFactory::CharEditorFactory(QObject *parent) :
    AbstractEditorFactory<CharPropertyManager>(parent),
    impl_(new CharEditorFactoryPrivate())
{
    impl_->charEditorFactoryPtr_ = this;

}

CharEditorFactory::~CharEditorFactory()
{
    qDeleteAll(impl_->editorToProperty_.keys());
}

void CharEditorFactory::connectPropertyManager(CharPropertyManager* manager)
{
    connect(manager, &CharPropertyManager::valueChanged,
        this, &CharEditorFactory::slotPropertyChanged);
}

QWidget* CharEditorFactory::createEditor(CharPropertyManager* manager,
    Property* property, QWidget* parent)
{
    auto editor = impl_->createEditor(property, parent);
    editor->setValue(manager->value(property));

    connect(editor, &CharEditWidget::valueChanged,
        this, [this] (const QChar& c) { impl_->slotSetValue(c); });
    connect(editor, &CharEditWidget::destroyed,
        this, [this](QObject* o) { impl_->slotEditorDestroyed(o); });
    return editor;
}

void CharEditorFactory::disconnectPropertyManager(CharPropertyManager* manager)
{
    disconnect(manager, &CharPropertyManager::valueChanged,
        this, &CharEditorFactory::slotPropertyChanged);
}

void CharEditorFactory::slotPropertyChanged(Property* property, const QChar& value)
{
    impl_->slotPropertyChanged(property, value);
}
