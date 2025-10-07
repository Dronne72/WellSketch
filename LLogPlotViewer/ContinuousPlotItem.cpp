#include "ContinuousPlotItem.h"

#include <QFile>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonArray>

#include "MercuryDataType.h"

ContinuousPlotItem::ContinuousPlotItem() {}
//-----------------------------------------------------------------------------
ContinuousPlotItem::ContinuousPlotItem(const QJsonObject &curve_obj) {

    auto header_obj = curve_obj["CurveHeader"].toObject();
    auto data_obj = curve_obj["CurveData"].toObject();

    m_curveHeader.curveDataID = header_obj["id"].toString();
    m_curveHeader.dataVersion = header_obj["0.0.1"].toString();

    // Main
    m_curveHeader.parameters.id = m_curveHeader.curveDataID;

    // Data type
    m_curveHeader.parameters.dataType =  header_obj["dataType"].toString();
    m_plotTitle = m_curveHeader.parameters.dataType;

    // Statistic
    m_curveHeader.parameters.start = header_obj["start"].toDouble();
    m_curveHeader.parameters.stop = header_obj["stop"].toDouble();
    m_curveHeader.parameters.step = header_obj["step"].toDouble();         // n-columns
    m_curveHeader.parameters.minValue = header_obj["minValue"].toDouble();
    m_curveHeader.parameters.maxValue = header_obj["maxValue"].toDouble();

    m_dataType = (QMetaType::Type)data_obj["dataType"].toInt();
    auto values = data_obj["values"].toArray();

    QList<qreal> curve_points;
    foreach (auto value, values) {
        curve_points.append(value.toDouble());
    }

    setCurvePoins(curve_points);
}
//-----------------------------------------------------------------------------
void ContinuousPlotItem::setCurvePoins(const QList<qreal> curve_points) {
    m_curvePoints = std::move(curve_points);
}
void ContinuousPlotItem::addCurvePoins(const QList<qreal> curve_points) {
    m_curvePoints.append(std::move(curve_points));
}
//-----------------------------------------------------------------------------
const QList<qreal> &ContinuousPlotItem::сurvePoins() const {
    return m_curvePoints;
}
