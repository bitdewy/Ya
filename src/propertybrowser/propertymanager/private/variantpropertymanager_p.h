/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <QtCore/QMap>

class QDate;
class QTime;
class QDateTime;
class QKeySequence;
class QLocale;
class QPoint;
class QPointF;
class QSize;
class QSizeF;
class QRect;
class QRectF;
class QColor;
class QIcon;
class QSizePolicy;
class QFont;
class QCursor;
class QVariant;
class Property;
class VariantProperty;
class AbstractPropertyManager;
class VariantPropertyManager;
class VariantPropertyManagerPrivate
{
    VariantPropertyManager* variantPropertyManagerPtr_;
    friend class VariantPropertyManager;
public:
    VariantPropertyManagerPrivate();

    bool creatingProperty_;
    bool creatingSubProperties_;
    bool destroyingSubProperties_;
    int propertyType_;

    void slotValueChanged(Property* property, int val);
    void slotRangeChanged(Property* property, int min, int max);
    void slotSingleStepChanged(Property* property, int step);
    void slotValueChanged(Property* property, double val);
    void slotRangeChanged(Property* property, double min, double max);
    void slotSingleStepChanged(Property* property, double step);
    void slotDecimalsChanged(Property* property, int prec);
    void slotValueChanged(Property* property, bool val);
    void slotValueChanged(Property* property, const QString& val);
    void slotRegExpChanged(Property* property, const QRegExp& regExp);
    void slotValueChanged(Property* property, const QDate& val);
    void slotRangeChanged(Property* property, const QDate& min, const QDate& max);
    void slotValueChanged(Property* property, const QTime& val);
    void slotValueChanged(Property* property, const QDateTime& val);
    void slotValueChanged(Property* property, const QKeySequence& val);
    void slotValueChanged(Property* property, const QChar& val);
    void slotValueChanged(Property* property, const QLocale& val);
    void slotValueChanged(Property* property, const QPoint& val);
    void slotValueChanged(Property* property, const QPointF& val);
    void slotValueChanged(Property* property, const QSize& val);
    void slotRangeChanged(Property* property, const QSize& min, const QSize& max);
    void slotValueChanged(Property* property, const QSizeF& val);
    void slotRangeChanged(Property* property, const QSizeF& min, const QSizeF& max);
    void slotValueChanged(Property* property, const QRect& val);
    void slotConstraintChanged(Property* property, const QRect& val);
    void slotValueChanged(Property* property, const QRectF& val);
    void slotConstraintChanged(Property* property, const QRectF& val);
    void slotValueChanged(Property* property, const QColor& val);
    void slotEnumChanged(Property* property, int val);
    void slotEnumNamesChanged(Property* property, const QStringList& enumNames);
    void slotEnumIconsChanged(Property* property, const QMap<int, QIcon>& enumIcons);
    void slotValueChanged(Property* property, const QSizePolicy& val);
    void slotValueChanged(Property* property, const QFont& val);
    void slotValueChanged(Property* property, const QCursor& val);
    void slotFlagChanged(Property* property, int val);
    void slotFlagNamesChanged(Property* property, const QStringList& flagNames);
    void slotPropertyInserted(Property* property, Property* parent, Property* after);
    void slotPropertyRemoved(Property* property, Property* parent);

    void valueChanged(Property* property, const QVariant& val);

    int internalPropertyToType(Property* property) const;
    VariantProperty*createSubProperty(VariantProperty* parent, VariantProperty* after,
        Property* internal);
    void removeSubProperty(VariantProperty* property);

    QMap<int, AbstractPropertyManager*> typeToPropertyManager_;
    QMap<int, QMap<QString, int>> typeToAttributeToAttributeType_;

    QMap<const Property*, QPair<VariantProperty*, int>> propertyToType_;

    QMap<int, int> typeToValueType_;


    QMap<Property*, VariantProperty*> internalToProperty_;

    const QString constraintAttribute_;
    const QString singleStepAttribute_;
    const QString decimalsAttribute_;
    const QString enumIconsAttribute_;
    const QString enumNamesAttribute_;
    const QString flagNamesAttribute_;
    const QString maximumAttribute_;
    const QString minimumAttribute_;
    const QString regExpAttribute_;
};


using PropertyMap = QMap<const Property*, Property*>;
Q_GLOBAL_STATIC(PropertyMap, propertyToWrappedProperty)

Property* wrappedProperty(Property* property);

