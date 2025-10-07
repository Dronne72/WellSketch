#ifndef COMPONENTLIBRARYWIDGET_H
#define COMPONENTLIBRARYWIDGET_H


#include "ComponentsTableView.h"
#include "../WellSchematicTree/wellschematictree.h"
// #include "../PropertiesWidget/propertieswidget.h"
#include "IconTextDelegate.h"
#include "../PropertiesWidget/propertiesview.h"

class ComponentLibraryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ComponentLibraryWidget(QWidget *parent = nullptr);
    QIcon getComponentIcon(const QString &componentName) const;
    QStringList getAvailableComponents() const;

    void debugResourcePaths();

signals:
    void componentSelected(const QString &name, const QString &iconPath);

private slots:
    void onItemDoubleClicked(const QModelIndex &index);

private:
    void setupUI();
    void loadComponentsData();


    ComponentsTableView *tableView;
    QStandardItemModel *model;
    IconTextDelegate *delegate;
    WellSchematicTree *m_treeView;
    PropertiesView * PRwidget;


};

#endif // COMPONENTLIBRARYWIDGET_H
