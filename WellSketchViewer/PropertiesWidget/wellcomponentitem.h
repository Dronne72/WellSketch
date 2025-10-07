#ifndef WELLCOMPONENTITEM_H
#define WELLCOMPONENTITEM_H

#include <QStandardItem>
#include <QColor>

class WellComponentItem : public QStandardItem
{
public:
    enum DataRole {
        NameRole = Qt::UserRole + 1,
        ColorRole = Qt::UserRole + 2,
        TopRole = Qt::UserRole + 3,
        BottomRole = Qt::UserRole + 4,
        InnerDiameterRole = Qt::UserRole + 5,
        OuterDiameterRole = Qt::UserRole + 6,
        CasingShoeRole = Qt::UserRole + 7,
        FillPatternRole = Qt::UserRole + 8
    };

    WellComponentItem(const QString &text, const QString &type);

    // Getters
    QString componentName() const { return data(NameRole).toString(); }
    QString componentType() const { return data(Qt::UserRole).toString(); }
    QColor componentColor() const { return data(ColorRole).value<QColor>(); }
    double top() const { return data(TopRole).toDouble(); }
    double bottom() const { return data(BottomRole).toDouble(); }
    double innerDiameter() const { return data(InnerDiameterRole).toDouble(); }
    double outerDiameter() const { return data(OuterDiameterRole).toDouble(); }
    QString casingShoe() const { return data(CasingShoeRole).toString(); }
    int fillPattern() const { return data(FillPatternRole).toInt(); }

    // Setters
    void setComponentName(const QString &name) { setData(name, NameRole); }
    void setComponentColor(const QColor &color) { setData(color, ColorRole); }
    void setTop(double value) { setData(value, TopRole); }
    void setBottom(double value) { setData(value, BottomRole); }
    void setInnerDiameter(double value) { setData(value, InnerDiameterRole); }
    void setOuterDiameter(double value) { setData(value, OuterDiameterRole); }
    void setCasingShoe(const QString &value) { setData(value, CasingShoeRole); }
    void setFillPattern(int value) { setData(value, FillPatternRole); }
};

#endif // WELLCOMPONENTITEM_H
