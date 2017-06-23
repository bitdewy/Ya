/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "propertybrowserutils_p.h"
#include <QtWidgets/QApplication>
#include <QtGui/QPainter>
#include <QtWidgets/QHBoxLayout>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMenu>
#include <QtCore/QLocale>

QPixmap PropertyBrowserUtils::brushValuePixmap(const QBrush& b)
{
    QImage img(16, 16, QImage::Format_ARGB32_Premultiplied);
    img.fill(0);

    QPainter painter(&img);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(0, 0, img.width(), img.height(), b);
    auto color = b.color();
    if (color.alpha() != 255)
    {
        auto opaqueBrush = b;
        color.setAlpha(255);
        opaqueBrush.setColor(color);
        painter.fillRect(img.width() / 4, img.height() / 4,
            img.width() / 2, img.height() / 2, opaqueBrush);
    }
    painter.end();
    return QPixmap::fromImage(img);
}

QIcon PropertyBrowserUtils::brushValueIcon(const QBrush& b)
{
    return QIcon(brushValuePixmap(b));
}

QString PropertyBrowserUtils::colorValueText(const QColor& c)
{
    return QCoreApplication::translate("QtPropertyBrowserUtils", "[%1, %2, %3] (%4)")
        .arg(c.red()).arg(c.green()).arg(c.blue()).arg(c.alpha());
}

QPixmap PropertyBrowserUtils::fontValuePixmap(const QFont& font)
{
    auto f = font;
    QImage img(16, 16, QImage::Format_ARGB32_Premultiplied);
    img.fill(0);
    QPainter p(&img);
    p.setRenderHint(QPainter::TextAntialiasing, true);
    p.setRenderHint(QPainter::Antialiasing, true);
    f.setPointSize(13);
    p.setFont(f);
    QTextOption t;
    t.setAlignment(Qt::AlignCenter);
    p.drawText(QRect(0, 0, 16, 16), QString(QLatin1Char('A')), t);
    return QPixmap::fromImage(img);
}

QIcon PropertyBrowserUtils::fontValueIcon(const QFont& f)
{
    return QIcon(fontValuePixmap(f));
}

QString PropertyBrowserUtils::fontValueText(const QFont& f)
{
    return QCoreApplication::translate("PropertyBrowserUtils", "[%1, %2]")
        .arg(f.family()).arg(f.pointSize());
}

QString PropertyBrowserUtils::dateFormat()
{
    QLocale loc;
    return loc.dateFormat(QLocale::ShortFormat);
}

QString PropertyBrowserUtils::timeFormat()
{
    QLocale loc;
    return loc.timeFormat(QLocale::LongFormat);
}

QString PropertyBrowserUtils::dateTimeFormat()
{
    auto format = dateFormat();
    format += QLatin1Char(' ');
    format += timeFormat();
    return format;
}
