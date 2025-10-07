#ifndef CONTINUOUSPLOTTRACK_H
#define CONTINUOUSPLOTTRACK_H

#include "AbstractPlotTrack.h"

#include <QObject>

class ContinuousPlotItem;

class ContinuousPlotTrack final : public AbstractPlotTrack {
    Q_OBJECT
public:
    ContinuousPlotTrack(QWidget *parent = nullptr);
    virtual ~ContinuousPlotTrack() override;

    QCPGraph *addContinuousPlot(const QJsonObject &continuous_obj);
    QCPGraph *addContinuousPlot(ContinuousPlotItem *continuous_item);

    void setLineWidth(int width);
    void setLineStyle(int style);

protected:
    void setYAxisRange(const QCPRange &range) override;
    void setYAxisRange(double lower, double upper) override;
    void setYAxisTicker(QSharedPointer<QCPAxisTicker> ticker) override;
    void setYAxisReversed(bool reversed) override;
    void setXAxisRange(const QCPRange &range) override;
    void setXAxisRange(double lower, double upper) override;
    void setXAxisReversed(bool reversed) override;

private:
    QMap<QString, ContinuousPlotItem *> m_logCurvesMap;
    QMap<QString, QCPGraph *> m_graphsMap;
};

#endif // CONTINUOUSPLOTTRACK_H
