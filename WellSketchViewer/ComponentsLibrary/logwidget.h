#ifndef LOGWIDGET_H
#define LOGWIDGET_H


#include <QWidget>
#include <QHBoxLayout>
#include <QTextEdit>

class LogWidget : public QWidget
{
public:
    explicit LogWidget(const QString& message, QWidget *parent = nullptr);

private:
    QString m_message;
};

#endif // LOGWIDGET_H
