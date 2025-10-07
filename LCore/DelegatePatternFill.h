// ### Qt6.8.0 MSVC 2022_64
// Copyright (C) 2025 North Side aly 210/09/2025

#ifndef DELEGATEPATTERNFILL_H
#define DELEGATEPATTERNFILL_H

#include <QStyledItemDelegate>
#include <QComboBox>
#include <QPainter>

#include <QMetaEnum>
#include <QMetaObject>

class DelegatePatternFill : public QStyledItemDelegate
{
    Q_OBJECT

public:
    enum PatternType {
        NoPattern,
        HorizontalLines,
        VerticalLines,
        CrossLines,
        DiagonalLines,
        RedRush,
        Checkerboard,
        Bricks,
        Dots,
        Dash,
        LastPattern
    };

    struct PatternData {
        QColor color;
        QColor backgroundColor;
        PatternType pattern;
        QString name;
    };

    explicit DelegatePatternFill(QObject *parent = nullptr);
        ~DelegatePatternFill();

        // Delegate methods
        QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
        void setEditorData(QWidget *editor, const QModelIndex &index) const override;
        void setModelData(QWidget *editor, QAbstractItemModel *model,
                          const QModelIndex &index) const override;
        void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const override;
        void paint(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
        QSize sizeHint(const QStyleOptionViewItem &option,
                       const QModelIndex &index) const override;

        // Pattern management
        static QList<PatternData> defaultPatterns();
        static QPixmap createPatternPixmap(const PatternData &patternData, const QSize &size = QSize(32, 32));
        static QString patternTypeToString(PatternType type);
        static PatternType stringToPatternType(const QString &str);
        static PatternData getPatternDataByType(const PatternType &type);

        // Save/load
        //void saveToSettings(QSettings &settings, const QString &group = "PatternDelegate") const;
        //void loadFromSettings(QSettings &settings, const QString &group = "PatternDelegate");
        static void drawPatternStatic(QPainter *painter, const QRect &rect, const PatternData &pattern);

    private:
        QList<PatternData> m_patterns;

        void drawPattern(QPainter *painter, const QRect &rect, const PatternData &pattern) const;

};

#endif // DELEGATEPATTERNFILL_H
