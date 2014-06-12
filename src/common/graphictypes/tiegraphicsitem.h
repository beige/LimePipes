/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#ifndef TIEGRAPHICSITEM_H
#define TIEGRAPHICSITEM_H

#include <QList>
#include <QGraphicsItem>

class GlyphItem;

class TieGraphicsItem : public QGraphicsItem
{
public:
    explicit TieGraphicsItem(QGraphicsItem *parent = 0);

    void addGlyph(GlyphItem *item);
    void removeGlyph(GlyphItem *item);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    QList<GlyphItem*> m_spanningGlyphs;
};

#endif // TIEGRAPHICSITEM_H
