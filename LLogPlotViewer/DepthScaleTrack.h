// ### Qt6.8.0 MSVC 2022_64
// Copyright (C) 2025 North Side pan 11/09/2025

#ifndef DEPTHSCALETRACK_H
#define DEPTHSCALETRACK_H

#include "AbstractPlotTrack.h"

#include <QObject>

class DepthScaleTrack final : public AbstractPlotTrack {
    Q_OBJECT
public:
    DepthScaleTrack(QWidget *parent = nullptr);
    virtual ~DepthScaleTrack() override;

public:
    void setGlobalDepthRange(double lower, double upper);

    void setYAxisRange(const QCPRange &range) override;
    void setYAxisRange(double lower, double upper) override;
    void setYAxisTicker(QSharedPointer<QCPAxisTicker> ticker) override;
    void setYAxisReversed(bool reversed) override;
    void setXAxisRange(const QCPRange &range) override;
    void setXAxisRange(double lower, double upper) override;
    void setXAxisReversed(bool reversed) override;

public slots:
    void onGlobalDepthChanged(const QCPRange &newRange);

private:
    bool m_globalDepthSetted = false;
};

#endif // DEPTHSCALETRACK_H
