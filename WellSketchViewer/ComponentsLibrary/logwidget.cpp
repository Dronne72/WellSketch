#include "logwidget.h"

LogWidget::LogWidget(const QString& message, QWidget *parent)
    : QWidget(parent),
    m_message(message)
{
    setFixedHeight(80);
    setStyleSheet("background-color: #C0C0C0; color: white;");

    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->setContentsMargins(15, 0, 0, 0);

    QTextEdit *textEdit = new QTextEdit(m_message);

    hlayout->addWidget(textEdit);
    hlayout->addStretch();
}
