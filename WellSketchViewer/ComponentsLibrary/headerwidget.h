#ifndef HEADERWIDGET_H
#define HEADERWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>


class HeaderWidget : public QWidget
{
public:
    explicit HeaderWidget(const QString& nameHeader, QWidget *parent = nullptr);

private:
    QString m_nameHeader;
};

#endif // HEADERWIDGET_H
