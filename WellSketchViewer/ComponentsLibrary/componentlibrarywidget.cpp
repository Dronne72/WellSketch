#include "ComponentLibraryWidget.h"
#include <QMessageBox>
#include "HeaderWidget.h"
#include "logwidget.h"

// Реализация ComponentLibraryWidget
ComponentLibraryWidget::ComponentLibraryWidget(QWidget *parent)
    : QWidget(parent)
    , tableView(nullptr)
    , model(nullptr)
    , delegate(nullptr)
{
    setupUI();
    loadComponentsData();

    // loadComponentsModel();
}

void ComponentLibraryWidget::debugResourcePaths()
{
    QStringList testPaths = {
        ":/res/Component Library/Blast joint.png",
        ":/res/Component Library/Casing.png",

    };

    qDebug() << "Checking resource paths:";
    for (const QString &path : testPaths) {
        bool exists = QFile::exists(path);
        QPixmap pixmap;
        bool canLoad = pixmap.load(path);

        qDebug() << path << "- Exists:" << exists << "- Can load:" << canLoad;
    }
}


void ComponentLibraryWidget::setupUI()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *componentLayout = new QVBoxLayout();
    componentLayout->setContentsMargins(5, 5, 5, 5);

    // Заголовок
    HeaderWidget *header = new HeaderWidget("Components Library", this);
    componentLayout->addWidget(header);

    //
    tableView = new ComponentsTableView();  // ИЗМЕНЕНО
    model = new QStandardItemModel(0, 3, this);
    delegate = new IconTextDelegate(this);

    // Настройка таблицы
    tableView->setModel(model);
    tableView->setItemDelegate(delegate);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    tableView->horizontalHeader()->setVisible(false);
    tableView->verticalHeader()->setVisible(false);
    tableView->setShowGrid(true);
    tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Настройка столбцов
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->verticalHeader()->setDefaultSectionSize(120);

    connect(tableView, &QTableView::doubleClicked,
            this, &ComponentLibraryWidget::onItemDoubleClicked);

    componentLayout->addWidget(tableView);

    LogWidget *logComponentLib = new LogWidget("Selected");
    componentLayout->addWidget(logComponentLib);

    QVBoxLayout *wellViewLayout = new QVBoxLayout();
    wellViewLayout->setContentsMargins(5, 5, 5, 5);

    HeaderWidget *headerWell = new HeaderWidget("Well Schematic Tree", this);
    wellViewLayout->addWidget(headerWell);

    m_treeView = new WellSchematicTree;
    m_treeView->setStyleSheet("QTreeView { border: 1px solid #cccccc; background-color: white; }");
    wellViewLayout->addWidget(m_treeView);
     LogWidget *logtreeView = new LogWidget("Selected");
     wellViewLayout->addWidget(logtreeView);

    QVBoxLayout *propertyLayout = new QVBoxLayout();
    propertyLayout->setContentsMargins(5, 5, 5, 5);

    HeaderWidget *headerProp = new HeaderWidget("Propertis", this);
    propertyLayout->addWidget(headerProp);

    // m_propertiesPanel =  new PropertyEditor(this);

    // m_propertiesPanel = new PropertiesPanel(this);
    // m_treeView->setPropertiesPanel(m_propertiesPanel);
    PRwidget = new PropertiesView(this);
    propertyLayout->addWidget(PRwidget);

    // женим их
    m_treeView->setPropertiesView(PRwidget);
    PRwidget->setSchematicTree(m_treeView);

    // connect(m_propertiesPanel, &PropertyEditor::propertyChanged,
    //         m_treeView, &WellSchematicTree::treeChanged);

    // connect(m_propertiesPanel, &PropertiesPanel::propertiesChanged,
    //         m_treeView, &WellSchematicTree::treeChanged);

    mainLayout->addLayout(componentLayout, 1);  // Растягиваем компоненты
    mainLayout->addLayout(wellViewLayout, 1);  // Растягиваем дерево
    mainLayout->addLayout(propertyLayout, 1);  // Растягиваем свойства

    setLayout(mainLayout);
}

void ComponentLibraryWidget::loadComponentsData()
{
    qDebug() << "Loading components data...";

    if (!model) {
        qDebug() << "ERROR: Model is null!";
        return;
    }

    model->clear();

    // Данные компонентов с ПРАВИЛЬНЫМИ путями к ресурсам
    QVector<QPair<QString, QString>> components = {
        {"Blast joint", ":data/resources/Component Library/blast joint.svg"},
        {"Casing", ":/data/resources/Component Library/casing.svg"},
        {"Cemen", ":/data/resources/Component Library/cement.svg"},
        {"Cement Plug", ":/data/resources/Component Library/cement plug.svg"},
        {"Cernralize", ":/data/resources/Component Library/centralize.svg"},
        {"Collar", ":/data/resources/Component Library/collar.svg"},
        {"Double Tubing", ":/data/resources/Component Library/double tubing.svg"},
        {"Fish", ":/data/resources/Component Library/fish.svg"},
        {"Fluid", ":/data/resources/Component Library/fluid.svg"},
        {"Gas Lift Mandre", ":/data/resources/Component Library/gas lift mandrel.svg"},
        {"Landing nipple", ":/data/resources/Component Library/landing nipple.svg"},
        {"Open Hole", ":/data/resources/Component Library/open hole.svg"},
        {"Packer", ":/data/resources/Component Library/packer.svg"},
        {"Perforations", ":/data/resources/Component Library/perforations.svg"},
        {"Pump", ":/data/resources/Component Library/pump.svg"},
        {"Screen", ":/data/resources/Component Library/screen.svg"},
        {"SSD", ":/data/resources/Component Library/SSD.svg"},
        {"TD", ":/data/resources/Component Library/TD.svg"},
        {"TRSSV", ":/data/resources/Component Library/TRSSV.svg"},
        {"Tubing", ":/data/resources/Component Library/tubing.svg"},
        {"Wireline Entry", ":/data/resources/Component Library/wireline entry.svg"},
        {"Y-tool", ":/data/resources/Component Library/Y-tool.svg"}
    };

    qDebug() << "Total components to load:" << components.size();

    // Настройка модели
    int columns = 3;
    int rows = (components.size() + columns - 1) / columns;

    model->setRowCount(rows);
    model->setColumnCount(columns);

    qDebug() << "Model configured with" << rows << "rows and" << columns << "columns";

    // Заполняем таблицу
    for (int i = 0; i < components.size(); ++i) {
        int row = i / columns;
        int col = i % columns;

        const auto &component = components[i];

        QStandardItem *item = new QStandardItem(component.first);
        item->setTextAlignment(Qt::AlignCenter);
        item->setEditable(false);

        // Устанавливаем данные для отображения
        item->setData(component.first, Qt::DisplayRole);
        item->setData(component.second, Qt::UserRole + 1); // Путь к иконке

        // ПРЕДВАРИТЕЛЬНАЯ ПРОВЕРКА ЗАГРУЗКИ ИКОНКИ
        QPixmap testPixmap;
        if (testPixmap.load(components[i].second)) {
            qDebug() << "✓ Icon loaded successfully:" << component.second;
        } else {
            qDebug() << "✗ Failed to load icon:" << component.second;

            // Попробуем альтернативные пути
            QString alternativePath = component.second.mid(1); // Убираем первый символ ':'
            if (testPixmap.load(alternativePath)) {
                qDebug() << "✓ Icon loaded from alternative path:" << alternativePath;
                item->setData(alternativePath, Qt::UserRole + 1);
            } else {
                qDebug() << "✗ All loading attempts failed for:" << component.first;
            }
        }

        model->setItem(row, col, item);
    }

    // Принудительно обновляем таблицу
    if (tableView) {
        tableView->update();
        tableView->viewport()->update();
        tableView->reset();
    }

    // //!!!
    // for (int row = 0; row < model->rowCount(); ++row) {
    //     for (int col = 0; col < model->columnCount(); ++col) {
    //         QStandardItem *item = model->item(row, col);
    //         if (item) {
    //             item->setDragEnabled(true);
    //         }
    //     }
    // }

    qDebug() << "Data loading completed";
}

void ComponentLibraryWidget::onItemDoubleClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        QString componentName = index.data(Qt::DisplayRole).toString();
        QString iconPath = index.data(Qt::UserRole + 1).toString();

        // Проверяем доступность иконки
        bool iconAvailable = QFile::exists(iconPath);

        QString message = QString("Selected: %1\nIcon: %2")
                              .arg(componentName)
                              .arg(iconAvailable ? "Available" : "Not available");

        QMessageBox::information(this, "Component Selected", message);

        // Испускаем сигнал для других частей приложения
        emit componentSelected(componentName, iconPath);
    }
}

QIcon ComponentLibraryWidget::getComponentIcon(const QString &componentName) const
{
    // Поиск иконки по имени компонента в модели
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QStandardItem *item = model->item(row, col);
            if (item && item->text() == componentName) {
                QString iconPath = item->data(Qt::UserRole + 1).toString();
                return QIcon(iconPath);
            }
        }
    }
    return QIcon();
}

QStringList ComponentLibraryWidget::getAvailableComponents() const
{
    QStringList components;
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QStandardItem *item = model->item(row, col);
            if (item && !item->text().isEmpty()) {
                components << item->text();
            }
        }
    }
    return components;
}

