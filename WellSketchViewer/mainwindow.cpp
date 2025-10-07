#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "AbstractPlotTrack.h"
#include "DepthScaleTrack.h"
#include "ContinuousPlotItem.h"
#include "ContinuousPlotTrack.h"

#include <QFile>
#include <QJsonDocument>
#include "ComponentsLibrary/componentlibrarywidget.h"
#include "WellSchematicTree/wellschematictree.h"
#include "WellSchematic/wellopenglwidget.h"
#include "PropertiesWidget/propertiesview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Well Schematic Constructor");

    ComponentLibraryWidget *CLwidget = new ComponentLibraryWidget(this);
    ui->plots_layout->addWidget(CLwidget, 0);

    // PropertiesWidget  *PRwidget = new PropertiesWidget(this);
    // ui->plots_layout->addWidget(PRwidget, 1);

    // PropertiesView *PRwidget = new PropertiesView(this);
    // ui->plots_layout->addWidget(PRwidget, 1);

    WellSchematicTree *m_wellTree = new WellSchematicTree(this);

    WellOpenGLWidget* WELLglWGT = new WellOpenGLWidget(this);
    ui->plots_layout->addWidget(WELLglWGT, 1);
    // WellSchematicViewWidget  *WShwidget = new WellSchematicViewWidget(this);
    // ui->plots_layout->addWidget(WShwidget, 1);
}
//-----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void MainWindow::init() {
    createDepthAxis();

    QFile file(":/data/resources/test_curve.json");
    if(file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        auto test_curve_obj = doc.object();
        addContinuousTrack(test_curve_obj);

        file.close();
    }
}
//-----------------------------------------------------------------------------
void MainWindow::addContinuousTrack(const QJsonObject &curve_obj) {
    if(nullptr == m_logPlotTable || nullptr == m_depthAxisTrack) {
        return;
    }

    auto curve_item = new ContinuousPlotItem(curve_obj);

    ContinuousPlotTrack *lp_track = new ContinuousPlotTrack(m_logPlotTable);
    auto track_header = curve_item->plotTitle();

    lp_track->setYAxis(m_depthAxisTrack->getYAxis());
    synchronize(lp_track);

    lp_track->addContinuousPlot(curve_item);

    m_logPlotTable->setColumnCount(m_logPlotTable->columnCount() + 1);
    m_logPlotTable->setHorizontalHeaderItem(
        m_plotsMap.size(), new QTableWidgetItem(track_header));

    int last_row = m_logPlotTable->rowCount() - 1;
    int last_column = m_plotsMap.size();
    m_logPlotTable->setCellWidget(last_row, last_column, lp_track);
    m_plotsMap.insert(last_column, lp_track);

    lp_track->setYAxis(m_depthAxisTrack->getYAxis());

    if(m_depthAxisTrack) {
        m_depthAxisTrack->onGlobalDepthChanged(lp_track->globalDepthRange());
    }

    resizeRows();
    resizeColumns();
}
//-----------------------------------------------------------------------------
void MainWindow::resizeEvent(QResizeEvent *event) {
    resizeRows();
    resizeColumns();
}
//-----------------------------------------------------------------------------
void MainWindow::createDepthAxis() {
    if(nullptr == m_depthAxisTrack) {
        QString style = QString("QTableWidget"
                                "{"
                                "gridline-color: gray; "
                                "background_color: lightgray"
                                "}"
                                ""
                                "QHeaderView::section { background-color: #b4c7e7; "
                                "color: black; "
                                "padding-left: 4px; "
                                "border: 1px solid gray; "
                                "font-weight: bold;"
                                "}");

        m_depthScaleTable = new QTableWidget(this);
        m_depthScaleTable->setStyleSheet(style);
        m_depthScaleTable->verticalHeader()->hide();
        m_depthScaleTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_depthScaleTable->setShowGrid(false);
        m_depthScaleTable->setSelectionMode(QTableWidget::NoSelection);

        // ui->plots_layout->addWidget(m_depthScaleTable, 0);

        m_depthAxisTrack = new DepthScaleTrack(this);

        auto range_reverse = true;
        // m_depthAxisTrack->getYAxis()->setRange(parameters.start, parameters.stop);
        m_depthAxisTrack->setGlobalDepthRange(0, 100);
        QSharedPointer<QCPAxisTickerFixed> intTicker(new QCPAxisTickerFixed);
        intTicker->setTickStep(10);
        intTicker->setScaleStrategy(QCPAxisTickerFixed::ssMultiples);
        m_depthAxisTrack->getYAxis()->setTicker(intTicker);
        m_depthAxisTrack->getYAxis()->setRangeReversed(range_reverse);

        synchronize(m_depthAxisTrack);

        m_depthScaleTable->setRowCount(1);
        m_depthScaleTable->setColumnCount(1);

        m_depthScaleTable->setHorizontalHeaderItem(0, new QTableWidgetItem(QString("DEPTH")));
        m_depthScaleTable->setCellWidget(0, 0, m_depthAxisTrack);
        m_depthScaleTable->setColumnWidth(0, m_depthAxisTrack->width());
        m_depthScaleTable->setMaximumWidth(m_depthAxisTrack->width());

        m_logPlotTable = new QTableWidget(this);
        m_logPlotTable->setSelectionMode(QTableWidget::NoSelection);
        m_logPlotTable->setStyleSheet(style);
        m_logPlotTable->verticalHeader()->hide();
        m_logPlotTable->horizontalHeader()->setSectionsMovable(true);
        m_logPlotTable->setRowCount(1);
        // ui->plots_layout->addWidget(m_logPlotTable, 1);
    }
}
//-----------------------------------------------------------------------------
void MainWindow::synchronize(const AbstractPlotTrack *sender) {
    connect(sender->getYAxis(), SIGNAL(rangeChanged(QCPRange,QCPRange)),
            this, SLOT(onRangeChanged(QCPRange,QCPRange)));

    connect(this, SIGNAL(yAxisRangeChanged(QCPRange)),
            sender, SLOT(onYAxisRangeChanged(QCPRange)));

    if(nullptr != m_depthAxisTrack) {
        connect(sender, SIGNAL(globalDepthChanged(QCPRange)),
                m_depthAxisTrack, SLOT(onGlobalDepthChanged(QCPRange)));
    }
}
//-----------------------------------------------------------------------------
void MainWindow::resizeRows() {
    if(nullptr == m_depthScaleTable || nullptr == m_logPlotTable) return;
    if(0 == m_logPlotTable->rowCount()) return;

    int rows_height = 0;
    int header_height = m_logPlotTable->horizontalHeader()->height();
    for(int row = 0; row < m_logPlotTable->rowCount() - 1; ++row) {
        rows_height += m_logPlotTable->rowHeight(row);
    }

    auto scroll_height = (m_logPlotTable->horizontalScrollBar()->isVisible())
                             ? m_logPlotTable->horizontalScrollBar()->height()
                             : 0;

    auto last_row = m_logPlotTable->rowCount() - 1;
    auto table_height = m_logPlotTable->height();
    auto last_row_height = table_height - header_height -
                           scroll_height - rows_height - 2;
    m_logPlotTable->setRowHeight(last_row, last_row_height);
    m_depthScaleTable->setRowHeight(last_row, last_row_height);
}
//-----------------------------------------------------------------------------
void MainWindow::resizeColumns() {
    if(nullptr == m_depthScaleTable || nullptr == m_logPlotTable) return;
    if(false == m_columnsAutoResize) return;
    if(m_logPlotTable->columnCount() == 0) return;

    int col_width = (m_logPlotTable->width() - 2)/m_logPlotTable->columnCount();
    if(col_width < 150) col_width = 150;
    for(int col = 0; col < m_logPlotTable->columnCount(); ++col) {
        m_logPlotTable->setColumnWidth(col, col_width);
    }
}
//-----------------------------------------------------------------------------
void MainWindow::onRangeChanged(const QCPRange &newRange,
                                const QCPRange &oldRange) {
    if(nullptr == m_depthAxisTrack) return;
    auto sender = QObject::sender();

    // Check Range
    QCPRange globalDepthRange = m_depthAxisTrack->globalDepthRange();
    QCPRange actualRange;

    if(newRange.lower < globalDepthRange.lower - (m_depthStep * 3)
        || newRange.upper > globalDepthRange.upper + (m_depthStep * 3)) {
        actualRange = oldRange;
    } else {
        actualRange = newRange;
    }

    if(sender != m_depthAxisTrack) {
        m_depthAxisTrack->setYAxisRange(actualRange.lower, actualRange.upper);
        m_depthScaleTable->setMinimumWidth(m_depthAxisTrack->width());
        m_depthScaleTable->setMaximumWidth(m_depthAxisTrack->width());
    }

    emit yAxisRangeChanged(actualRange);

    auto tracks_list = m_plotsMap.values();
    foreach(auto plots_wgt, tracks_list) {
        if(sender != plots_wgt) {
            plots_wgt->setYAxisRange(actualRange.lower, actualRange.upper);
        }
    }
}

void MainWindow::on_pB_cansel_clicked()
{
    close();
}

