// ### Qt6.8.0 MSVC 2022_64
// Copyright (C) 2025 North Side pan 10/06/2025

#ifndef AbstractPlotTrack_H
#define AbstractPlotTrack_H

#include <QWidget>

#include "qcustomplot.h"

class QCustomPlot;
class PlotHeaderLabel;

class AbstractPlotTrack : public QWidget {
    Q_OBJECT

public:
    explicit AbstractPlotTrack(QWidget *parent = nullptr);
    virtual ~AbstractPlotTrack() override;

    QCPRange globalDepthRange() const;

    QCPAxis *getYAxis() const;
    void setYAxis(const QCPAxis *yAxis);
    void setOpenGl(bool use_opengl);

    QMap<double, QColor> colorGradient() const;

    virtual void setYAxisRange(const QCPRange &range) = 0;
    virtual void setYAxisRange(double lower, double upper) = 0;
    virtual void setYAxisTicker(QSharedPointer<QCPAxisTicker> ticker) = 0;
    virtual void setYAxisReversed(bool reversed) = 0;
    virtual void setXAxisRange(const QCPRange &range) = 0;
    virtual void setXAxisRange(double lower, double upper) = 0;
    virtual void setXAxisReversed(bool reversed) = 0;

    void setLineWidth(int width);
    void setLineStyle(int style);

    int graphsCount();

private:
    QCPColorGradient loadGradient(const QString &dataset_speices);

public slots:
    void onYAxisRangeChanged(const QCPRange &range);

signals:
    void globalDepthChanged(const QCPRange &newRange);

protected:
    QCustomPlot *m_customPlot = nullptr;

    double m_globalDepthLower = 0;
    double m_globalDepthUpper = 0;
    double m_globalValuesLower = 0;
    double m_globalValuesUpper = 0;

    QCPColorGradient m_colorGradient;
};

#endif // AbstractPlotTrack_H
