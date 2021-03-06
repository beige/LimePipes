/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

/*!
  * @class Symbol
  * A Tune consists of Symbols.
  */

#include "symbol.h"
#include <common/datahandling/symbolbehavior.h>
#include <common/datatypes/pitch.h>
#include <common/defines.h>
#include <QXmlStreamWriter>

using namespace LP;

Symbol::Symbol(MusicItem *parent)
    : MusicItem(ItemType::SymbolType, ItemType::NoItemType, parent),
      m_behavior(0)
{
    Q_UNUSED(parent)
}

Symbol::Symbol(int type, const QString &name, MusicItem *parent)
    : MusicItem(ItemType::SymbolType, ItemType::NoItemType),
      m_behavior(0)
{
    Q_UNUSED(parent)
}

Symbol::~Symbol()
{
}

int Symbol::symbolType() const
{
    if (!m_behavior)
        return LP::NoSymbolType;

    return m_behavior->symbolType();
}

bool Symbol::hasPitch() const
{
    if (!m_behavior)
        return false;

    return m_behavior->hasOption(SymbolBehavior::HasPitch);
}

Pitch Symbol::pitch() const
{
    if (!m_behavior)
        return Pitch();

    return m_behavior->data(LP::SymbolPitch).value<Pitch>();
}

bool Symbol::hasLength() const
{
    if (!m_behavior)
        return false;

    return m_behavior->hasOption(SymbolBehavior::HasLength);
}

Length::Value Symbol::length() const
{
    if (!m_behavior)
        return Length::_4;

    return m_behavior->data(LP::SymbolLength).value<Length::Value>();
}

bool Symbol::itemSupportsWritingOfData(int role) const
{
    switch (role) {
    case LP::SymbolPitch:
        if (this->hasPitch())
            return true;
    case LP::SymbolLength:
        if (this->hasLength())
            return true;
    default:
        return false;
    }
}

SymbolBehavior *Symbol::symbolBehavior() const
{
    return m_behavior;
}

void Symbol::setSymbolBehavior(SymbolBehavior *behavior)
{
    m_behavior = behavior;
    MusicItem::setItemBehavior(static_cast<ItemBehavior *>(behavior));
}
