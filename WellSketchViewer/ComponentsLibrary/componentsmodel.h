#ifndef COMPONENTSMODEL_H
#define COMPONENTSMODEL_H

#include <QStandardItemModel>

class ComponentsModel : public QStandardItemModel
{
    Q_OBJECT

public:
    explicit ComponentsModel(QObject *parent = nullptr);

    void loadComponents(const QVector<QPair<QString, QIcon>> &components);
};

#endif // COMPONENTSMODEL_H
