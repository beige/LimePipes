/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#include <QGraphicsLinearLayout>
#include "graphicitems/textwidget.h"
#include "graphicitems/textrowwidget.h"
#include "graphicalscoreitem.h"

GraphicalScoreItem::GraphicalScoreItem(QGraphicsItem *parent)
    : InteractingGraphicsItem(parent),
      m_rowLayout(0)
{
    m_rowLayout = new QGraphicsLinearLayout(Qt::Vertical, this);
    m_rowLayout->setContentsMargins(0, 0, 0, 0);

    appendRow();
}

void GraphicalScoreItem::appendRow()
{
    TextRowWidget *newRow = new TextRowWidget();
    m_textRows.append(newRow);
    m_rowLayout->addItem(newRow);

    connect(newRow, SIGNAL(textChanged(TextRowWidget::TextPosition,QString)),
            this, SLOT(textRowItemChanged(TextRowWidget::TextPosition,QString)));
}

void GraphicalScoreItem::textRowItemChanged(TextRowWidget::TextPosition position, const QString &newText)
{
    QObject *senderRow = sender();
    TextRowWidget *rowWidget = qobject_cast<TextRowWidget*>(senderRow);
    if (!rowWidget) return;

    int rowIndex = m_textRows.indexOf(rowWidget);
    if (rowIndex == -1) return;

    ItemPosition itemPosition;
    itemPosition.rowIndex = rowIndex;
    itemPosition.rowPosition = position;

    if (!m_itemPostions.values().contains(itemPosition))
        return;

    LP::ScoreDataRole itemType = m_itemPostions.key(itemPosition);
    switch (itemType) {
    case LP::ScoreYear: {
        emit yearChanged(newText);
        break;
    }
    case LP::ScoreCopyright: {
        emit copyrightChanged(newText);
        break;
    }
    case LP::ScoreTimeSignature: {
        TimeSignature timeSignature;
        timeSignature.fromString(newText);
        emit timeSignatureChanged(timeSignature);
        break;
    }
    case LP::ScoreTitle: {
        emit titleChanged(newText);
        break;
    }
    case LP::ScoreArranger: {
        emit arrangerChanged(newText);
        break;
    }
    case LP::ScoreComposer: {
        emit composerChanged(newText);
        break;
    }
    }
}

void GraphicalScoreItem::addRowsUntilRowIndex(int index)
{
    if (m_rowLayout->count() >= index + 1)
        return;

    while (m_rowLayout->count() < index + 1) {
        appendRow();
    }
}

void GraphicalScoreItem::setItemPosition(LP::ScoreDataRole itemType, int row, TextRowWidget::TextPosition position)
{
    ItemPosition itemPosition;
    if (m_itemPostions.contains(itemType))
        itemPosition = m_itemPostions.value(itemType);

    if (row < 0)
        return;

    addRowsUntilRowIndex(row);
    itemPosition.rowIndex = row;
    itemPosition.rowPosition = position;

    m_itemPostions.insert(itemType, itemPosition);
}

void GraphicalScoreItem::setItemText(LP::ScoreDataRole itemType, const QString &text)
{
    if (!m_itemPostions.contains(itemType))
        return;

    ItemPosition position = m_itemPostions.value(itemType);
    if (m_textRows.count() - 1 > position.rowIndex)
        addRowsUntilRowIndex(position.rowIndex);

    TextRowWidget *row = m_textRows.at(position.rowIndex);
    row->setText(position.rowPosition, text);
}

QString GraphicalScoreItem::itemText(LP::ScoreDataRole itemType)
{
    if (!m_itemPostions.contains(itemType))
        return QString();

    ItemPosition position = m_itemPostions.value(itemType);
    if (!m_textRows.count() > position.rowIndex)
        return QString();

    TextRowWidget *row = m_textRows.at(position.rowIndex);
    return row->text(position.rowPosition);
}

void GraphicalScoreItem::setItemFont(LP::ScoreDataRole itemType, const QFont &font)
{
    if (!m_itemPostions.contains(itemType))
        return;

    ItemPosition position = m_itemPostions.value(itemType);
    if (m_textRows.count() - 1 > position.rowIndex)
        addRowsUntilRowIndex(position.rowIndex);

    TextRowWidget *row = m_textRows.at(position.rowIndex);
    row->setFont(position.rowPosition, font);
}

void GraphicalScoreItem::setItemColor(LP::ScoreDataRole itemType, const QColor &color)
{
    if (!m_itemPostions.contains(itemType))
        return;

    ItemPosition position = m_itemPostions.value(itemType);
    if (m_textRows.count() - 1 > position.rowIndex)
        addRowsUntilRowIndex(position.rowIndex);

    TextRowWidget *row = m_textRows.at(position.rowIndex);
    row->setColor(position.rowPosition, color);
}
