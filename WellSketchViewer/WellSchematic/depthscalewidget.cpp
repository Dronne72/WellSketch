#include "DepthScaleWidget.h"
#include <QPainter>
#include <QDebug>
#include <QLocale>

DepthScaleWidget::DepthScaleWidget(QWidget *parent)
    : QWidget(parent), m_maxDepth(3000)
{
    setFixedWidth(80);
    setMinimumHeight(600);

    // Default depth values from image
    m_depths = {0, 101, 343, 606, 667, 909, 1700, 1920, 1966, 2080, 2089, 2119, 2178, 2950};
}

void DepthScaleWidget::setDepthValues(const QVector<double> &depths)
{
    m_depths = depths;
    if (!m_depths.isEmpty()) {
        m_maxDepth = m_depths.last();
    }
    update();
}

void DepthScaleWidget::setMaxDepth(double maxDepth)
{
    m_maxDepth = maxDepth;
    update();
}

void DepthScaleWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Fill background
    painter.fillRect(rect(), Qt::white);

    // Draw border
    painter.setPen(QPen(Qt::black, 2));
    painter.drawRect(rect().adjusted(1, 1, -1, -1));

    if (m_depths.isEmpty()) return;

    // Calculate scale factor
    double scaleFactor = (height() - 40) / m_maxDepth;

    // Draw title
    painter.setPen(Qt::black);
    QFont titleFont = painter.font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(QRect(0, 10, width(), 30), Qt::AlignCenter, "Ft");

    // Draw "Cased Hole" subtitle
    QFont subtitleFont = painter.font();
    subtitleFont.setPointSize(10);
    painter.setFont(subtitleFont);
    painter.drawText(QRect(0, 35, width(), 25), Qt::AlignCenter, "Cased Hole");

    // Draw depth markers
    painter.setPen(QPen(Qt::black, 1));
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    for (double depth : m_depths) {
        int y = static_cast<int>(depth * scaleFactor) + 60;

        if (y > height() - 20) continue;

        // Draw tick mark
        painter.drawLine(width() - 20, y, width() - 5, y);

        // Draw depth value with comma formatting
        QString depthText = QLocale().toString(depth, 'f', 0);
        QRect textRect(5, y - 10, width() - 25, 20);
        painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter, depthText);

        // Draw horizontal line
        painter.setPen(QPen(QColor(150, 150, 150), 1));
        painter.drawLine(0, y, width(), y);
        painter.setPen(QPen(Qt::black, 1));
    }
}
