/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <QtWidgets/QWidget>

class QLineEdit;
class CharEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CharEditWidget(QWidget* parent = nullptr);

    QChar value() const;
    bool eventFilter(QObject* o, QEvent* e);
public Q_SLOTS:
    void setValue(const QChar& value);
Q_SIGNALS:
    void valueChanged(const QChar& value);
protected:
    void focusInEvent(QFocusEvent* e);
    void focusOutEvent(QFocusEvent* e);
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);
    bool event(QEvent* e);
private slots:
    void slotClearChar();
private:
    void handleKeyEvent(QKeyEvent* e);

    QChar m_value;
    QLineEdit* lineEdit_;
};
