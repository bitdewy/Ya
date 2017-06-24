/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "cursordatabase.h"
#include <QtCore/QCoreApplication>
#include <QtGui/QCursor>

CursorDatabase::CursorDatabase()
{
    appendCursor(Qt::ArrowCursor, QCoreApplication::translate("QtCursorDatabase", "Arrow"),
        QIcon(QLatin1String(":/bitdewy/ya/propertybrowser/images/cursor-arrow.png")));
    appendCursor(Qt::UpArrowCursor, QCoreApplication::translate("QtCursorDatabase", "Up Arrow"),
        QIcon(QLatin1String(":/bitdewy/ya/propertybrowser/images/cursor-uparrow.png")));
    appendCursor(Qt::CrossCursor, QCoreApplication::translate("QtCursorDatabase", "Cross"),
        QIcon(QLatin1String(":/bitdewy/ya/propertybrowser/images/cursor-cross.png")));
    appendCursor(Qt::WaitCursor, QCoreApplication::translate("QtCursorDatabase", "Wait"),
        QIcon(QLatin1String(":/bitdewy/ya/propertybrowser/images/cursor-wait.png")));
    appendCursor(Qt::IBeamCursor, QCoreApplication::translate("QtCursorDatabase", "IBeam"),
        QIcon(QLatin1String(":/bitdewy/ya/propertybrowser/images/cursor-ibeam.png")));
    appendCursor(Qt::SizeVerCursor, QCoreApplication::translate("QtCursorDatabase", "Size Vertical"),
        QIcon(QLatin1String(":/bitdewy/ya/propertybrowser/images/cursor-sizev.png")));
    appendCursor(Qt::SizeHorCursor, QCoreApplication::translate("QtCursorDatabase", "Size Horizontal"),
        QIcon(QLatin1String(":/bitdewy/ya/propertybrowser/images/cursor-sizeh.png")));
    appendCursor(Qt::SizeFDiagCursor, QCoreApplication::translate("QtCursorDatabase", "Size Backslash"),
        QIcon(QLatin1String(":/bitdewy/ya/propertybrowser/images/cursor-sizef.png")));
    appendCursor(Qt::SizeBDiagCursor, QCoreApplication::translate("QtCursorDatabase", "Size Slash"),
        QIcon(QLatin1String(":/bitdewy/ya/propertybrowser/images/cursor-sizeb.png")));
    appendCursor(Qt::SizeAllCursor, QCoreApplication::translate("QtCursorDatabase", "Size All"),
        QIcon(QLatin1String(":/bitdewy/ya/propertybrowser/images/cursor-sizeall.png")));
    appendCursor(Qt::BlankCursor, QCoreApplication::translate("QtCursorDatabase", "Blank"), QIcon());
    appendCursor(Qt::SplitVCursor, QCoreApplication::translate("QtCursorDatabase", "Split Vertical"),
        QIcon(QLatin1String(":/bitdewy/ya/propertybrowser/images/cursor-vsplit.png")));
    appendCursor(Qt::SplitHCursor, QCoreApplication::translate("QtCursorDatabase", "Split Horizontal"),
        QIcon(QLatin1String(":/bitdewy/ya/propertybrowser/images/cursor-hsplit.png")));
    appendCursor(Qt::PointingHandCursor, QCoreApplication::translate("QtCursorDatabase", "Pointing Hand"),
        QIcon(QLatin1String(":/bitdewy/ya/propertybrowser/images/cursor-hand.png")));
    appendCursor(Qt::ForbiddenCursor, QCoreApplication::translate("QtCursorDatabase", "Forbidden"),
        QIcon(QLatin1String(":/bitdewy/ya/propertybrowser/images/cursor-forbidden.png")));
    appendCursor(Qt::OpenHandCursor, QCoreApplication::translate("QtCursorDatabase", "Open Hand"),
        QIcon(QLatin1String(":/bitdewy/ya/propertybrowser/images/cursor-openhand.png")));
    appendCursor(Qt::ClosedHandCursor, QCoreApplication::translate("QtCursorDatabase", "Closed Hand"),
        QIcon(QLatin1String(":/bitdewy/ya/propertybrowser/images/cursor-closedhand.png")));
    appendCursor(Qt::WhatsThisCursor, QCoreApplication::translate("QtCursorDatabase", "What's This"),
        QIcon(QLatin1String(":/bitdewy/ya/propertybrowser/images/cursor-whatsthis.png")));
    appendCursor(Qt::BusyCursor, QCoreApplication::translate("QtCursorDatabase", "Busy"),
        QIcon(QLatin1String(":/bitdewy/ya/propertybrowser/images/cursor-busy.png")));
}

void CursorDatabase::clear()
{
    cursorNames_.clear();
    cursorIcons_.clear();
    valueToCursorShape_.clear();
    cursorShapeToValue_.clear();
}

void CursorDatabase::appendCursor(Qt::CursorShape shape, const QString& name, const QIcon& icon)
{
    if (cursorShapeToValue_.contains(shape))
    {
        return;
    }
    const auto value = cursorNames_.count();
    cursorNames_.append(name);
    cursorIcons_.insert(value, icon);
    valueToCursorShape_.insert(value, shape);
    cursorShapeToValue_.insert(shape, value);
}

QStringList CursorDatabase::cursorShapeNames() const
{
    return cursorNames_;
}

QMap<int, QIcon> CursorDatabase::cursorShapeIcons() const
{
    return cursorIcons_;
}

QString CursorDatabase::cursorToShapeName(const QCursor& cursor) const
{
    auto val = cursorToValue(cursor);
    return val >= 0 ? cursorNames_.at(val) : QString();
}

QIcon CursorDatabase::cursorToShapeIcon(const QCursor& cursor) const
{
    auto val = cursorToValue(cursor);
    return cursorIcons_.value(val);
}

int CursorDatabase::cursorToValue(const QCursor& cursor) const
{

    auto shape = cursor.shape();
    if (cursorShapeToValue_.contains(shape))
    {
        return cursorShapeToValue_[shape];
    }
    return -1;
}

QCursor CursorDatabase::valueToCursor(int value) const
{
    if (valueToCursorShape_.contains(value))
    {
        return QCursor(valueToCursorShape_[value]);
    }
    return QCursor();
}
