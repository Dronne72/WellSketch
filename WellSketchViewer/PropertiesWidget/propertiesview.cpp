#include "PropertiesView.h"
#include <QDebug>
#include "../WellSchematicTree/wellschematictree.h"

PropertiesView::PropertiesView(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

void PropertiesView::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // Create property editor
    m_propertyEditor = new PropertyEditor(this);

    // Минимальные отступы для компактного вида
    m_propertyEditor->setPropertyGeometry(20, 2, 2);

    // Дополнительные настройки для более компактного отображения
    m_propertyEditor->setIndentation(8); // Уменьшаем отступ для дерева

    layout->addWidget(m_propertyEditor);

    // Connect signals
    connect(m_propertyEditor, &PropertyEditor::propertyChanged,
            this, &PropertiesView::onPropertyChanged);
    connect(m_propertyEditor, &PropertyEditor::propertyDoubleClicked,
            this, &PropertiesView::onPropertyDoubleClicked);
}

void PropertiesView::displayComponentProperties(WellComponentItem *component)
{
    if (!component) {
        clearProperties();
        return;
    }

    m_currentComponent = component;

    // Clear existing properties
    m_propertyEditor->clear();

    // Create categories and properties for the component

    // Context category with blue header
    QTreeWidgetItem *contextCategory = m_propertyEditor->addCategory("Context");
    contextCategory->setBackground(0, QColor(173, 216, 230));
    contextCategory->setBackground(1, QColor(173, 216, 230));

    // Type property (read-only)
    m_propertyEditor->addProperty(
        "Type", component->componentType(), PropertyDelegate::String, contextCategory, true);

    // Name property (editable)
    m_nameItem = m_propertyEditor->addProperty(
        "Name", component->componentName(), PropertyDelegate::String, contextCategory);

    // Set from Specs property
    QTreeWidgetItem *specsItem = m_propertyEditor->addProperty(
        "Set from Specs", 6, PropertyDelegate::Integer, contextCategory);

    QStringList specsOptions = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
    m_propertyEditor->setPropertyValue(specsItem, specsOptions, true);

    // Dimensions properties
    m_topItem = m_propertyEditor->addProperty(
        "Top", component->top(),
        PropertyDelegate::Double, contextCategory);

    m_bottomItem = m_propertyEditor->addProperty(
        "Bottom", component->bottom(),
        PropertyDelegate::Double, contextCategory);

    m_innerDiaItem = m_propertyEditor->addProperty(
        "Inner Diameter", component->innerDiameter(),
        PropertyDelegate::Double, contextCategory);

    m_outerDiaItem = m_propertyEditor->addProperty(
        "Outer Diameter", component->outerDiameter(),
        PropertyDelegate::Double, contextCategory);

    m_propertyEditor->addProperty(
        "Roughness", "NA", PropertyDelegate::String, contextCategory, true);

    m_casingShoeItem = m_propertyEditor->addProperty(
        "Casing shoe", component->casingShoe(),
        PropertyDelegate::String, contextCategory);

    // View category with blue header
    QTreeWidgetItem *viewCategory = m_propertyEditor->addCategory("View");
    viewCategory->setBackground(0, QColor(173, 216, 230));
    viewCategory->setBackground(1, QColor(173, 216, 230));

    // Color property
    m_colorItem = m_propertyEditor->addProperty(
        "Color", component->componentColor(),
        PropertyDelegate::Color, viewCategory);

    // Fill property with pattern
    m_fillItem = m_propertyEditor->addProperty(
        "Fill", component->fillPattern(),
        PropertyDelegate::Pattern, viewCategory);

    QList<DelegatePatternFill::PatternData> patterns = DelegatePatternFill::defaultPatterns();
    m_propertyEditor->setPropertyValue(m_fillItem, QVariant::fromValue(patterns), true);

    // Очищаем все красные отметки изменений
    m_propertyEditor->clearChangedMarksRecursive();

    // Expand all categories
    m_propertyEditor->expandAll();
}

void PropertiesView::clearProperties()
{
    m_currentComponent = nullptr;
    m_propertyEditor->clear();
}

void PropertiesView::onPropertyChanged(QTreeWidgetItem *item, const QVariant &value)
{
    if (!m_currentComponent) return;

    qDebug() << "Property changed:" << item->text(0) << "=" << value;

    // Update component data based on which property changed
    if (item == m_nameItem) {
        m_currentComponent->setComponentName(value.toString());
        m_currentComponent->setText(value.toString());
    }
    else if (item == m_colorItem) {
        QColor newColor = value.value<QColor>();
        m_currentComponent->setComponentColor(newColor);

        // Update icon in the tree
        if (m_schematicTree) {
            m_schematicTree->updateComponentIcon(m_currentComponent, newColor);
        }
    }
    else if (item == m_topItem) {
        m_currentComponent->setTop(value.toDouble());
    }
    else if (item == m_bottomItem) {
        m_currentComponent->setBottom(value.toDouble());
    }
    else if (item == m_innerDiaItem) {
        m_currentComponent->setInnerDiameter(value.toDouble());
    }
    else if (item == m_outerDiaItem) {
        m_currentComponent->setOuterDiameter(value.toDouble());
    }
    else if (item == m_casingShoeItem) {
        m_currentComponent->setCasingShoe(value.toString());
    }
    else if (item == m_fillItem) {
        if (value.canConvert<DelegatePatternFill::PatternData>()) {
            auto pattern = value.value<DelegatePatternFill::PatternData>();
            m_currentComponent->setFillPattern(static_cast<int>(pattern.pattern));
        }
    }
}

void PropertiesView::onPropertyDoubleClicked(QTreeWidgetItem *item)
{
    qDebug() << "Property double-clicked:" << item->text(0);
}

QIcon PropertiesView::createColorIcon(const QColor &color)
{
    QPixmap pixmap(16, 16);
    pixmap.fill(color);
    return QIcon(pixmap);
}
