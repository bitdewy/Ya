/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "fonteditorfactory.h"
#include "private/fonteditorfactory_p.h"

FontEditorFactory::FontEditorFactory(QObject* parent) :
    AbstractEditorFactory<FontPropertyManager>(parent),
    impl_(new FontEditorFactoryPrivate())
{
    impl_->fontEditorFactoryPtr_ = this;
}

FontEditorFactory::~FontEditorFactory()
{
    qDeleteAll(impl_->editorToProperty_.keys());
}

void FontEditorFactory::connectPropertyManager(FontPropertyManager* manager)
{
    connect(manager, &FontPropertyManager::valueChanged,
        this, &FontEditorFactory::slotPropertyChanged);
}

QWidget* FontEditorFactory::createEditor(FontPropertyManager* manager,
    Property* property, QWidget* parent)
{
    auto editor = impl_->createEditor(property, parent);
    editor->setValue(manager->value(property));
    connect(editor, &FontEditWidget::valueChanged,
        this, [this](const QFont& font) { impl_->slotSetValue(font); });
    connect(editor, &FontEditWidget::destroyed,
        this, [this](QObject* o) { impl_->slotEditorDestroyed(o); });
    return editor;
}

void FontEditorFactory::disconnectPropertyManager(FontPropertyManager* manager)
{
    disconnect(manager, &FontPropertyManager::valueChanged,
        this, &FontEditorFactory::slotPropertyChanged);
}

void FontEditorFactory::slotPropertyChanged(Property* property, const QFont& value)
{
    impl_->slotPropertyChanged(property, value);
}
