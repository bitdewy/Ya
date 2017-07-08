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

class QMouseEvent;
class QCheckBox;
class BoolEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoolEditWidget(QWidget* parent = nullptr);

    bool textVisible() const;
    void setTextVisible(bool textVisible);

    Qt::CheckState checkState() const;
    void setCheckState(Qt::CheckState state);

    bool isChecked() const;
    void setChecked(bool c);

    bool blockCheckBoxSignals(bool block);

Q_SIGNALS:
    void toggled(bool);

protected:
    void mousePressEvent(QMouseEvent* event);

private:
    QCheckBox* checkBox_;
    bool textVisible_;
};
