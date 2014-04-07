/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#include <QPainter>
#include "SMuFL/smufl.h"
#include "glyphitem.h"

GlyphItem::GlyphItem(const QString &glyphName, QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    initFromGlyphName(glyphName);
}

void GlyphItem::initFromGlyphName(const QString &glyphName)
{
    SMuFL *smufl = SMuFL::instance();
    if (!smufl) {
        return;
    }

    quint32 codepoint = smufl->codepointForGlyph(glyphName);
    m_char = QChar(codepoint);
}

QRectF GlyphItem::boundingRect() const
{
    return QRectF(0, 0, 20, 20);
}

void GlyphItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!SMuFL::instance())
        return;

    painter->setFont(SMuFL::instance()->font());
    painter->drawText(0, 0, m_char);
}