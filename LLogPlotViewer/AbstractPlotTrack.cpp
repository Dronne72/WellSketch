#include "AbstractPlotTrack.h"

#include <QLayout>

AbstractPlotTrack::AbstractPlotTrack(QWidget *parent) :
    QWidget(parent){
    this->setMinimumHeight(500);

    QGridLayout *grid = new QGridLayout(this);
    grid->setContentsMargins(0, 0, 0, 0);

    QFrame *main_frame = new QFrame(this);
    // main_frame->setStyleSheet("QFrame{ border: 1px solid gray }");

    grid->addWidget(main_frame, 0, 0);

    QGridLayout *frame_grid = new QGridLayout(main_frame);
    frame_grid->setContentsMargins(0, 0, 0, 0);

    m_customPlot = new QCustomPlot(this);
    m_customPlot->axisRect()->setRangeZoom(Qt::Vertical);
    m_customPlot->axisRect()->setRangeDrag(Qt::Vertical);
    m_customPlot->axisRect()->setMargins(QMargins(0, 0, 0, 0));
    m_customPlot->setOpenGl(true);

    frame_grid->addWidget(m_customPlot, 0, 0);
}
//-----------------------------------------------------------------------------
AbstractPlotTrack::~AbstractPlotTrack(){}
//-----------------------------------------------------------------------------
void AbstractPlotTrack::setYAxis(const QCPAxis *yAxis) {
    if(nullptr == yAxis) return;
    setYAxisRange(yAxis->range());
    setYAxisTicker(yAxis->ticker());
    setYAxisReversed(yAxis->rangeReversed());
}
//-----------------------------------------------------------------------------
QCPAxis *AbstractPlotTrack::getYAxis() const {
    if(m_customPlot->graphCount() > 0) {
        return m_customPlot->graph()->keyAxis();
    } else {
        return m_customPlot->yAxis;
    }
}
//-----------------------------------------------------------------------------
void AbstractPlotTrack::setOpenGl(bool use_opengl) {
    if(nullptr != m_customPlot) {
        m_customPlot->setOpenGl(use_opengl);
        m_customPlot->replot();
    }
}
//-----------------------------------------------------------------------------
QMap<double, QColor> AbstractPlotTrack::colorGradient() const {
    return m_colorGradient.colorStops();
}
//-----------------------------------------------------------------------------
QCPRange AbstractPlotTrack::globalDepthRange() const {
    return QCPRange(m_globalDepthLower, m_globalDepthUpper);
}
//-----------------------------------------------------------------------------
void AbstractPlotTrack::onYAxisRangeChanged(const QCPRange &range) {
    setYAxisRange(range);
    m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}
//-----------------------------------------------------------------------------
int AbstractPlotTrack::graphsCount() {
    return m_customPlot->graphCount();
}

