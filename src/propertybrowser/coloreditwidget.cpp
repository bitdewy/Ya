/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "coloreditwidget.h"
#include "editorfactory/private/editorfactory_p.h"
#include "private/propertybrowserutils_p.h"
#include <QtGui/QKeyEvent>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QColorDialog>

ColorEditWidget::ColorEditWidget(QWidget* parent) :
    QWidget(parent),
    pixmapLabel_(new QLabel),
    label_(new QLabel),
    button_(new QToolButton)
{
    auto lt = new QHBoxLayout(this);
    setupTreeViewEditorMargin(lt);
    lt->setSpacing(0);
    lt->addWidget(pixmapLabel_);
    lt->addWidget(label_);
    lt->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Ignored));

    button_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
    button_->setFixedWidth(20);
    setFocusProxy(button_);
    setFocusPolicy(button_->focusPolicy());
    button_->setText(tr("..."));
    button_->installEventFilter(this);
    connect(button_, &QToolButton::clicked, this, &ColorEditWidget::buttonClicked);
    lt->addWidget(button_);
    pixmapLabel_->setPixmap(PropertyBrowserUtils::brushValuePixmap(QBrush(color_)));
    label_->setText(PropertyBrowserUtils::colorValueText(color_));
}

void ColorEditWidget::setValue(const QColor& c)
{
    if (color_ != c)
    {
        color_ = c;
        pixmapLabel_->setPixmap(PropertyBrowserUtils::brushValuePixmap(QBrush(c)));
        label_->setText(PropertyBrowserUtils::colorValueText(c));
    }
}

void ColorEditWidget::buttonClicked()
{
    const auto newColor = QColorDialog::getColor(color_, this, QString(), QColorDialog::ShowAlphaChannel);
    if (newColor.isValid() && newColor != color_)
    {
        setValue(newColor);
        emit valueChanged(color_);
    }
}

bool ColorEditWidget::eventFilter(QObject* obj, QEvent* ev)
{
    if (obj == button_)
    {
        switch (ev->type())
        {
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
        {
            switch (static_cast<const QKeyEvent*>(ev)->key())
            {
            case Qt::Key_Escape:
            case Qt::Key_Enter:
            case Qt::Key_Return:
                ev->ignore();
                return true;
            default:
                break;
            }
        }
            break;
        default:
            break;
        }
    }
    return QWidget::eventFilter(obj, ev);
}
