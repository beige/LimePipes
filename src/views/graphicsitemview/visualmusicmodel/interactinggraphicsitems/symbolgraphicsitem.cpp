/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#include <QPainter>
#include <QDebug>
#include <common/itemdataroles.h>
#include "symbolgraphicsitem.h"

SymbolGraphicsItem::SymbolGraphicsItem(QGraphicsItem *parent)
    : InteractingGraphicsItem(parent)
{
}

void SymbolGraphicsItem::setGraphicBuilder(SymbolGraphicBuilder *symbolGraphicBuilder)
{
    m_graphicBuilder.reset(symbolGraphicBuilder);
}

SymbolGraphicBuilder *SymbolGraphicsItem::graphicBuilder() const
{
    return m_graphicBuilder.data();
}

void SymbolGraphicsItem::setPluginManager(PluginManager pluginManger)
{
    m_pluginManager = pluginManger;
}

PluginManager SymbolGraphicsItem::pluginManger() const
{
    return m_pluginManager;
}

void SymbolGraphicsItem::setLineHeight(int lineHeigth)
{
    if (m_graphicBuilder.isNull())
        return;

    m_graphicBuilder->setLineHeight(lineHeigth);
}

int SymbolGraphicsItem::lineHeight() const
{
    if (m_graphicBuilder.isNull())
        return 0;

    return m_graphicBuilder->lineHeight();
}

void SymbolGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

void SymbolGraphicsItem::setData(const QVariant &value, int key)
{
    if (!value.isValid())
        return;

    if (key == LP::SymbolType) {
        if (m_pluginManager.isNull()) {
            qWarning() << QLatin1String("Plugin manager isn't set, can't get graphic builder");
            return;
        }
        SymbolGraphicBuilder *graphicBuilder = m_pluginManager->symbolGraphicBuilderForType(value.toInt());
        setGraphicBuilder(graphicBuilder);
    }
}
