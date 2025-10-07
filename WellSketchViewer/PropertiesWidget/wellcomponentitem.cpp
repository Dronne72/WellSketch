#include "wellcomponentitem.h"

WellComponentItem::WellComponentItem(const QString &text, const QString &type)
    : QStandardItem(text)
{
    setData(type, Qt::UserRole);
}

// WellComponentItem::WellComponentItem(const QString &text, const QString &type)
//     : QStandardItem(text)
// {
//     // Set default properties
//     setData(text, NameRole);
//     setData(type, TypeRole);
//     setData("", SetFromSpecsRole);
//     setData(QVariant(), TopRole); // NA by default
//     setData("ft", TopUnitsRole);
//     setData(QVariant(), BottomRole); // NA by default
//     setData("ft", BottomUnitsRole);
//     setData(0.0, InnerDiameterRole);
//     setData("in", InnerDiameterUnitsRole);
//     setData(0.0, OuterDiameterRole);
//     setData("in", OuterDiameterUnitsRole);
//     setData(QVariant(), RoughnessRole); // NA by default
//     setData("in", RoughnessUnitsRole);
//     setData("", GradeRole);
//     setData("", ColorRole);
//     setData("no fill", FillRole);
// }

// void WellComponentItem::setProperties(const QVariantMap &properties)
// {
//     for (auto it = properties.begin(); it != properties.end(); ++it) {
//         if (it.key() == "Name") setData(it.value(), NameRole);
//         else if (it.key() == "Type") setData(it.value(), TypeRole);
//         else if (it.key() == "SetFromSpecs") setData(it.value(), SetFromSpecsRole);
//         else if (it.key() == "Top") setData(it.value(), TopRole);
//         else if (it.key() == "TopUnits") setData(it.value(), TopUnitsRole);
//         else if (it.key() == "Bottom") setData(it.value(), BottomRole);
//         else if (it.key() == "BottomUnits") setData(it.value(), BottomUnitsRole);
//         else if (it.key() == "InnerDiameter") setData(it.value(), InnerDiameterRole);
//         else if (it.key() == "InnerDiameterUnits") setData(it.value(), InnerDiameterUnitsRole);
//         else if (it.key() == "OuterDiameter") setData(it.value(), OuterDiameterRole);
//         else if (it.key() == "OuterDiameterUnits") setData(it.value(), OuterDiameterUnitsRole);
//         else if (it.key() == "Roughness") setData(it.value(), RoughnessRole);
//         else if (it.key() == "RoughnessUnits") setData(it.value(), RoughnessUnitsRole);
//         else if (it.key() == "Grade") setData(it.value(), GradeRole);
//         else if (it.key() == "Color") setData(it.value(), ColorRole);
//         else if (it.key() == "Fill") setData(it.value(), FillRole);
//     }
//     setText(data(NameRole).toString());
// }

// QVariantMap WellComponentItem::getProperties() const
// {
//     QVariantMap properties;
//     properties["Name"] = data(NameRole);
//     properties["Type"] = data(TypeRole);
//     properties["SetFromSpecs"] = data(SetFromSpecsRole);
//     properties["Top"] = data(TopRole);
//     properties["TopUnits"] = data(TopUnitsRole);
//     properties["Bottom"] = data(BottomRole);
//     properties["BottomUnits"] = data(BottomUnitsRole);
//     properties["InnerDiameter"] = data(InnerDiameterRole);
//     properties["InnerDiameterUnits"] = data(InnerDiameterUnitsRole);
//     properties["OuterDiameter"] = data(OuterDiameterRole);
//     properties["OuterDiameterUnits"] = data(OuterDiameterUnitsRole);
//     properties["Roughness"] = data(RoughnessRole);
//     properties["RoughnessUnits"] = data(RoughnessUnitsRole);
//     properties["Grade"] = data(GradeRole);
//     properties["Color"] = data(ColorRole);
//     properties["Fill"] = data(FillRole);
//     return properties;
// }
