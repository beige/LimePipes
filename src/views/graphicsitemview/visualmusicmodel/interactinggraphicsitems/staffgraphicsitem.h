/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#ifndef STAFFGRAPHICSITEM_H
#define STAFFGRAPHICSITEM_H

#include <QPen>
#include <common/defines.h>
#include <common/graphictypes/clefglyphitem.h>
#include "interactinggraphicsitem.h"

class QGraphicsLinearLayout;

class StaffGraphicsItem : public InteractingGraphicsItem
{
    friend class StaffGraphicsItemTest;

public:
    explicit StaffGraphicsItem(QGraphicsItem *parent = 0);

    enum { Type = StaffGraphicsItemType };
    int type() const { return Type; }

    StaffType staffType() const;
    void setStaffType(StaffType type);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // InteractingGraphicsItem interface
    void insertChildItem(int index, InteractingGraphicsItem *childItem);
    int measureCount() const;

    ClefType clefType() const;
    void setClefType(const ClefType &clefType);

private:
    void musicFontHasChanged(const MusicFontPtr &musicFont);
    int staffLineHeight() const;
    void setStaffSpace(qreal staffLineHeight);
    qreal penWidth() const;
    void setPenWidth(qreal width);
    void setSizeHintsForStaffType(StaffType type);
    void setWindowFrameRectForLineWidth(qreal width);
    void setFixedWidthsOnChildren();
    void updateTopMarginToMusicLayout();
    void layoutClef();
    void setMarginsForClefGlyph(qreal glyphWidth);
    qreal clefLeftMargin();
    StaffType m_staffType;
    qreal m_staffSpace;
    qreal m_topMargin;
    QPen m_pen;
    QGraphicsLinearLayout *m_measureLayout;
    ClefGlyphItem *m_clefGlyph;
};

#endif // STAFFGRAPHICSITEM_H
