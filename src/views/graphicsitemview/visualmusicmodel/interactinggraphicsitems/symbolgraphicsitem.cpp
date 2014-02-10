/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#include <QPainter>
#include <itemdatatypes.h>
#include "symbolgraphicsitem.h"

SymbolGraphicsItem::SymbolGraphicsItem(QGraphicsItem *parent)
    : InteractingGraphicsItem(parent)
{
}

void SymbolGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

void SymbolGraphicsItem::setData(const QVariant &value, int key)
{
}
