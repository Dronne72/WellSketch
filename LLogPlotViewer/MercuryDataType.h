// ### Qt6.8.0 MSVC 2022_64
// Copyright (C) 2025 North Side aly 24/03/2025

#pragma once

#include <QStringList>
#include <QtGlobal>
#include <QString>
#include <QObject>
#include <QFont>
#include <QColor>
#include <QMetaEnum>
#include <QJsonObject>

const qreal kUndefinedData = -999.25;

const qreal kSystemUndefinedData = -9999;

// ----------------------------------------------------------------
//                DATASET Properties
// ----------------------------------------------------------------

struct DatasetProperties
{
    // Main
    QString id;           // unique identifier

    // Data type
    QString dataType = QString("");

    // Statistic
    qreal start = 0.0;        // Start depth
    qreal stop = 0.0;         // Stop Depth
    qreal step = 0.0;         // Depth Step
    int sizeY = 0;            // n-rows
    int sizeX = 0;            // n-columns
    qreal minValue = kSystemUndefinedData; // Min Value
    qreal maxValue = kSystemUndefinedData; // Max Value;
};

//-----------------------------------------------------------------------------
struct CurveHeader {
    QString curveDataID;
    QString dataVersion;
    DatasetProperties parameters;
};
//-----------------------------------------------------------------------------
struct CurveData {
    QMetaType::Type metaType;
    QList<QVariant> values;
};

