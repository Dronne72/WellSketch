#include "DepthScaleTrack.h"

DepthScaleTrack::DepthScaleTrack(QWidget *parent) :
    AbstractPlotTrack(parent) {
    m_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    m_customPlot->legend->setVisible(false);
    m_customPlot->addGraph(m_customPlot->yAxis);

    m_customPlot->axisRect()->insetLayout()
        ->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignLeft);

    m_customPlot->xAxis->setVisible(false);
    m_customPlot->xAxis->setTickLabelColor(Qt::transparent);
    m_customPlot->xAxis->setTickPen(QPen(QColor(Qt::transparent)));
    m_customPlot->xAxis->grid()->setVisible(false);
    m_customPlot->xAxis->setRange(0, 0);
    m_customPlot->yAxis->setRange(0, 100);
    m_customPlot->setNotAntialiasedElements(QCP::aeAxes);
    QSharedPointer<QCPAxisTickerFixed> intTicker(new QCPAxisTickerFixed);
    intTicker->setTickStep(10);
    intTicker->setScaleStrategy(QCPAxisTickerFixed::ssMultiples);
    m_customPlot->yAxis->setTicker(intTicker);
    m_customPlot->yAxis->grid()->setVisible(false);
    auto max_width = m_customPlot->yAxis->axisRect()->width();
    if(max_width == 0) max_width = 60;
    m_customPlot->setMaximumWidth(max_width);
    this->setMaximumWidth(max_width);
}
//-----------------------------------------------------------------------------
DepthScaleTrack::~DepthScaleTrack() {}
//-----------------------------------------------------------------------------
void DepthScaleTrack::onGlobalDepthChanged(const QCPRange &newRange) {
    if(false == m_globalDepthSetted) {
        m_globalDepthLower = newRange.lower;
        m_globalDepthUpper = newRange.upper;
        setGlobalDepthRange(m_globalDepthLower, m_globalDepthUpper);
        m_globalDepthSetted = true;
        return;
    }

    auto global_depth_updated = false;
    if(newRange.lower < m_globalDepthLower) {
        m_globalDepthLower = newRange.lower;
        global_depth_updated = true;
    }

    if(newRange.upper > m_globalDepthUpper) {
        m_globalDepthUpper = newRange.upper;
        global_depth_updated = true;
    }

    if(true == global_depth_updated)  {
        setGlobalDepthRange(m_globalDepthLower, m_globalDepthUpper);
    }
}
//-----------------------------------------------------------------------------
void DepthScaleTrack::setYAxisRange(const QCPRange &range) {
    setYAxisRange(range.lower, range.upper);
}
//-----------------------------------------------------------------------------
void DepthScaleTrack::setYAxisRange(double lower, double upper) {
    // Block the signals to avoid a recursive call
    for(int i = 0; i < m_customPlot->graphCount(); ++i) {
        m_customPlot->graph(i)->keyAxis()->blockSignals(true);
        m_customPlot->graph(i)->keyAxis()->setRange(lower, upper);
        m_customPlot->graph(i)->keyAxis()->blockSignals(false);
    }
    m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}
//-----------------------------------------------------------------------------
void DepthScaleTrack::setYAxisTicker(QSharedPointer<QCPAxisTicker> ticker) {
    // Block the signals to avoid a recursive call
    for(int i = 0; i < m_customPlot->graphCount(); ++i) {
        m_customPlot->graph(i)->keyAxis()->blockSignals(true);
        m_customPlot->graph(i)->keyAxis()->setTicker(ticker);
        m_customPlot->graph(i)->keyAxis()->blockSignals(false);
    }
}
//-----------------------------------------------------------------------------
void DepthScaleTrack::setYAxisReversed(bool reversed) {
    // Block the signals to avoid a recursive call
    for(int i = 0; i < m_customPlot->graphCount(); ++i) {
        m_customPlot->graph(i)->keyAxis()->blockSignals(true);
        m_customPlot->graph(i)->keyAxis()->setRangeReversed(reversed);
        m_customPlot->graph(i)->keyAxis()->blockSignals(false);
    }
}
//-----------------------------------------------------------------------------
void DepthScaleTrack::setXAxisRange(const QCPRange &range) {
    Q_UNUSED(range)
}
//-----------------------------------------------------------------------------
void DepthScaleTrack::setXAxisRange(double lower, double upper) {
    Q_UNUSED(lower)
    Q_UNUSED(upper)
}
//-----------------------------------------------------------------------------
void DepthScaleTrack::setXAxisReversed(bool reversed)  {
    Q_UNUSED(reversed)
}
//-----------------------------------------------------------------------------
void DepthScaleTrack::setGlobalDepthRange(double lower, double upper) {
    m_globalDepthLower = lower;
    m_globalDepthUpper = upper;
    getYAxis()->setRange(m_globalDepthLower, m_globalDepthUpper);
}
