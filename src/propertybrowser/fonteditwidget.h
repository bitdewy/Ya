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

class QLabel;
class QToolButton;
class FontEditWidget : public QWidget
{
    Q_OBJECT

public:
    FontEditWidget(QWidget* parent);

    bool eventFilter(QObject* obj, QEvent* ev);

public Q_SLOTS:
    void setValue(const QFont& value);

private Q_SLOTS:
    void buttonClicked();

Q_SIGNALS:
    void valueChanged(const QFont& value);

private:
    QFont font_;
    QLabel* pixmapLabel_;
    QLabel* label_;
    QToolButton* button_;
};
