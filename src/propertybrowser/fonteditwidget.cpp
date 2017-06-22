/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "fonteditwidget.h"
#include "private/propertybrowserutils_p.h"
#include "editorfactory/private/editorfactory_p.h"
#include <QtGui/QKeyEvent>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFontDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>

FontEditWidget::FontEditWidget(QWidget* parent) :
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
    connect(button_, &QToolButton::clicked, this, &FontEditWidget::buttonClicked);
    lt->addWidget(button_);
    pixmapLabel_->setPixmap(PropertyBrowserUtils::fontValuePixmap(font_));
    label_->setText(PropertyBrowserUtils::fontValueText(font_));
}

void FontEditWidget::setValue(const QFont& f)
{
    if (font_ != f)
    {
        font_ = f;
        pixmapLabel_->setPixmap(PropertyBrowserUtils::fontValuePixmap(f));
        label_->setText(PropertyBrowserUtils::fontValueText(f));
    }
}

void FontEditWidget::buttonClicked()
{
    bool ok = false;
    auto newFont = QFontDialog::getFont(&ok, font_, this, tr("Select Font"));
    if (ok && newFont != font_)
    {
        QFont f = font_;
        // prevent mask for unchanged attributes, don't change other attributes (like kerning, etc...)
        if (font_.family() != newFont.family())
        {
            f.setFamily(newFont.family());
        }
        if (font_.pointSize() != newFont.pointSize())
        {
            f.setPointSize(newFont.pointSize());
        }
        if (font_.bold() != newFont.bold())
        {
            f.setBold(newFont.bold());
        }
        if (font_.italic() != newFont.italic())
        {
            f.setItalic(newFont.italic());
        }
        if (font_.underline() != newFont.underline())
        {
            f.setUnderline(newFont.underline());
        }
        if (font_.strikeOut() != newFont.strikeOut())
        {
            f.setStrikeOut(newFont.strikeOut());
        }
        setValue(f);
        emit valueChanged(font_);
    }
}

bool FontEditWidget::eventFilter(QObject* obj, QEvent* ev)
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
