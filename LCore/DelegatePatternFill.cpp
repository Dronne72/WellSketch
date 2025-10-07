#include "DelegatePatternFill.h"

#include <QComboBox>
#include <QSettings>
#include <QApplication>
#include <QStyle>

DelegatePatternFill::DelegatePatternFill(QObject *parent)
    : QStyledItemDelegate{parent}
{
    m_patterns = defaultPatterns();
}

DelegatePatternFill::~DelegatePatternFill() {}

QWidget *DelegatePatternFill::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QComboBox *comboBox = new QComboBox(parent);
    comboBox->setEditable(false);

    // Add patterns to combo box with preview icons
    for (const auto &pattern : m_patterns) {
        QPixmap pixmap = createPatternPixmap(pattern, QSize(32, 32));
        comboBox->addItem(QIcon(pixmap), pattern.name, QVariant::fromValue(pattern));
    }

    return comboBox;
}

void DelegatePatternFill::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
    if (!comboBox) return;

    QVariant data = index.data(Qt::EditRole);
    if (data.canConvert<PatternData>()) {
        PatternData pattern = data.value<PatternData>();

        // Find matching pattern in combo box
        for (int i = 0; i < comboBox->count(); ++i) {
            PatternData itemPattern = comboBox->itemData(i).value<PatternData>();
            if (itemPattern.color == pattern.color && itemPattern.name == pattern.name) {
                comboBox->setCurrentIndex(i);
                break;
            }
        }
    }
}

void DelegatePatternFill::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
    if (!comboBox) return;

    PatternData pattern = comboBox->currentData().value<PatternData>();
    model->setData(index, QVariant::fromValue(pattern), Qt::EditRole);
}

void DelegatePatternFill::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

void DelegatePatternFill::paint(QPainter *painter, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    // Draw background
    QStyledItemDelegate::paint(painter, opt, index);

    // Get pattern data
    QVariant data = index.data(Qt::DisplayRole);
    if (data.canConvert<PatternData>()) {
        PatternData pattern = data.value<PatternData>();

        // Draw pattern preview
        QRect patternRect = opt.rect.adjusted(4, 4, -4, -4);
        drawPattern(painter, patternRect, pattern);

        // Draw text
        QRect textRect = opt.rect;
        textRect.setLeft(patternRect.right() + 8);
        painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, pattern.name);
    } else {
        QStyledItemDelegate::paint(painter, opt, index);
    }
}

QSize DelegatePatternFill::sizeHint(const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(150, 30);
}

QList<DelegatePatternFill::PatternData> DelegatePatternFill::defaultPatterns()
{
     return {
        {Qt::transparent, Qt::transparent,NoPattern, "NoPattern"},
        {Qt::black, Qt::transparent, HorizontalLines, "Horizontal"},
        {Qt::black, Qt::transparent, VerticalLines, "Vertical"},
        {Qt::black, Qt::transparent, CrossLines, "Cross"},
        {Qt::black, Qt::transparent, DiagonalLines, "Diagonal"},
        {Qt::red, Qt::transparent, RedRush, "RedRush"},
        {Qt::black, Qt::transparent, Checkerboard, "Checkerboard"},

        {Qt::black, Qt::transparent, Bricks, "Bricks"},
        {Qt::black, Qt::transparent, Dash, "Dash"},
        {Qt::black, Qt::transparent, Dots, "Dots"}
    };
}

DelegatePatternFill::PatternData DelegatePatternFill::getPatternDataByType(const PatternType &type)
{
    if (type == NoPattern) return {Qt::transparent, Qt::transparent, NoPattern, "NoPattern"};
    else if (type == HorizontalLines) return {Qt::black, Qt::transparent, HorizontalLines, "HorizontalLines"};
    else if (type == VerticalLines) return {Qt::black, Qt::transparent, VerticalLines, "VerticalLines"};
    else if (type == CrossLines) return {Qt::black, Qt::transparent, CrossLines, "CrossLines"};
    else if (type == DiagonalLines) return {Qt::black, Qt::transparent, DiagonalLines, "DiagonalLines"};
    else if (type == RedRush) return {Qt::red, Qt::transparent, RedRush, "RedRush"};
    else if (type == Checkerboard) return {Qt::black, Qt::transparent, Checkerboard, "Checkerboard"};
    else if (type == Bricks) return {Qt::black, Qt::transparent, Bricks, "Bricks"};
    else if (type == Dash) return {Qt::black, Qt::transparent, Dash, "Dash"};
    else if (type == Dots) return {Qt::black, Qt::transparent, Dots, "Dots"};
    else return {Qt::transparent, Qt::transparent, NoPattern, "NoPattern"};
}

QPixmap DelegatePatternFill::createPatternPixmap(const PatternData &patternData, const QSize &size)
{
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    QRect rect = QRect(QPoint(0, 0), size);
    drawPatternStatic(&painter, rect, patternData);

    return pixmap;
}

void DelegatePatternFill::drawPattern(QPainter *painter, const QRect &rect, const PatternData &pattern) const
{
    drawPatternStatic(painter, rect, pattern);
}

void DelegatePatternFill::drawPatternStatic(QPainter *painter, const QRect &rect, const PatternData &pattern)
{
    // @aly Explisitly you can setup background, border and pattern colors

    painter->save();

    // Draw background for all
    painter->fillRect(rect, pattern.backgroundColor);

    // Draw foreground for all
    painter->setPen(QPen(pattern.color, 1));

    switch (pattern.pattern) {
    case NoPattern:
        break;

    case HorizontalLines:
        for (int y = rect.top(); y <= rect.bottom(); y += 4) {
            painter->drawLine(rect.left(), y, rect.right(), y);
        }
        break;

    case VerticalLines:
        for (int x = rect.left(); x <= rect.right(); x += 4) {
            painter->drawLine(x, rect.top(), x, rect.bottom());
        }
        break;

    case CrossLines:
        for (int y = rect.top(); y <= rect.bottom(); y += 4) {
            painter->drawLine(rect.left(), y, rect.right(), y);
        }
        for (int x = rect.left(); x <= rect.right(); x += 4) {
            painter->drawLine(x, rect.top(), x, rect.bottom());
        }
        break;

    case DiagonalLines:
        for (int i = -rect.width(); i <= rect.width(); i += 6) {
            painter->drawLine(rect.left() + i, rect.top(),
                              rect.left() + i + rect.height(), rect.bottom());
        }
        break;

    case RedRush:
        for (int y = rect.top() + 2; y <= rect.bottom(); y += 4) {
            for (int x = rect.left() + 2; x <= rect.right(); x += 4) {
                painter->drawPoint(x, y);
            }
        }
        break;

    case Checkerboard:
        for (int y = rect.top(); y <= rect.bottom() - 3; y += 4) {
            for (int x = rect.left(); x <= rect.right() - 3; x += 4) {
                 if ((x / 4 + y / 4) % 2 == 0) {
                    painter->fillRect(QRect(x, y, 4, 4), pattern.color);
                }
            }
        }
        break;

    case Dots:
    {
        int dotSpacing = 8;
        int lineSpacing = 3;

        int i = 0;
        for (int y = rect.top(); y < rect.bottom() - lineSpacing +1; y += lineSpacing) {
             for (int x = rect.left(); x < rect.right(); x += dotSpacing) {
                if (i % 2)
                    painter->drawPoint(x, y );
                else
                    painter->drawPoint(x + dotSpacing/2, y);
                // Alternative: draw small circles
                // painter->drawEllipse(QPoint(x, y), dotSize, dotSize);
            }
            i++;
        }
        break;
    }
    case Bricks:
    {
        int patternWidth = 10;
        int patternHeight = 4;

        int i = 0;
        for (int y = rect.top(); y < rect.bottom() - patternHeight + 1; y += patternHeight) {
            for (int x = rect.left(); x < rect.right(); x += patternWidth) {
                int xOffset = (i % 2) ? patternWidth / 2 : 0;

                int brickX = x + xOffset;
                int brickY = y;

                // Draw brick if it's within the visible area
                if (brickX < rect.right() && brickY < rect.bottom()) {
                    QRect brick(brickX, brickY,
                                std::min(patternWidth, rect.right() - brickX),
                                std::min(patternHeight, rect.bottom() - brickY));

                    painter->drawRect(brick);
                }
            }
            i++;
        }

        break;
    }
    case Dash:
    {
        QPen pen(pattern.color);
        pen.setDashPattern({4, 4}); // 4 pixels dash, 4 pixels gap
        painter->setPen(pen);

        int lineSpacing = 3;

        int i = 0;
        for (int y = rect.top(); y < rect.bottom(); y += lineSpacing) {
            if (i % 2)
                painter->drawLine(rect.left(), y, rect.right(), y);
            else
                painter->drawLine(rect.left() + 4, y, rect.right(), y);
            i++;
        }
        break;
    }
    }

    // Draw border
    painter->setBrush(Qt::NoBrush);
    painter->setPen(Qt::lightGray);
    painter->drawRect(rect);

    painter->restore();
}

QString DelegatePatternFill::patternTypeToString(PatternType type)
{
    switch (type) {
    case NoPattern: return "NoPattern";
    case HorizontalLines: return "HorizontalLines";
    case VerticalLines: return "VerticalLines";
    case CrossLines: return "CrossLines";
    case DiagonalLines: return "DiagonalLines";
    case RedRush: return "RedRush";
    case Checkerboard: return "Checkerboard";
    case Bricks: return "Bricks";
    case Dash: return "Dash";
    case Dots: return "Dots";
    default: return "NoPattern";
    }
}

DelegatePatternFill::PatternType DelegatePatternFill::stringToPatternType(const QString &str)
{
    if (str == "NoPattern") return NoPattern;
    if (str == "HorizontalLines") return HorizontalLines;
    if (str == "VerticalLines") return VerticalLines;
    if (str == "CrossLines") return CrossLines;
    if (str == "DiagonalLines") return DiagonalLines;
    if (str == "RedRush") return RedRush;
    if (str == "Checkerboard") return Checkerboard;
    if (str == "Bricks") return Bricks;
    if (str == "Dash") return Dash;
    if (str == "Dots") return Dots;

    return NoPattern;
}
