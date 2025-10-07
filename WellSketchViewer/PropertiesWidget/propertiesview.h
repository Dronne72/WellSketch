#ifndef PROPERTIESVIEW_H
#define PROPERTIESVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include "PropertyEditor.h"
#include "WellComponentItem.h"

class WellSchematicTree; // Forward declaration

class PropertiesView : public QWidget
{
    Q_OBJECT

public:
    explicit PropertiesView(QWidget *parent = nullptr);

    void displayComponentProperties(WellComponentItem *component);
    void clearProperties();
    void setSchematicTree(WellSchematicTree *tree) { m_schematicTree = tree; }
    PropertyEditor* propertyEditor() const { return m_propertyEditor; }

private slots:
    void onPropertyChanged(QTreeWidgetItem *item, const QVariant &value);
    void onPropertyDoubleClicked(QTreeWidgetItem *item);

private:
    void setupUI();
    QIcon createColorIcon(const QColor &color);

private:
    PropertyEditor *m_propertyEditor;
    WellComponentItem *m_currentComponent = nullptr;
    WellSchematicTree *m_schematicTree = nullptr;

    // Property items for easy access
    QTreeWidgetItem *m_nameItem = nullptr;
    QTreeWidgetItem *m_colorItem = nullptr;
    QTreeWidgetItem *m_topItem = nullptr;
    QTreeWidgetItem *m_bottomItem = nullptr;
    QTreeWidgetItem *m_innerDiaItem = nullptr;
    QTreeWidgetItem *m_outerDiaItem = nullptr;
    QTreeWidgetItem *m_casingShoeItem = nullptr;
    QTreeWidgetItem *m_fillItem = nullptr;
};

#endif // PROPERTIESVIEW_H
