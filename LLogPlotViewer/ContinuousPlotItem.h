// ### Qt6.8.0 MSVC 2022_64
// Copyright (C) 2025 North Side pan 12/09/2025

#ifndef CONTINUOUSPLOTITEM_H
#define CONTINUOUSPLOTITEM_H

#include "MercuryDataType.h"

#include <QMetaType>

struct MercuryUserUnit;

class ContinuousPlotItem final {
public:
    ContinuousPlotItem();
    ContinuousPlotItem(const QJsonObject &curve_obj);
    ~ContinuousPlotItem() {}

    QString plotTitle() {return m_plotTitle; }
    QString curveID() { return m_curveHeader.curveDataID; }
    DatasetProperties getDatasetProperties() { return m_curveHeader.parameters; }
    void setDatasetProperties(const DatasetProperties &parameters) {
        m_curveHeader.parameters = parameters; }

    void setCurvePoins(const QList<qreal> curve_points);
    void addCurvePoins(const QList<qreal> curve_points);
    const QList<qreal> &сurvePoins() const;

private:
    QString m_plotTitle;
    CurveHeader m_curveHeader;
    QMetaType::Type m_dataType = QMetaType::Double;
    QList<qreal> m_curvePoints;
};

#endif // CONTINUOUSPLOTITEM_H
