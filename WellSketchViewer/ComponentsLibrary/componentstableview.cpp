#include "ComponentsTableView.h"
#include <QMouseEvent>
#include <QMimeData>
#include <QApplication>
#include <QDrag>

ComponentsTableView::ComponentsTableView(QWidget *parent)
    : QTableView(parent)
{
    setDragEnabled(true);
    setSelectionMode(SingleSelection);
}

void ComponentsTableView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragStartPosition = event->pos();
    }
    QTableView::mousePressEvent(event);
}

void ComponentsTableView::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }

    if ((event->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance()) {
        return;
    }

    QModelIndex index = indexAt(dragStartPosition);
    if (!index.isValid()) {
        return;
    }

    // Создаем drag операцию
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    // Получаем данные из модели
    QString componentName = model()->data(index, Qt::DisplayRole).toString();
    QString iconPath = model()->data(index, Qt::UserRole + 1).toString();

    // Сохраняем данные в MIME
    mimeData->setData("application/x-component", componentName.toUtf8());
    mimeData->setText(componentName);
    mimeData->setProperty("iconPath", iconPath);

    drag->setMimeData(mimeData);

    // Устанавливаем иконку для drag операции
    QPixmap pixmap(32, 32);
    if (QPixmap testPixmap; testPixmap.load(iconPath)) {
        pixmap = testPixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    } else {
        pixmap.fill(Qt::lightGray);
    }
    drag->setPixmap(pixmap);
    drag->setHotSpot(QPoint(15, 15));

    // Начинаем drag операцию
    drag->exec(Qt::CopyAction | Qt::MoveAction);
}

// // В ComponentsTableView (или аналогичном классе)
// QMimeData* ComponentsTableView::mimeData(const QModelIndexList &indexes) const
// {
//     QMimeData *mimeData = new QMimeData();//QTableView::mimeData(indexes);

//     if (!indexes.isEmpty()) {
//         QModelIndex index = indexes.first();
//         QString componentName = index.data(Qt::DisplayRole).toString();
//         QString iconPath = index.data(Qt::UserRole + 1).toString();

//         mimeData->setData("application/x-component", componentName.toUtf8());
//         mimeData->setProperty("iconPath", iconPath);
//     }

//     return mimeData;
// }

// Qt::DropActions ComponentsTableView::supportedDropActions() const
// {
//     return Qt::CopyAction;
// }

// QStringList ComponentsTableView::mimeTypes() const
// {
//     QStringList types;
//     types << "application/x-component";
//     return types;
// }
