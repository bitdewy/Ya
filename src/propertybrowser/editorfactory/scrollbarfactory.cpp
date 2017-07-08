/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "scrollbarfactory.h"
#include "editorfactory/private/scrollbarfactory_p.h"

ScrollBarFactory::ScrollBarFactory(QObject* parent) :
    AbstractEditorFactory<IntPropertyManager>(parent),
    impl_(new ScrollBarFactoryPrivate())
{
    impl_->scrollBarFactoryPtr_ = this;

}

ScrollBarFactory::~ScrollBarFactory()
{
    qDeleteAll(impl_->editorToProperty_.keys());
}

void ScrollBarFactory::connectPropertyManager(IntPropertyManager* manager)
{
    connect(manager, &IntPropertyManager::valueChanged,
        this, &ScrollBarFactory::slotPropertyChanged);
    connect(manager, &IntPropertyManager::rangeChanged,
        this, &ScrollBarFactory::slotRangeChanged);
    connect(manager, &IntPropertyManager::singleStepChanged,
        this, &ScrollBarFactory::slotSingleStepChanged);
}

QWidget* ScrollBarFactory::createEditor(IntPropertyManager* manager, Property* property,
    QWidget* parent)
{
    auto editor = new QScrollBar(Qt::Horizontal, parent);
    impl_->initializeEditor(property, editor);
    editor->setSingleStep(manager->singleStep(property));
    editor->setRange(manager->minimum(property), manager->maximum(property));
    editor->setValue(manager->value(property));
    connect(editor, &QScrollBar::valueChanged,
        this, [this](int value) { impl_->slotSetValue(value); });
    connect(editor, &QScrollBar::destroyed,
        this, [this](QObject* o) { impl_->slotEditorDestroyed(o); });
    return editor;
}

void ScrollBarFactory::disconnectPropertyManager(IntPropertyManager* manager)
{
    disconnect(manager, &IntPropertyManager::valueChanged,
        this, &ScrollBarFactory::slotPropertyChanged);
    disconnect(manager, &IntPropertyManager::rangeChanged,
        this, &ScrollBarFactory::slotRangeChanged);
    disconnect(manager, &IntPropertyManager::singleStepChanged,
        this, &ScrollBarFactory::slotSingleStepChanged);
}

void ScrollBarFactory::slotPropertyChanged(Property* property, int value)
{
    impl_->slotPropertyChanged(property, value);
}

void ScrollBarFactory::slotRangeChanged(Property* property, int min, int max)
{
    impl_->slotRangeChanged(property, min, max);
}

void ScrollBarFactory::slotSingleStepChanged(Property* property, int step)
{
    impl_->slotSingleStepChanged(property, step);
}
