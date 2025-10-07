#include "ContinuousPlotTrack.h"

#include "ContinuousPlotItem.h"

ContinuousPlotTrack::ContinuousPlotTrack(QWidget *parent) :
    AbstractPlotTrack(parent) {
    m_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    m_customPlot->legend->setVisible(false);
    // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
    m_customPlot->axisRect()->insetLayout()
        ->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);

    // setup for graph 0: key axis left, value axis bottom
    // will contain left maxwell-like function
    // m_customPlot->addGraph(m_customPlot->yAxis, m_customPlot->xAxis);
    m_customPlot->xAxis->setVisible(false);
    m_customPlot->yAxis->setTicks(false);
    m_customPlot->yAxis->setTickLabels(false);
    m_customPlot->yAxis->grid()->setVisible(true);
    m_customPlot->yAxis->setBasePen(QPen(QColor(Qt::transparent)));
    m_customPlot->axisRect()->setMargins(QMargins(0, 0, 0, 0));
    m_customPlot->yAxis->setPadding(2);
}
//----------------------------------------------------------------
ContinuousPlotTrack::~ContinuousPlotTrack() {
    qDeleteAll(m_logCurvesMap);
    m_logCurvesMap.clear();

    // All graph instances will be deleted using QCustomPlot.
    // No need to delete them here
    m_graphsMap.clear();
}
//-----------------------------------------------------------------------------
void ContinuousPlotTrack::setYAxisRange(const QCPRange &range) {
    setYAxisRange(range.lower, range.upper);
}
//-----------------------------------------------------------------------------
void ContinuousPlotTrack::setYAxisRange(double lower, double upper) {
    // Block the signals to avoid a recursive call
    for(int i = 0; i < m_customPlot->graphCount(); ++i) {
        m_customPlot->graph(i)->keyAxis()->blockSignals(true);
        m_customPlot->graph(i)->keyAxis()->setRange(lower, upper);
        m_customPlot->graph(i)->keyAxis()->blockSignals(false);
    }
    m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}
//-----------------------------------------------------------------------------
void ContinuousPlotTrack::setYAxisTicker(QSharedPointer<QCPAxisTicker> ticker) {
    // Block the signals to avoid a recursive call
    for(int i = 0; i < m_customPlot->graphCount(); ++i) {
        m_customPlot->graph(i)->keyAxis()->blockSignals(true);
        m_customPlot->graph(i)->keyAxis()->setTicker(ticker);
        m_customPlot->graph(i)->keyAxis()->blockSignals(false);
    }
}
//-----------------------------------------------------------------------------
void ContinuousPlotTrack::setYAxisReversed(bool reversed) {
    // Block the signals to avoid a recursive call
    for(int i = 0; i < m_customPlot->graphCount(); ++i) {
        m_customPlot->graph(i)->keyAxis()->blockSignals(true);
        m_customPlot->graph(i)->keyAxis()->setRangeReversed(reversed);
        m_customPlot->graph(i)->keyAxis()->blockSignals(false);
    }
}
//-----------------------------------------------------------------------------
void ContinuousPlotTrack::setXAxisRange(const QCPRange &range) {
    setYAxisRange(range.lower, range.upper);
}
//-----------------------------------------------------------------------------
void ContinuousPlotTrack::setXAxisRange(double lower, double upper) {
    // Block the signals to avoid a recursive call
    for(int i = 0; i < m_customPlot->graphCount(); ++i) {
        m_customPlot->graph(i)->keyAxis()->blockSignals(true);
        m_customPlot->graph(i)->valueAxis()->setRange(lower, upper);
        m_customPlot->graph(i)->keyAxis()->blockSignals(false);
    }
    m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}
//-----------------------------------------------------------------------------
void ContinuousPlotTrack::setXAxisReversed(bool reversed) {
    // Block the signals to avoid a recursive call
    for(int i = 0; i < m_customPlot->graphCount(); ++i) {
        m_customPlot->graph(i)->keyAxis()->blockSignals(true);
        m_customPlot->graph(i)->valueAxis()->setRangeReversed(reversed);
        m_customPlot->graph(i)->keyAxis()->blockSignals(false);
    }
    m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}
//-----------------------------------------------------------------------------
QCPGraph *ContinuousPlotTrack::addContinuousPlot(const QJsonObject &continuous_obj) {
    auto curve_item = new ContinuousPlotItem(continuous_obj);
    return addContinuousPlot(curve_item);
}
//-----------------------------------------------------------------------------
QCPGraph *ContinuousPlotTrack::addContinuousPlot(ContinuousPlotItem *continuous_item) {
    auto graph = m_customPlot->addGraph(m_customPlot->yAxis, m_customPlot->xAxis);
    graph->setAdaptiveSampling(true);
    graph->setName(continuous_item->getDatasetProperties().dataType);

    auto curve_points = continuous_item->сurvePoins();
    auto points_count = curve_points.count();
    // QVector<double> x0(points_count), y0(points_count);
    QVector<double> x0, y0;
    qreal y_start = continuous_item->getDatasetProperties().start;
    qreal y_stop = continuous_item->getDatasetProperties().stop;
    qreal y_step = continuous_item->getDatasetProperties().step;

    for(int i = 0; i < points_count; ++i) {
        auto value = curve_points[i];
        if(value != kSystemUndefinedData) {
            y0.append(value);
            x0.append(y_start + (y_step * i));
        }
    }

    int graph_count = m_customPlot->graphCount();

    m_customPlot->xAxis->setRange(*std::min_element(x0.begin(), x0.end()),
                                  *std::max_element(x0.begin(), x0.end()));
    if(graph_count == 1) {
        m_globalDepthLower = *std::min_element(x0.begin(), x0.end());
        m_globalDepthUpper = *std::max_element(x0.begin(), x0.end());
    }
    auto depth_lower = *std::min_element(x0.begin(), x0.end());
    auto depth_upper = *std::max_element(x0.begin(), x0.end());

    if(depth_lower < m_globalDepthLower) m_globalDepthLower = depth_lower;
    if(depth_upper > m_globalDepthUpper) m_globalDepthUpper = depth_upper;

    auto values_lower = *std::min_element(y0.begin(), y0.end());
    auto values_upper = *std::max_element(y0.begin(), y0.end());

    if(m_customPlot->graphCount() == 1) {
        m_globalValuesLower = values_lower;
        m_globalValuesUpper = values_upper;
    } else {
        if(values_lower < m_globalValuesLower) m_globalValuesLower = values_lower;
        if(values_upper > m_globalValuesUpper) m_globalValuesUpper = values_upper;
    }

    m_customPlot->xAxis->setRange(m_globalDepthLower, m_globalDepthUpper);
    graph->setData(x0, y0);

    int color_index = graph_count;
    m_logCurvesMap.insert(continuous_item->curveID(), continuous_item);
    m_graphsMap.insert(continuous_item->curveID(), graph);
    m_customPlot->replot();

    QColor color = QColor(Qt::blue);

    QPen pen = QPen(color);
    pen.setWidth(1);
    graph->setPen(pen);
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone, 1));
    graph->rescaleAxes(false);

    setXAxisRange(m_globalValuesLower, m_globalValuesUpper);
    emit globalDepthChanged(QCPRange(m_globalDepthLower, m_globalDepthUpper));

    return graph;
}
//-----------------------------------------------------------------------------
void ContinuousPlotTrack::setLineWidth(int width) {
    for(int i = 0; i < m_customPlot->graphCount(); ++i) {
        auto graph = m_customPlot->graph(i);
        auto pen = graph->pen();
        pen.setWidth(width);
        graph->setPen(pen);
    }
    m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}
//-----------------------------------------------------------------------------
void ContinuousPlotTrack::setLineStyle(int style) {
    for(int i = 0; i < m_customPlot->graphCount(); ++i) {
        auto graph = m_customPlot->graph(i);
        graph->setLineStyle(QCPGraph::lsNone);
        graph->setScatterStyle(
            QCPScatterStyle((QCPScatterStyle::ScatterShape)style, graph->pen().width()));
    }
    m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}
