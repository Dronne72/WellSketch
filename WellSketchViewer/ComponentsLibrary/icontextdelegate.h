#ifndef ICONTEXTDELEGATE_H
#define ICONTEXTDELEGATE_H

#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QMouseEvent>
#include <QDialog>
#include <QFileInfo>

class IconTextDelegate : public QStyledItemDelegate
{
public:
    explicit IconTextDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

private:
    void drawIconPlaceholder(QPainter *painter, const QRect &rect, const QString &message) const;
};
#endif // ICONTEXTDELEGATE_H
