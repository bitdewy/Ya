/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "coloreditorfactory.h"
#include "private/coloreditorfactory_p.h"

ColorEditorFactory::ColorEditorFactory(QObject *parent) :
    AbstractEditorFactory<ColorPropertyManager>(parent),
    impl_(new ColorEditorFactoryPrivate())
{
    impl_->colorEditorFactoryPtr_ = this;
}

ColorEditorFactory::~ColorEditorFactory()
{
    qDeleteAll(impl_->editorToProperty_.keys());
}

void ColorEditorFactory::connectPropertyManager(ColorPropertyManager* manager)
{
    connect(manager, &ColorPropertyManager::valueChanged,
        this, &ColorEditorFactory::slotPropertyChanged);
}

QWidget* ColorEditorFactory::createEditor(ColorPropertyManager* manager,
    Property* property, QWidget* parent)
{
    auto editor = impl_->createEditor(property, parent);
    editor->setValue(manager->value(property));
    connect(editor, &ColorEditWidget::valueChanged, this, [this](QColor color) { impl_->slotSetValue(color); });
    connect(editor, &ColorEditWidget::destroyed, this, [this](QObject* o) { impl_->slotEditorDestroyed(o); });
    return editor;
}

void ColorEditorFactory::disconnectPropertyManager(ColorPropertyManager* manager)
{
    disconnect(manager, &ColorPropertyManager::valueChanged,
        this, &ColorEditorFactory::slotPropertyChanged);
}

void ColorEditorFactory::slotPropertyChanged(Property* property, const QColor& value)
{
    impl_->slotPropertyChanged(property, value);
}
