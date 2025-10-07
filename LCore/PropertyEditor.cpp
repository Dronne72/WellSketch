#include "PropertyEditor.h"

#include <QPainter>
#include <QApplication>
#include <QMouseEvent>
#include <QHeaderView>

//! The Property Editor has got 2 columns: name & value.
//! Value has a bunch of property delegates to handle Color, Font, etc (see PropertyType)
//! Each values can have controls: checkbox, combo-box or else.
//! Properties can be displayed in a hierarchical tree under Categories, be enable, disabled, read-only.

const int fillHeight = 14;
const int fillWidth = 40;

const QString hoverColor("#e7f6ff");
const QString selectedBorderColor("gray");
const QColor categoryColor = QColor(212,212,212);

PropertyDelegate::PropertyDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

QWidget *PropertyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const
{
    // Add items to delegate when init

    if (index.column() == 0) // skip name column
        return nullptr;

    PropertyType type = getPropertyType(index);
    QTreeWidgetItem *item = static_cast<QTreeWidgetItem*>(index.internalPointer());
    bool readOnly = item->data(1, Qt::UserRole + 2).toBool();

    if (readOnly)
        return nullptr;

    // Set borders if delegate activated
    switch (type) {
    case Boolean: {
        QCheckBox *editor = new QCheckBox(parent);
        QString style =  QString("QCheckBox {margin-left: %1px;  border: 1px solid %2;}").arg(m_propLeftMargin).arg(selectedBorderColor);
        editor->setStyleSheet(style);
        return editor;
    }
    case Integer: {
        QSpinBox *editor = new QSpinBox(parent);
        QString style =  QString("QSpinBox {margin-left: %1px;  border: 1px solid %2;}").arg(m_propLeftMargin).arg(selectedBorderColor);
        editor->setStyleSheet(style);
        editor->setRange(INT_MIN, INT_MAX);
        return editor;
    }
    case Double: {
        QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
        editor->setRange(-1e9, 1e9);
        editor->setDecimals(6);
        QString style =  QString("QDoubleSpinBox {margin-left: %1px;  border: 1px solid %2;}").arg(m_propLeftMargin).arg(selectedBorderColor);
        editor->setStyleSheet(style);
        return editor;
    }
    case String: {
        QLineEdit *editor = new QLineEdit(parent);
        QString style =  QString("QLineEdit {margin-left: %1px;  border: 1px solid %2;}").arg(m_propLeftMargin).arg(selectedBorderColor);
        editor->setStyleSheet(style);
        return editor;
    }
    case List: {
        QComboBox *editor = new QComboBox(parent);
        QStringList options = item->data(1, Qt::UserRole + 3).toStringList();
        QString style =  QString("QComboBox {margin-left: %1px;  border: 1px solid %2;}").arg(m_propLeftMargin).arg(selectedBorderColor);
        editor->setStyleSheet(style);
        editor->addItems(options);
        return editor;
    }
    case Pattern: {
        if (item->data(1, Qt::UserRole + 3).canConvert<QList<DelegatePatternFill::PatternData>>())  {
            QComboBox *editor = new QComboBox(parent);
            QList<DelegatePatternFill::PatternData> patterns =
                item->data(1, Qt::UserRole + 3).value<QList<DelegatePatternFill::PatternData>>();

            // Add patterns to combo box with preview icons
            for (const auto &pattern : patterns) {
                QPixmap pixmap = DelegatePatternFill::createPatternPixmap(pattern, QSize(fillWidth, fillHeight));
                editor->setIconSize(QSize(fillWidth, fillHeight));
                editor->addItem(QIcon(pixmap), pattern.name, QVariant::fromValue(pattern));
            }
            QString style =  QString("QComboBox {margin-left: %1px;  border: 1px solid %2;}").arg(m_propLeftMargin).arg(selectedBorderColor);
            editor->setStyleSheet(style);
            return editor;
        }
        return nullptr;
    }
    case DateTime: {
        QDateTimeEdit *editor = new QDateTimeEdit(parent);
        QString style =  QString("QDateTimeEdit {margin-left: %1px;  border: 1px solid %2;}").arg(m_propLeftMargin).arg(selectedBorderColor);
        editor->setStyleSheet(style);
        return editor;
    }
    default:
        return nullptr;
    }
}

void PropertyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    // Set data delegate after item activation

    QVariant value = index.data(Qt::EditRole);
    PropertyType type = getPropertyType(index);

    switch (type) {
    case Boolean: {
        QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
        checkBox->setChecked(value.toBool());
        break;
    }
    case Integer: {
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->setValue(value.toInt());
        break;
    }
    case Double: {
        QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
        spinBox->setValue(value.toDouble());
        break;
    }
    case String: {
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        lineEdit->setText(value.toString());
        break;
    }
    case List: {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        comboBox->setCurrentText(value.toString());
        break;
    }
    case Pattern: {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);

        QVariant data = index.data(Qt::EditRole);
        if (data.canConvert<DelegatePatternFill::PatternData>()) {
            DelegatePatternFill::PatternData pattern = data.value<DelegatePatternFill::PatternData>();

            // Find matching pattern in combo box
            for (int i = 0; i < comboBox->count(); ++i) {
                DelegatePatternFill::PatternData itemPattern = comboBox->itemData(i).value<DelegatePatternFill::PatternData>();
                if (itemPattern.name == pattern.name) {
                    comboBox->setCurrentIndex(i);
                    break;
                }
            }
        }
        break;
    }
    case DateTime: {
        QDateTimeEdit *dateTimeEdit = static_cast<QDateTimeEdit*>(editor);
        dateTimeEdit->setDateTime(value.toDateTime());
        break;
    }
    default:
        break;
    }
}

void PropertyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    // Set data to delegate after selection
    PropertyType type = getPropertyType(index);
    QVariant newValue;

    switch (type) {
    case Boolean: {
        QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
        newValue = checkBox->isChecked();
        break;
    }
    case Integer: {
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        newValue = spinBox->value();
        break;
    }
    case Double: {
        QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
        newValue = spinBox->value();
        break;
    }
    case String: {
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        newValue = lineEdit->text();
        break;
    }
    case List: {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        newValue = comboBox->currentText();
        break;
    }
    case Pattern: {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        DelegatePatternFill::PatternData pattern = comboBox->currentData().value<DelegatePatternFill::PatternData>();
        newValue = QVariant::fromValue(pattern);;
        break;
    }
    case DateTime: {
        QDateTimeEdit *dateTimeEdit = static_cast<QDateTimeEdit*>(editor);
        newValue = dateTimeEdit->dateTime();
        break;
    }
    default:
        return;
    }

    model->setData(index, newValue, Qt::EditRole);
}

void PropertyDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void PropertyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;

    // Draw bottom border
    QRect underlineRect = option.rect.adjusted(m_nameLeftMargin, 0, 0, 0);
    painter->setPen(QPen(Qt::lightGray, 1));
    painter->drawLine(underlineRect.bottomLeft(), underlineRect.bottomRight());

    if (index.column() == 0)
    {
        // Remove hover state
        opt.state &= ~QStyle::State_MouseOver;

        // Paint Category items
        int margin = 20; // 20
        if (index.model()->hasChildren(index) &&
            !index.model()->parent(index).isValid())
        {
            painter->save();

            // Draw Category row
            QRect borderRect = option.rect.adjusted(-margin, 0, 0, 0);
            borderRect = QRect( borderRect.left(),  borderRect.top(), margin, borderRect.height());
            painter->fillRect(borderRect, categoryColor);

            // Draw indicator
            QStyleOption branchOption;
            branchOption.rect = option.rect;
            branchOption.rect.setWidth(16); // Width of expand indicator
            branchOption.rect.setHeight(16);
            branchOption.rect.moveTop(option.rect.top() + (option.rect.height() - 16) / 2);
            branchOption.rect.moveLeft(option.rect.left() - margin); // Position to the left

            // Set state for expanded/collapsed
            branchOption.state = QStyle::State_Children;
            if (option.state & QStyle::State_Open) {
                branchOption.state |= QStyle::State_Open;
            }

            // Draw the expand control using the style
            QApplication::style()->drawPrimitive(QStyle::PE_IndicatorBranch,
                                                 &branchOption, painter);
        }
        else
        {
            // @aly PLEASE leave as may use later - draw frame on child items
            // QRect borderRect = option.rect.adjusted(-2 * margin, 0, 0, 0);
            // borderRect = QRect( borderRect.left(),  borderRect.top(), margin, borderRect.height());
            // painter->fillRect(borderRect, categoryColor);

            if (!index.model()->hasChildren(index))
                opt.rect.adjust(m_nameLeftMargin, 0, 0, 0); // adjust left margin

            // Draw a red data changed flag
            QVariant changedData = index.data(Qt::UserRole + 4);
            if (changedData.toBool())
            {
                painter->save();
                QRect redFlagRect = option.rect.adjusted(-2 * margin, 0, 0, 0);
                redFlagRect.setWidth(2); // Border width
                painter->fillRect(redFlagRect, Qt::red);
                painter->restore();
            }
        }

        QStyledItemDelegate::paint(painter, opt, index);
        return;
    }

    PropertyType type = getPropertyType(index);
    QTreeWidgetItem *item = static_cast<QTreeWidgetItem*>(index.internalPointer());
    QVariant value = index.data(Qt::DisplayRole);

    // Get custom colors
    QColor bgColor = item->data(1, Qt::BackgroundRole).value<QColor>();
    QColor textColor = item->data(1, Qt::ForegroundRole).value<QColor>();

    if (bgColor.isValid() && bgColor != Qt::transparent) {
        painter->fillRect(option.rect, bgColor);
    }

    painter->save();

    if (textColor.isValid()) {
        painter->setPen(textColor);
    }

    QRect rect = option.rect.adjusted(m_propLeftMargin, 0, 0, 0);

    // Paint Delegates
    switch (type) {
    case Boolean: {
        bool checked = value.toBool();
        QStyleOptionButton opt;
        opt.rect = QRect(rect.left(), rect.top(), 16, 16);
        opt.state = checked ? QStyle::State_On : QStyle::State_Off;
        opt.state |= QStyle::State_Enabled;
        QApplication::style()->drawControl(QStyle::CE_CheckBox, &opt, painter);

        break;
    }
    case Color: {
        QColor color = value.value<QColor>();
        if (color.isValid()) {
            QRect colorRect = rect;
            colorRect = QRect(rect.left(), rect.top() + 1, fillWidth, fillHeight);
            painter->fillRect(colorRect, color);

            if (color == Qt::white)
            {
                QPen pen(Qt::lightGray);
                pen.setWidth(1);
                painter->setPen(pen);
                painter->drawRect(colorRect.adjusted(0,0,0,0));
            }

            //Draw RGB text
            QString text = QString(" RGB (%1; %2; %3)").
                           arg(color.red()).
                           arg(color.green()).
                           arg(color.blue());
            painter->setPen(Qt::black);
            painter->drawText(colorRect.right() + 5, rect.top(),
                              rect.width() - 25, rect.height(),
                              Qt::AlignLeft | Qt::AlignVCenter,
                              text);

            // Draw three right dots
            painter->drawText(rect, Qt::AlignRight | Qt::AlignVCenter, " ... ");
        }
        break;
    }
    case Font: {
        QFont font = value.value<QFont>();
        painter->setFont(font);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter,
                          font.family() + " " + QString::number(font.pointSize()));

        // Draw three right dots
        painter->setPen(Qt::black);
        painter->drawText(rect, Qt::AlignRight | Qt::AlignVCenter, " ... ");

        break;
    }
    case Pattern: {
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);

        // Draw background
        QStyledItemDelegate::paint(painter, opt, index);

        QVariant data = index.data(Qt::DisplayRole);
        if (data.canConvert<DelegatePatternFill::PatternData>())
        {
            DelegatePatternFill::PatternData pattern =
                data.value<DelegatePatternFill::PatternData>();

            // @aly Front color should always be first in row
            QColor frontColor, backColor;
            for(int i = 0; i < item->parent()->childCount(); ++i)
            {
                QTreeWidgetItem* child = item->parent()->child(i);
                PropertyDelegate::PropertyType type = static_cast<PropertyDelegate::PropertyType>(
                    child->data(1, Qt::UserRole + 1).toInt());

                if (type == PropertyDelegate::Color)
                {
                    if (!frontColor.isValid())
                        frontColor = child->data(1, Qt::DisplayRole).value<QColor>();
                    else
                        backColor = child->data(1, Qt::DisplayRole).value<QColor>();
                }
            }

            pattern.backgroundColor = backColor;
            pattern.color = frontColor;

            // Draw pattern preview
            QRect patternRect = rect;
            patternRect = QRect(rect.left(), rect.top() + 1, fillWidth, fillHeight);
            DelegatePatternFill::drawPatternStatic(painter, patternRect, pattern);

            // Draw text
            QRect textRect = opt.rect;
            textRect.setLeft(patternRect.right() + 8);
            painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, pattern.name);
        }

        break;
    }
    default:
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, value.toString());
        break;
    }

    painter->restore();
}

QSize PropertyDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    // Set left margin of the Property values
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setWidth(size.width() + m_propLeftMargin);
    size.setHeight(m_rowHeight);
    return size;
}

bool PropertyDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                 const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

PropertyDelegate::PropertyType PropertyDelegate::getPropertyType(const QModelIndex &index)
{
    QTreeWidgetItem *item = static_cast<QTreeWidgetItem*>(index.internalPointer());
    return static_cast<PropertyType>(item->data(1, Qt::UserRole + 1).toInt());
}

void PropertyDelegate::setPropertyGeometry(int rheight, int nameLeftMargin, int propLeftMargin)
{
    m_rowHeight = rheight;
    m_nameLeftMargin = nameLeftMargin;
    m_propLeftMargin = propLeftMargin;
}

// ------------------------------------------------
// Property Editor
// ------------------------------------------------
PropertyEditor::PropertyEditor(QWidget *parent)
    : QTreeWidget(parent)
{
    // Set up Tree Style
    setColumnCount(2);
    setHeaderHidden(true);
    header()->setSectionResizeMode(QHeaderView::Stretch);  // Stretch equally

    setRootIsDecorated(true);
    setUniformRowHeights(false);

    setFocusPolicy(Qt::NoFocus);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::AllEditTriggers);
    setSelectionMode(QAbstractItemView::NoSelection);

    setStyleSheet(QString(
        "QTreeWidget::item:hover {"
        "    background: %1;"
        "}"
        "QTreeWidget::item:selected {"
        "    background: none;"
        "}").arg(hoverColor));

    auto delegate = new PropertyDelegate(this);
    setItemDelegate(delegate);

    connect(this, &QTreeWidget::itemChanged, this, &PropertyEditor::onItemChanged);
}

void PropertyEditor::setPropertyGeometry(int rowHeight, int nameLeftMargin, int propLeftMargin)
{
    PropertyDelegate* delegate = qobject_cast<PropertyDelegate*>(this->itemDelegate());
    delegate->setPropertyGeometry(rowHeight, nameLeftMargin, propLeftMargin);
}

QTreeWidgetItem* PropertyEditor::addProperty(const QString &name, const QVariant &value,
                                             PropertyDelegate::PropertyType type,
                                             QTreeWidgetItem *parent,
                                             bool readOnly,
                                             const QColor &bgColor,
                                             const QColor &textColor)
{
    QTreeWidgetItem *item;
    if (parent) {
        item = new QTreeWidgetItem(parent);
    } else {
        item = new QTreeWidgetItem(this);
    }

    item->setText(0, name);

    QVariant convertedValue = convertValue(value, type);
    item->setData(1, Qt::DisplayRole, convertedValue);
    item->setData(1, Qt::EditRole, convertedValue);
    item->setData(1, Qt::UserRole + 1, type); // Store property type
    item->setData(1, Qt::UserRole + 2, readOnly); // Store read-only flag
    //item->setData(1, Qt::UserRole + 3, type); // do not Store list here
    item->setData(0, Qt::UserRole + 4, false); // Store red mark flag

    if (bgColor.isValid()) {
        item->setData(1, Qt::BackgroundRole, bgColor);
    }
    if (textColor.isValid()) {
        item->setData(1, Qt::ForegroundRole, textColor);
    }

    // Set flags
    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    if (!readOnly && type != PropertyDelegate::Font && type != PropertyDelegate::Color) {
        flags |= Qt::ItemIsEditable;
    }
    item->setFlags(flags);

    return item;
}

void PropertyEditor::setPropertyValue(QTreeWidgetItem *item, const QVariant value, const bool setList)
{
    // @ aly we come here after: update from Settings or List for combo-box or mousePressEvent

    PropertyDelegate::PropertyType type = static_cast<PropertyDelegate::PropertyType>(
        item->data(1, Qt::UserRole + 1).toInt());

    QVariant convertedValue = convertValue(value, type);
    if (setList)
    {
        item->setData(1, Qt::UserRole + 3, value);      // set list only when init
        return;
    }

    item->setData(1, Qt::DisplayRole, convertedValue);  // color or else
    item->setData(1, Qt::EditRole, convertedValue);     // value
    item->setData(1, Qt::UserRole + 1, type);           // type
}

QTreeWidgetItem* PropertyEditor::addCategory(const QString &name, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *item;
    if (parent) {
        item = new QTreeWidgetItem(parent);
    } else {
        item = new QTreeWidgetItem(this);
    }

    item->setText(0, name);
    item->setFirstColumnSpanned(true);

    QFont boldFont = font();
    boldFont.setBold(true);
    item->setFont(0, boldFont);

    if (!parent) // for top Category only
    {
        item->setBackground(0, categoryColor);
    }

    Qt::ItemFlags flags = item->flags();
    flags &= ~Qt::ItemIsSelectable;
    item->setFlags(flags);

    return item;
}

QVariant PropertyEditor::getPropertyValue(QTreeWidgetItem *item) const
{
    return item->data(1, Qt::EditRole);
}

void PropertyEditor::setPropertyReadOnly(QTreeWidgetItem *item, bool readOnly)
{
    item->setData(1, Qt::UserRole + 2, readOnly);
    Qt::ItemFlags flags = item->flags();
    if (readOnly) {
        flags &= ~Qt::ItemIsEditable;
    } else {
        PropertyDelegate::PropertyType type = static_cast<PropertyDelegate::PropertyType>(
            item->data(1, Qt::UserRole + 1).toInt());
        if (type != PropertyDelegate::Font && type != PropertyDelegate::Color) {
            flags |= Qt::ItemIsEditable;
        }
    }
    item->setFlags(flags);
}

void PropertyEditor::setPropertyEnabled(QTreeWidgetItem *item, bool enabled)
{
    Qt::ItemFlags flags = item->flags();
    if (enabled) {
        flags |= Qt::ItemIsEnabled;
    } else {
        flags &= ~Qt::ItemIsEnabled;
    }
    item->setFlags(flags);
}

void PropertyEditor::setPropertyColors(QTreeWidgetItem *item, const QColor &bgColor, const QColor &textColor)
{
    if (bgColor.isValid()) {
        item->setData(1, Qt::BackgroundRole, bgColor);
    } else {
        item->setData(1, Qt::BackgroundRole, QVariant());
    }

    if (textColor.isValid()) {
        item->setData(1, Qt::ForegroundRole, textColor);
    } else {
        item->setData(1, Qt::ForegroundRole, QVariant());
    }
}

void PropertyEditor::mousePressEvent(QMouseEvent *event)
{
    QTreeWidgetItem *item = itemAt(event->pos());

    if (item && item->columnCount() > 1)
    {
        QRect itemRect = visualItemRect(item);
        QRect valueRect(columnWidth(0), itemRect.top(), itemRect.right(), itemRect.height());

        if (valueRect.contains(event->pos())) // only for Value column
        {
            PropertyDelegate::PropertyType type = static_cast<PropertyDelegate::PropertyType>(
                item->data(1, Qt::UserRole + 1).toInt());
            bool readOnly = item->data(1, Qt::UserRole + 2).toBool();

            if (!readOnly)
            {
                switch (type)
                {
                case PropertyDelegate::Font: {
                    QFont currentFont = item->data(1, Qt::EditRole).value<QFont>();
                    bool ok;
                    QFont newFont = QFontDialog::getFont(&ok, currentFont, this);
                    if (ok) {
                        setPropertyValue(item, newFont, false);
                        emit propertyChanged(item, newFont);
                    }
                    break;
                }
                case PropertyDelegate::Color: {
                    QColor currentColor = item->data(1, Qt::EditRole).value<QColor>();
                    QColor newColor = QColorDialog::getColor(currentColor, this);
                    if (newColor.isValid()) {
                        setPropertyValue(item, newColor, false);
                        emit propertyChanged(item, newColor);
                    }
                    break;
                }
                default:
                    break;
                }
            }
            emit propertyDoubleClicked(item);
        }
    }

    QTreeWidget::mousePressEvent(event);
}

void PropertyEditor::mouseDoubleClickEvent(QMouseEvent *event)
{
    QTreeWidget::mouseDoubleClickEvent(event);
}

void PropertyEditor::onItemChanged(QTreeWidgetItem *item, int column)
{
    if (column > 0) {
        emit propertyChanged(item, getPropertyValue(item));
        item->setData(0, Qt::UserRole + 4, true); // set to draw red mark flag
    }
}

void PropertyEditor::clearChangedMarksRecursive()
{
    for (int i = 0; i < this->topLevelItemCount(); ++i) {
        QTreeWidgetItem* topLevelItem = this->topLevelItem(i);
        iterateItem(topLevelItem);
    }
}

void PropertyEditor::iterateItem(QTreeWidgetItem* item)
{
    // Remove red mark flag role
    for (int col = 0; col < item->columnCount(); ++col) {
        item->setData(0, Qt::UserRole + 4, false);
    }

    for (int i = 0; i < item->childCount(); ++i) {
        iterateItem(item->child(i));
    }
}

QVariant PropertyEditor::convertValue(const QVariant &value, PropertyDelegate::PropertyType type) const
{
    switch (type) {
    case PropertyDelegate::Boolean:
        return value.toBool();
    case PropertyDelegate::Integer:
        return value.toInt();
    case PropertyDelegate::Double:
        return value.toDouble();
    case PropertyDelegate::String:
        return value.toString();
    case PropertyDelegate::Font:
        return value.value<QFont>();
    case PropertyDelegate::List:
        return value.toString(); // @aly caution may return List<QString> or String
    case PropertyDelegate::Pattern:
    {
        DelegatePatternFill::PatternData pattern =
            DelegatePatternFill::getPatternDataByType(
            static_cast<DelegatePatternFill::PatternType>(value.toInt()));
        return  QVariant::fromValue(pattern);
    }
    case PropertyDelegate::Color:
        return value.value<QColor>();
    case PropertyDelegate::DateTime:
        return value.toDateTime();
    case PropertyDelegate::Data:
        return value.toByteArray();
    default:
        return value;
    }
}

QTreeWidgetItem* PropertyEditor::findProperty(const QString &name, QTreeWidgetItem *parent) const
{
    int count = parent ? parent->childCount() : topLevelItemCount();

    for (int i = 0; i < count; ++i) {
        QTreeWidgetItem *item = parent ? parent->child(i) : topLevelItem(i);
        if (item->text(0) == name) {
            return item;
        }

        // Recursively search children
        QTreeWidgetItem *found = findProperty(name, item);
        if (found) return found;
    }

    return nullptr;
}

QByteArray PropertyEditor::serializeProperties() const
{
    // Serialize to QByteArray and save to Settings
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    QTreeWidgetItemIterator it(const_cast<PropertyEditor*>(this));
    while (*it) {
        QTreeWidgetItem *item = *it;
        if (item->childCount() == 0) // Only serialize leaf properties
        {
            QString name = item->text(0);
            QVariant value = getPropertyValue(item) ;
            int type = item->data(1, Qt::UserRole + 1).toInt();
            bool readOnly = item->data(1, Qt::UserRole + 2).toBool();

            if (type == PropertyDelegate::Pattern)
            {
                // @aly can not serialize struct, only pattern type (int)
                if (value.canConvert<DelegatePatternFill::PatternData>())
                {
                    DelegatePatternFill::PatternData pattern =
                            value.value<DelegatePatternFill::PatternData>();
                    value = static_cast<int>(pattern.pattern);
                }
            }

            stream << name << value << type << readOnly;
        }
        ++it;
    }

    return data;
}

void PropertyEditor::deserializeProperties(const QByteArray &data)
{
    // Update existing Tree from QByteArray by name
    QDataStream stream(data);

    while (!stream.atEnd()) {
        QString name;
        QVariant value;
        int type;
        bool readOnly;

        stream >> name >> value >> type >> readOnly;

        // Find item by name and update its value
        QList<QTreeWidgetItem*> items = findItems(name, Qt::MatchExactly | Qt::MatchRecursive, 0);
        if (!items.isEmpty())
        {
             if (type == PropertyDelegate::Pattern)
                    value = value.value<DelegatePatternFill::PatternType>();

            setPropertyValue(items.first(), value, false);
            setPropertyReadOnly(items.first(), readOnly);
        }
    }
}
