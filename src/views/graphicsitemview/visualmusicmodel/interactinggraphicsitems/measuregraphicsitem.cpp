/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#include <QPen>
#include <QPainter>
#include <QGraphicsLinearLayout>
#include "measuregraphicsitem.h"

const int InitialLineWidth  = 1;

using namespace LP::View;

MeasureGraphicsItem::MeasureGraphicsItem(QGraphicsItem *parent)
    : InteractingGraphicsItem(parent),
      m_symbolLayout(0)
{
    m_symbolLayout = new QGraphicsLinearLayout(Qt::Horizontal, this);
    m_symbolLayout->setSpacing(0);
    m_symbolLayout->setContentsMargins(0, 0, 0, 0);

    setPenWidth(InitialLineWidth);
}

void MeasureGraphicsItem::setPenWidth(qreal width)
{
    if (width <= 0)
        return;

    m_linePen.setWidthF(width);
}

qreal MeasureGraphicsItem::penWidth() const
{
    return m_linePen.widthF();
}

void MeasureGraphicsItem::insertChildItem(int index, InteractingGraphicsItem *childItem)
{
    m_symbolLayout->insertItem(index, childItem);
}

void MeasureGraphicsItem::setData(const QVariant &value, int key)
{

}

void MeasureGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(m_linePen);
    QRectF rect(geometry());

    qreal rightEdge = rect.right();

    painter->drawLine(0, 0, 0, rect.height());
    painter->drawLine(rightEdge, 0, rightEdge, rect.height());
}

void MeasureGraphicsItem::smuflHasChanged(const SMuFLPtr &smufl)
{
    QFont font = smufl->font();
    qreal staffSpace = font.pixelSize() / 4;
    Engravings engravings(smufl->engravings());
    qreal width = engravings.thinBarlineThickness * staffSpace;
    setPenWidth(width);
}
