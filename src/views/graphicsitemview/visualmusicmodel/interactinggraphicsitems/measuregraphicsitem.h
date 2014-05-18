/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#ifndef MEASUREGRAPHICSITEM_H
#define MEASUREGRAPHICSITEM_H

#include <QPen>
#include <common/defines.h>
#include "interactinggraphicsitem.h"

class QGraphicsLinearLayout;

class MeasureGraphicsItem : public InteractingGraphicsItem
{
    friend class MeasureGraphicsItemTest;

public:
    explicit MeasureGraphicsItem(QGraphicsItem *parent = 0);

    enum { Type = MeasureGraphicsItemType };
    int type() const { return Type; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // InteractingGraphicsItem interface
public:
    void insertChildItem(int index, InteractingGraphicsItem *childItem);
    void setData(const QVariant &value, int key);

    // InteractingGraphicsItem interface
protected:
    void musicFontHasChanged(const MusicFontPtr &musicFont);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);

private:
    void setPenWidth(qreal width);
    qreal penWidth() const;
    QGraphicsLinearLayout *m_symbolLayout;
    QPen m_linePen;
};

#endif // MEASUREGRAPHICSITEM_H
