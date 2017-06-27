/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "sliderfactory.h"
#include "private/sliderfactory_p.h"

SliderFactory::SliderFactory(QObject* parent) :
    AbstractEditorFactory<IntPropertyManager>(parent),
    impl_(new SliderFactoryPrivate())
{
    impl_->sliderFactoryPtr_ = this;
}

SliderFactory::~SliderFactory()
{
    qDeleteAll(impl_->editorToProperty_.keys());
}

void SliderFactory::connectPropertyManager(IntPropertyManager* manager)
{
    connect(manager, &IntPropertyManager::valueChanged,
        this, &SliderFactory::slotPropertyChanged);
    connect(manager, &IntPropertyManager::rangeChanged,
        this, &SliderFactory::slotRangeChanged);
    connect(manager, &IntPropertyManager::singleStepChanged,
        this, &SliderFactory::slotSingleStepChanged);
}

QWidget* SliderFactory::createEditor(IntPropertyManager* manager,
        Property* property, QWidget* parent)
{
    auto editor = new QSlider(Qt::Horizontal, parent);
    impl_->initializeEditor(property, editor);
    editor->setSingleStep(manager->singleStep(property));
    editor->setRange(manager->minimum(property), manager->maximum(property));
    editor->setValue(manager->value(property));

    connect(editor, &QSlider::valueChanged,
        this, [this](int value) { impl_->slotSetValue(value); });
    connect(editor, &QSlider::destroyed,
        this, [this](QObject* o) { impl_->slotEditorDestroyed(o); });
    return editor;
}

void SliderFactory::disconnectPropertyManager(IntPropertyManager* manager)
{
    disconnect(manager, &IntPropertyManager::valueChanged,
        this, &SliderFactory::slotPropertyChanged);
    disconnect(manager, &IntPropertyManager::rangeChanged,
        this, &SliderFactory::slotRangeChanged);
    disconnect(manager, &IntPropertyManager::singleStepChanged,
        this, &SliderFactory::slotSingleStepChanged);
}

void SliderFactory::slotPropertyChanged(Property* property, int value)
{
    impl_->slotPropertyChanged(property, value);
}

void SliderFactory::slotRangeChanged(Property* property, int min, int max)
{
    impl_->slotRangeChanged(property, min, max);
}

void SliderFactory::slotSingleStepChanged(Property* property, int step)
{
    impl_->slotSingleStepChanged(property, step);
}
