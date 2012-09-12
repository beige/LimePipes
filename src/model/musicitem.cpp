/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

/*!
  * @class MusicItem musicitem.h
  * @brief MusicItem builds the hierarchic data, which represents a music sheet.
  *
  * @fn bool MusicItem::okToInsertChild(const MusicItem *item)
  * @brief By overwriting this member function, subclasses can restrict the insertion of child items.
  *     E.g. a Tune supports not all available Symbol types to be inserted.
  *     It can check the type and return false if the Symbol is not supported.
  * @param item The item to be inserted.
  * @return True by default.
  */

#include "musicitem.h"

MusicItem::MusicItem(Type type, Type childType, MusicItem *parent)
    : m_type(type), m_childType(childType), m_parent(parent)
{
    if (m_parent)
        m_parent->addChild(this);
}

bool MusicItem::insertChild(int row, MusicItem *item)
{
    if (m_childType == NoItemType)
        return false;

    if (m_childType == item->type()) {
        item->m_parent = this;
        m_children.insert(row, item);
        return true;
    }
    return false;
}

bool MusicItem::addChild(MusicItem *item)
{
    if (m_childType == NoItemType)
        return false;

    if (m_childType == item->type()) {
        item->m_parent = this;
        m_children << item;
        return true;
    }
    return false;
}
