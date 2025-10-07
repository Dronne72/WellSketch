// ### Qt6.8.0 MSVC 2022_64
// Copyright (C) 2025 North Side aly 26/08/2025

#ifndef PROPERTYEDITOR_H
#define PROPERTYEDITOR_H

#include <QTreeWidget>
#include <QStyledItemDelegate>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QFontDialog>
#include <QColorDialog>
#include <QDateTimeEdit>
#include <QDataStream>

#include "DelegatePatternFill.h"

/*
    qt c++ create a pattern delegate based on QStyledItemDelegate for different colors and patterns.
    Add the delegate to QTreeWidget for user interaction. Load/save the data from/to qsettings.


    // Example - how to use to Propety Editor

    PropertyEditor *editor = new PropertyEditor();
    layout->addWidget(editor);

    QTreeWidgetItem *generalCategory = editor->addCategory("General Properties");
    QTreeWidgetItem *appearanceCategory = editor->addCategory("Appearance");

    editor->addProperty("Enabled", true, PropertyDelegate::Boolean, generalCategory);
    editor->addProperty("Width", 800, PropertyDelegate::Integer, generalCategory);
    editor->addProperty("Opacity", 0.8, PropertyDelegate::Double, generalCategory);
    editor->addProperty("Title", "My Application", PropertyDelegate::String, generalCategory);

    editor->addProperty("Background Color", QColor(255, 200, 200), PropertyDelegate::Color, appearanceCategory);
    editor->addProperty("Text Font", QFont("Arial", 9), PropertyDelegate::Font, appearanceCategory);

    QStringList options = {"Option 1", "Option 2", "Option 3"};
    QTreeWidgetItem *list = editor->addProperty("Selection", "Option 1", PropertyDelegate::List, appearanceCategory, false);
    editor->setPropertyValue(list, options); // Set list options

    editor->addProperty("Creation Date", QDateTime::currentDateTime(), PropertyDelegate::DateTime, generalCategory);

    editor->expandAll();
*/

class PropertyDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    enum PropertyType {
        Boolean,
        Integer,
        Double,
        String,
        Font,
        List,
        Color,
        Pattern,
        DateTime,
        Data
    };

    explicit PropertyDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index) override;

    static PropertyType getPropertyType(const QModelIndex &index);
    void setPropertyGeometry(int rheight, int nameLeftMargin, int propleftMargin);

private:

    int m_rowHeight = 17;
    int m_nameLeftMargin = -20;
    int m_propLeftMargin = 4;
};

class PropertyEditor : public QTreeWidget
{
    Q_OBJECT
public:
    explicit PropertyEditor(QWidget *parent = nullptr);

    // Property management
    QTreeWidgetItem* addProperty(const QString &name, const QVariant &value,
                                 PropertyDelegate::PropertyType type,
                                 QTreeWidgetItem *parent = nullptr,
                                 bool readOnly = false,
                                 const QColor &bgColor = Qt::transparent,
                                 const QColor &textColor = Qt::black);

    QTreeWidgetItem* addCategory(const QString &name, QTreeWidgetItem *parent = nullptr);

    // Utility methods
    void setPropertyValue(QTreeWidgetItem *item, const QVariant value, const bool setList = true);
    QVariant getPropertyValue(QTreeWidgetItem *item) const;

    void setPropertyReadOnly(QTreeWidgetItem *item, bool readOnly);
    void setPropertyEnabled(QTreeWidgetItem *item, bool enabled);
    void setPropertyColors(QTreeWidgetItem *item, const QColor &bgColor, const QColor &textColor);
    void setPropertyGeometry(int rowHeight, int leftMargin1, int leftMargin2);

    QTreeWidgetItem* findProperty(const QString &name, QTreeWidgetItem *parent = nullptr) const;

    void clearChangedMarksRecursive();

    // Data serialization
    QByteArray serializeProperties() const;
    void deserializeProperties(const QByteArray &data);

signals:
    void propertyChanged(QTreeWidgetItem *item, const QVariant &newValue);
    void propertyDoubleClicked(QTreeWidgetItem *item);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private slots:
    void onItemChanged(QTreeWidgetItem *item, int column);

private:
    QVariant convertValue(const QVariant &value, PropertyDelegate::PropertyType type) const;
    void iterateItem(QTreeWidgetItem* item);
};

#endif // PROPERTYEDITOR_H


