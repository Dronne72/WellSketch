#ifndef DEPTHSCALEWIDGET_H
#define DEPTHSCALEWIDGET_H

#include <QWidget>
#include <QVector>

class DepthScaleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DepthScaleWidget(QWidget *parent = nullptr);
    void setDepthValues(const QVector<double> &depths);
    void setMaxDepth(double maxDepth);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<double> m_depths;
    double m_maxDepth;
};

#endif // DEPTHSCALEWIDGET_H
