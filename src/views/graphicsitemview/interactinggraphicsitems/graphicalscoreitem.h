/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#ifndef GRAPHICALSCOREITEM_H
#define GRAPHICALSCOREITEM_H

#include <QList>
#include <QHash>
#include <itemdatatypes.h>
#include "interactinggraphicsitem.h"
#include "graphicitems/textrowwidget.h"
#include <graphicsitemview/scorepropertiesinterface.h>

class TextWidget;
class QGraphicsLinearLayout;

class GraphicalScoreItem : public InteractingGraphicsItem
{
    Q_OBJECT

    friend class GraphicalScoreItemTest;

public:
    explicit GraphicalScoreItem(QGraphicsItem *parent = 0);

    void setItemPosition(LP::ScoreDataRole itemType, int row, TextRowWidget::RowAlignment position);
    bool hasItemPositionForDataRole(LP::ScoreDataRole itemType);

    void setItemText(LP::ScoreDataRole itemType, const QString& text);
    QString itemText(LP::ScoreDataRole itemType);

    void setItemFont(LP::ScoreDataRole itemType, const QFont& font);
    void setItemColor(LP::ScoreDataRole itemType, const QColor& color);

signals:
    void itemTextChanged(LP::ScoreDataRole dataRole, const QString& text);

private slots:
    void textRowItemChanged(TextRowWidget::RowAlignment position, const QString& newText);

private:
    class TextItemPosition {
    public:
        int rowIndex;
        TextRowWidget::RowAlignment rowPosition;
        bool operator ==(const TextItemPosition& other) const
        {
            if (rowIndex == other.rowIndex &&
                    rowPosition == other.rowPosition)
                return true;
            return false;
        }
    };

    void appendRow();
    void addRowsUntilRowIndex(int index);
    QList<TextRowWidget*> m_textRows;
    QGraphicsLinearLayout *m_rowLayout;
    QHash<LP::ScoreDataRole, TextItemPosition> m_itemPositions;
};

#endif // GRAPHICALSCOREITEM_H
