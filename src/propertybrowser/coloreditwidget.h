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
class ColorEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColorEditWidget(QWidget* parent);

    bool eventFilter(QObject* obj, QEvent* ev);

public Q_SLOTS:
    void setValue(const QColor& value);

private Q_SLOTS:
    void buttonClicked();

Q_SIGNALS:
    void valueChanged(const QColor& value);

private:
    QColor color_;
    QLabel* pixmapLabel_;
    QLabel* label_;
    QToolButton* button_;
};
