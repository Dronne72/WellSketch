#include "wellschematictree.h"
#include <QStandardItemModel>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include <QFileInfo>
#include "../PropertiesWidget/PropertiesView.h"

WellSchematicTree::WellSchematicTree(QWidget *parent)
    : QTreeView(parent)
{
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(DropOnly);
    setSelectionMode(SingleSelection);
    setHeaderHidden(true);

    // Создаем модель для дерева
    m_model = new QStandardItemModel(this);
    setModel(m_model);

    // Добавляем корневой элемент
    WellComponentItem *rootItem = new WellComponentItem("Well Schematic", "Project");
    m_model->appendRow(rootItem);

    expandAll();

    // Connect selection changes
    connect(selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &WellSchematicTree::onSelectionChanged);
}

void WellSchematicTree::setPropertiesView(PropertiesView *propertiesView)
{
    m_propertiesView = propertiesView;
}

void WellSchematicTree::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected)

    if (selected.indexes().isEmpty()) {
        if (m_propertiesView) {
            m_propertiesView->clearProperties();
        }
        return;
    }

    QModelIndex selectedIndex = selected.indexes().first();
    WellComponentItem *item = dynamic_cast<WellComponentItem*>(m_model->itemFromIndex(selectedIndex));

    if (item && m_propertiesView) {
        m_propertiesView->displayComponentProperties(item);
    }

    emit componentSelected(item);
}

WellComponentItem* WellSchematicTree::createWellComponent(const QString &name)
{
    WellComponentItem *item = new WellComponentItem(name, name);

    // Set default properties
    item->setComponentName(name);

    // Set unique color icon based on component name
    QColor color = getColorForComponent(name);
    item->setComponentColor(color);
    item->setIcon(createColorItemIcon(color));

    // Set default dimensions
    item->setTop(0.0);
    item->setBottom(1000.0);
    item->setInnerDiameter(5.0);
    item->setOuterDiameter(6.0);
    item->setCasingShoe("");
    item->setFillPattern(static_cast<int>(DelegatePatternFill::NoPattern));

    return item;
}

QColor WellSchematicTree::getColorForComponent(const QString &componentName)
{
    // Map component names to specific colors
    static const QHash<QString, QColor> colorMap = {
        {"Blast joint", QColor(255, 0, 0)},      // Red
        {"Casing", QColor(0, 0, 255)},           // Blue
        {"Cement", QColor(139, 69, 19)},         // Brown
        {"Cement Plug", QColor(160, 82, 45)},    // Sienna
        {"Centralize", QColor(0, 255, 0)},       // Green
        {"Collar", QColor(255, 165, 0)},         // Orange
        {"Double Tubing", QColor(128, 0, 128)},  // Purple
        {"Fish", QColor(255, 192, 203)},         // Pink
        {"Fluid", QColor(0, 255, 255)},          // Cyan
        {"Gas Lift Mandrel", QColor(255, 215, 0)}, // Gold
        {"Landing nipple", QColor(165, 42, 42)}, // Brown
        {"Open Hole", QColor(210, 180, 140)},    // Tan
        {"Packer", QColor(255, 0, 255)},         // Magenta
        {"Perforations", QColor(128, 128, 0)},   // Olive
        {"Pump", QColor(0, 128, 128)},           // Teal
        {"Screen", QColor(75, 0, 130)},          // Indigo
        {"SSD", QColor(47, 79, 79)},             // Dark Slate Gray
        {"TD", QColor(220, 20, 60)},             // Crimson
        {"TRSSV", QColor(30, 144, 255)},         // Dodger Blue
        {"Tubing", QColor(50, 205, 50)},         // Lime Green
        {"Wireline Entry", QColor(148, 0, 211)}, // Dark Violet
        {"Y-tool", QColor(255, 140, 0)}          // Dark Orange
    };

    // Return specific color if found, otherwise generate from hash
    if (colorMap.contains(componentName)) {
        return colorMap[componentName];
    }

    // Generate consistent color from string hash for unknown components
    uint hash = qHash(componentName);
    return QColor::fromHsv(hash % 360, 200 + hash % 56, 150 + hash % 106);
}

void WellSchematicTree::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-component")) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void WellSchematicTree::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-component")) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void WellSchematicTree::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-component")) {
        QString componentName = QString::fromUtf8(event->mimeData()->data("application/x-component"));
        QString iconPath = event->mimeData()->property("iconPath").toString();

        QFileInfo fileInfo(iconPath);
        QString iconName = fileInfo.baseName();

        QModelIndex dropIndex = indexAt(event->pos());
        QStandardItem *parentItem = m_model->itemFromIndex(dropIndex);

        if (!parentItem) {
            parentItem = m_model->invisibleRootItem()->child(0); // Well Schematic
        }

        // Create new component with properties
        WellComponentItem *newItem = createWellComponent(componentName);
        newItem->setEditable(true);

        // Store original icon path if needed
        if (!iconName.isEmpty()) {
            newItem->setData(iconName, Qt::UserRole + 1);
        }

        // Determine insertion position
        if (event->keyboardModifiers() & Qt::ControlModifier) {
            // Ctrl - insert between elements
            if (dropIndex.isValid() && dropIndex.parent().isValid()) {
                QStandardItem *siblingItem = m_model->itemFromIndex(dropIndex);
                QStandardItem *actualParent = siblingItem->parent();
                int row = siblingItem->row();
                actualParent->insertRow(row, newItem);
            } else {
                parentItem->appendRow(newItem);
            }
        } else {
            // Normal drop - add as child
            parentItem->appendRow(newItem);
        }

        event->acceptProposedAction();
        expandAll();

        // Select the new item and show properties
        setCurrentIndex(newItem->index());
        if (m_propertiesView) {
            m_propertiesView->displayComponentProperties(newItem);
        }

        emit treeChanged();

        qDebug() << "Component" << componentName << "added to tree with color:" << getColorForComponent(componentName).name();
    } else {
        event->ignore();
    }
}

QIcon WellSchematicTree::createColorItemIcon(const QColor &color)
{
    QPixmap pixmap(24, 24);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw colored rectangle with rounded corners
    painter.setBrush(color);
    painter.setPen(QPen(Qt::black, 1));
    painter.drawRoundedRect(2, 2, 20, 20, 4, 4);

    // Add a small border
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(Qt::darkGray, 1));
    painter.drawRoundedRect(2, 2, 20, 20, 4, 4);

    return QIcon(pixmap);
}

WellComponentItem* WellSchematicTree::getSelectedItem() const
{
    QModelIndexList selected = selectionModel()->selectedIndexes();
    if (selected.isEmpty()) return nullptr;

    return dynamic_cast<WellComponentItem*>(m_model->itemFromIndex(selected.first()));
}

QColor WellSchematicTree::getComponentColor(WellComponentItem *item) const
{
    if (!item) return Qt::black;
    return item->componentColor();
}

void WellSchematicTree::updateComponentIcon(WellComponentItem *item, const QColor &color)
{
    if (item) {
        item->setIcon(createColorItemIcon(color));
    }
}
