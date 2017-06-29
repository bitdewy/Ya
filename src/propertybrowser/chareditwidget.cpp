/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "chareditwidget.h"
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMenu>
#include <QtGui/QtEvents>

CharEditWidget::CharEditWidget(QWidget* parent) :
    QWidget(parent),
    lineEdit_(new QLineEdit(this))
{
    auto layout = new QHBoxLayout(this);
    layout->addWidget(lineEdit_);
    layout->setMargin(0);
    lineEdit_->installEventFilter(this);
    lineEdit_->setReadOnly(true);
    lineEdit_->setFocusProxy(this);
    setFocusPolicy(lineEdit_->focusPolicy());
    setAttribute(Qt::WA_InputMethodEnabled);
}

bool CharEditWidget::eventFilter(QObject* o, QEvent* e)
{
    if (o == lineEdit_ && e->type() == QEvent::ContextMenu)
    {
        auto c = static_cast<QContextMenuEvent *>(e);
        auto menu = lineEdit_->createStandardContextMenu();
        const auto actions = menu->actions();
        for (auto&& action : actions)
        {
            action->setShortcut(QKeySequence());
            auto actionString = action->text();
            const auto pos = actionString.lastIndexOf(QLatin1Char('\t'));
            if (pos > 0)
            {
                actionString = actionString.remove(pos, actionString.length() - pos);
            }
            action->setText(actionString);
        }
        QAction* actionBefore = nullptr;
        if (actions.count() > 0)
        {
            actionBefore = actions[0];
        }
        auto clearAction = new QAction(tr("Clear Char"), menu);
        menu->insertAction(actionBefore, clearAction);
        menu->insertSeparator(actionBefore);
        clearAction->setEnabled(!m_value.isNull());
        connect(clearAction, &QAction::triggered, this, &CharEditWidget::slotClearChar);
        menu->exec(c->globalPos());
        delete menu;
        e->accept();
        return true;
    }
    return QWidget::eventFilter(o, e);
}

void CharEditWidget::slotClearChar()
{
    if (m_value.isNull())
    {
        return;
    }
    setValue(QChar());
    emit valueChanged(m_value);
}

void CharEditWidget::handleKeyEvent(QKeyEvent* e)
{
    const int key = e->key();
    switch (key)
    {
    case Qt::Key_Control:
    case Qt::Key_Shift:
    case Qt::Key_Meta:
    case Qt::Key_Alt:
    case Qt::Key_Super_L:
    case Qt::Key_Return:
        return;
    default:
        break;
    }

    const auto text = e->text();
    if (text.count() != 1)
    {
        return;
    }
    const auto c = text.at(0);
    if (!c.isPrint())
    {
        return;
    }
    if (m_value == c)
    {
        return;
    }
    m_value = c;
    const auto str = m_value.isNull() ? QString() : QString(m_value);
    lineEdit_->setText(str);
    e->accept();
    emit valueChanged(m_value);
}

void CharEditWidget::setValue(const QChar& value)
{
    if (value == m_value)
    {
        return;
    }
    m_value = value;
    auto str = value.isNull() ? QString() : QString(value);
    lineEdit_->setText(str);
}

QChar CharEditWidget::value() const
{
    return m_value;
}

void CharEditWidget::focusInEvent(QFocusEvent* e)
{
    lineEdit_->event(e);
    lineEdit_->selectAll();
    QWidget::focusInEvent(e);
}

void CharEditWidget::focusOutEvent(QFocusEvent* e)
{
    lineEdit_->event(e);
    QWidget::focusOutEvent(e);
}

void CharEditWidget::keyPressEvent(QKeyEvent* e)
{
    handleKeyEvent(e);
    e->accept();
}

void CharEditWidget::keyReleaseEvent(QKeyEvent* e)
{
    lineEdit_->event(e);
}

bool CharEditWidget::event(QEvent* e)
{
    switch(e->type())
    {
    case QEvent::Shortcut:
    case QEvent::ShortcutOverride:
    case QEvent::KeyRelease:
        e->accept();
        return true;
    default:
        break;
    }
    return QWidget::event(e);
}
