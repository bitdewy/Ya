/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "booleditwidget.h"
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QApplication>
#include <QtGui/QMouseEvent>

BoolEditWidget::BoolEditWidget(QWidget* parent) :
    QWidget(parent),
    checkBox_(new QCheckBox(this)),
    textVisible_(true)
{
    auto lt = new QHBoxLayout;
    if (QApplication::layoutDirection() == Qt::LeftToRight)
    {
        lt->setContentsMargins(4, 0, 0, 0);
    }
    else
    {
        lt->setContentsMargins(0, 0, 4, 0);
    }
    lt->addWidget(checkBox_);
    setLayout(lt);
    connect(checkBox_, &QCheckBox::toggled, this, &BoolEditWidget::toggled);
    setFocusProxy(checkBox_);
    checkBox_->setText(tr("True"));
}

bool BoolEditWidget::textVisible() const
{
     return textVisible_;
}

void BoolEditWidget::setTextVisible(bool textVisible)
{
    if (textVisible_ == textVisible)
    {
        return;
    }
    textVisible_ = textVisible;
    if (textVisible_)
    {
        checkBox_->setText(isChecked() ? tr("True") : tr("False"));
    }
    else
    {
        checkBox_->setText(QString());
    }
}

Qt::CheckState BoolEditWidget::checkState() const
{
    return checkBox_->checkState();
}

void BoolEditWidget::setCheckState(Qt::CheckState state)
{
    checkBox_->setCheckState(state);
}

bool BoolEditWidget::isChecked() const
{
    return checkBox_->isChecked();
}

void BoolEditWidget::setChecked(bool c)
{
    checkBox_->setChecked(c);
    if (!textVisible_)
    {
        return;
    }
    checkBox_->setText(isChecked() ? tr("True") : tr("False"));
}

bool BoolEditWidget::blockCheckBoxSignals(bool block)
{
    return checkBox_->blockSignals(block);
}

void BoolEditWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        checkBox_->click();
        event->accept();
    }
    else
    {
        QWidget::mousePressEvent(event);
    }
}
