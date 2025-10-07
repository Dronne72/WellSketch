#ifndef WELLSCHEMATICTREE_H
#define WELLSCHEMATICTREE_H

#include <QTreeView>
#include <QStandardItemModel>
#include "../PropertiesWidget/WellComponentItem.h"

class PropertiesView; // Forward declaration

class WellSchematicTree : public QTreeView
{
    Q_OBJECT

public:
    explicit WellSchematicTree(QWidget *parent = nullptr);

    void setPropertiesView(PropertiesView *propertiesView);
    WellComponentItem* getSelectedItem() const;
    QColor getComponentColor(WellComponentItem *item) const;
    void updateComponentIcon(WellComponentItem *item, const QColor &color);

signals:
    void componentSelected(WellComponentItem *item);
    void treeChanged();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

private:
    WellComponentItem* createWellComponent(const QString &name);
    QColor getColorForComponent(const QString &componentName);
    QIcon createColorItemIcon(const QColor &color);

private:
    QStandardItemModel *m_model;
    PropertiesView *m_propertiesView = nullptr;
};

#endif // WELLSCHEMATICTREE_H
