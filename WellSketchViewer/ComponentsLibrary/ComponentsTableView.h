#ifndef COMPONENTSTABLEVIEW_H
#define COMPONENTSTABLEVIEW_H

#include <QTableView>
#include <QDrag>
#include <QMimeData>

class ComponentsTableView : public QTableView
{
    Q_OBJECT

public:
    explicit ComponentsTableView(QWidget *parent = nullptr);

    // QMimeData *mimeData(const QModelIndexList &indexes) const;
    // Qt::DropActions supportedDropActions() const;
    // QStringList mimeTypes() const;
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QPoint dragStartPosition;

};

#endif // COMPONENTSTABLEVIEW_H
