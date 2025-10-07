#include "componentsmodel.h"
#include <QStandardItem>

ComponentsModel::ComponentsModel(QObject *parent) : QStandardItemModel(parent) {}

void ComponentsModel::loadComponents(const QVector<QPair<QString, QIcon>> &components)
{
    clear();
    setColumnCount(2);
    setRowCount((components.size() + 1) / 2);

    for (int i = 0; i < components.size(); ++i) {
        int row = i / 2;
        int col = i % 2;

        QStandardItem *item = new QStandardItem(components[i].first);
        item->setIcon(components[i].second);
        item->setEditable(false);
        item->setData(components[i].first, Qt::UserRole); // Сохраняем имя компонента

        setItem(row, col, item);
    }
}
