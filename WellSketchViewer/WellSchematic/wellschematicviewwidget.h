#ifndef WELLSCHEMATICVIEWWIDGET_H
#define WELLSCHEMATICVIEWWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include "../WellSchematicTree/wellschematictree.h"


class WellSchematicViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WellSchematicViewWidget(QWidget *parent = nullptr);

public slots:
    void updateSchematic();

private:
    void setupUI();
    void drawSchematic();

    QScrollArea *m_scrollArea;
    QWidget *m_schematicWidget;
};

#endif // WELLSCHEMATICVIEWWIDGET_H
