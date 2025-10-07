#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>

#include "qcustomplot.h"

class AbstractPlotTrack;
class DepthScaleTrack;
class ContinuousPlotTrack;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();
    void addContinuousTrack(const QJsonObject &curve_obj);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void createDepthAxis();

    // Synchronize the Plots with each other
    void synchronize(const AbstractPlotTrack *sender);

    void resizeRows();
    void resizeColumns();

private slots:
    void onRangeChanged(const QCPRange &newRange,
                        const QCPRange &oldRange);

signals:
    void yAxisRangeChanged(const QCPRange &newRange);

private:
    Ui::MainWindow *ui;
    DepthScaleTrack *m_depthAxisTrack = nullptr;
    QTableWidget *m_depthScaleTable = nullptr;
    QTableWidget *m_logPlotTable = nullptr;
    QMap<int, AbstractPlotTrack *> m_plotsMap;

    qreal m_depthStep = 10;
    bool m_columnsAutoResize = true;
};
#endif // MAINWINDOW_H
