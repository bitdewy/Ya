/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "lineeditfactory.h"
#include "private/lineeditfactory_p.h"
#include <QtGui/QRegExpValidator>

LineEditFactory::LineEditFactory(QObject* parent) :
    AbstractEditorFactory<StringPropertyManager>(parent),
    impl_(new LineEditFactoryPrivate())
{
    impl_->lineEditFactoryPtr_ = this;

}

LineEditFactory::~LineEditFactory()
{
    qDeleteAll(impl_->editorToProperty_.keys());
}

void LineEditFactory::connectPropertyManager(StringPropertyManager* manager)
{
    connect(manager, &StringPropertyManager::valueChanged,
        this, &LineEditFactory::slotPropertyChanged);
    connect(manager, &StringPropertyManager::regExpChanged,
        this, &LineEditFactory::slotRegExpChanged);
}

QWidget* LineEditFactory::createEditor(StringPropertyManager* manager,
    Property* property, QWidget* parent)
{

    auto editor = impl_->createEditor(property, parent);
    auto regExp = manager->regExp(property);
    if (regExp.isValid())
    {
        auto validator = new QRegExpValidator(regExp, editor);
        editor->setValidator(validator);
    }
    editor->setText(manager->value(property));

    connect(editor, &QLineEdit::textEdited,
        this, [this] (const QString& value) { impl_->slotSetValue(value); });
    connect(editor, &QLineEdit::destroyed,
        this, [this](QObject* o) { impl_->slotEditorDestroyed(o); });
    return editor;
}

void LineEditFactory::disconnectPropertyManager(StringPropertyManager* manager)
{
    disconnect(manager, &StringPropertyManager::valueChanged,
        this, &LineEditFactory::slotPropertyChanged);
    disconnect(manager, &StringPropertyManager::regExpChanged,
        this, &LineEditFactory::slotRegExpChanged);
}

void LineEditFactory::slotPropertyChanged(Property* property, const QString& value)
{
    impl_->slotPropertyChanged(property, value);
}

void LineEditFactory::slotRegExpChanged(Property* property, const QRegExp& regExp)
{
    impl_->slotRegExpChanged(property, regExp);
}
