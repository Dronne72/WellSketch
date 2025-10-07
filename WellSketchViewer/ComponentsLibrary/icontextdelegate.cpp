#include "icontextdelegate.h"


IconTextDelegate::IconTextDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void IconTextDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // Настройка фона
    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, QColor(220, 240, 255));
        painter->setPen(QPen(QColor(100, 150, 255), 2));
    } else {
        painter->fillRect(option.rect, option.palette.base());
        painter->setPen(QPen(QColor(200, 200, 200), 1));
    }
    painter->drawRect(option.rect);

    // Получаем данные
    QString text = index.data(Qt::DisplayRole).toString();
    QVariant iconData = index.data(Qt::UserRole + 1);

    QString iconPath;
    QPixmap iconPixmap;

    // Обрабатываем данные иконки
    if (iconData.isValid()) {
        if (iconData.typeId() == QMetaType::QString) {
            // Это путь к иконке
            iconPath = iconData.toString();
            qDebug() << "Loading icon from path:" << iconPath;

            // Пробуем загрузить иконку разными способами
            if (!iconPath.isEmpty()) {
                // Способ 1: Прямая загрузка через QPixmap
                if (iconPixmap.load(iconPath)) {
                    qDebug() << "Successfully loaded icon from path:" << iconPath;
                }
                //  Попробовать с префиксом ресурсов
                else if (iconPixmap.load(":/" + iconPath)) {
                    qDebug() << "Successfully loaded icon with resource prefix:" << iconPath;
                }
                // Способ 3: Через QIcon
                else {
                    QIcon icon(iconPath);
                    if (!icon.isNull()) {
                        iconPixmap = icon.pixmap(48, 48);
                        if (!iconPixmap.isNull()) {
                            qDebug() << "Successfully loaded icon via QIcon:" << iconPath;
                        }
                    }
                }
            }
        } else if (iconData.typeId() == QMetaType::QIcon) {
            // Это уже QIcon
            QIcon icon = iconData.value<QIcon>();
            if (!icon.isNull()) {
                iconPixmap = icon.pixmap(48, 48);
                qDebug() << "Using QIcon directly";
            }
        }
    }

    // Рассчитываем размеры
    int iconSize = 48;
    int textHeight = option.fontMetrics.height();
    int totalHeight = iconSize + textHeight + 15;
    int startY = option.rect.top() + (option.rect.height() - totalHeight) / 2;
    int centerX = option.rect.left() + option.rect.width() / 2;

    // Рисуем иконку
    QRect iconRect(centerX - iconSize/2, startY, iconSize, iconSize);

    if (!iconPixmap.isNull()) {
        // Масштабируем и рисуем иконку
        QPixmap scaledPixmap = iconPixmap.scaled(iconSize, iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QRect targetRect = scaledPixmap.rect();
        targetRect.moveCenter(iconRect.center());
        painter->drawPixmap(targetRect, scaledPixmap);
        qDebug() << "Icon drawn successfully for:" << text;
    } else {
        // Рисуем заглушку с информацией об ошибке
        QString errorMsg;
        if (iconPath.isEmpty()) {
            errorMsg = "No icon\npath";
        } else {
            // Обрезаем длинный путь для отображения
            QString shortPath = iconPath;
            if (shortPath.length() > 20) {
                shortPath = "..." + shortPath.right(17);
            }
            errorMsg = "Failed to\nload icon";
            qDebug() << "Failed to load icon:" << iconPath << "for component:" << text;
        }
        drawIconPlaceholder(painter, iconRect, errorMsg);
    }

    // Рисуем текст под иконкой
    QRect textRect(option.rect.left() + 5, startY + iconSize + 8,
                   option.rect.width() - 10, textHeight);

    QFont textFont = option.font;
    textFont.setPointSize(9);
    textFont.setWeight(QFont::Medium);
    painter->setFont(textFont);

    painter->setPen(option.state & QStyle::State_Selected ?
                        QColor(0, 0, 139) : option.palette.text().color());

    painter->drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, text);

    painter->restore();
}

void IconTextDelegate::drawIconPlaceholder(QPainter *painter, const QRect &rect, const QString &message) const
{
    painter->setBrush(QColor(240, 240, 240));
    painter->setPen(QColor(180, 180, 180));
    painter->drawRect(rect);

    QFont smallFont = painter->font();
    smallFont.setPointSize(6);
    painter->setFont(smallFont);
    painter->setPen(Qt::darkGray);
    painter->drawText(rect, Qt::AlignCenter, message);
}

QSize IconTextDelegate::sizeHint(const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(140, 110); // Увеличим размер ячейки для лучшего отображения
}
