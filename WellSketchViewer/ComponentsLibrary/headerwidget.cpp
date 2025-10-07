#include "headerwidget.h"

// Реализация HeaderWidget
HeaderWidget::HeaderWidget(const QString& nameHeader, QWidget *parent)
    : QWidget(parent),
    m_nameHeader(nameHeader)
{
    setFixedHeight(30);
    setStyleSheet("background-color: #808080; color: white;");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(15, 0, 0, 0);

    QLabel *title = new QLabel(m_nameHeader);
    title->setStyleSheet("color: white; font-size: 16px; font-weight: bold;");

    layout->addWidget(title);
    layout->addStretch();
}
