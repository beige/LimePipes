/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#ifndef MUSICITEM_H
#define MUSICITEM_H

#include <Qt>
#include <QList>
#include <QMap>
#include <QVariant>
#include <QJsonObject>

#include <common/defines.h>
#include <common/datahandling/itembehavior.h>

class MusicItem
{
public:
    explicit MusicItem(LP::ItemType type=LP::ItemType::NoItemType,
                       LP::ItemType childType=LP::ItemType::NoItemType,
                       MusicItem *parent=0);
    virtual ~MusicItem();

    LP::ItemType type() const { return m_type; }
    LP::ItemType childType() const { return m_childType; }

    MusicItem *parent() const { return m_parent; }
    void setParent(MusicItem *parent);

    bool hasChildren() const { return !m_children.isEmpty(); }
    bool insertChild(int row, MusicItem *item);
    bool addChild(MusicItem *item);
    void swapChildren(int oldRow, int newRow) { m_children.swap(oldRow, newRow); }
    int rowOfChild(MusicItem *item) const { return m_children.indexOf(item); }
    int childCount() const { return m_children.count(); }
    MusicItem *childAt(int row) const { return m_children.value(row); }
    MusicItem *takeChild(int row);
    QList<MusicItem*> children() const { return m_children; }
    virtual bool okToInsertChild(const MusicItem *item, int row) { Q_UNUSED(item) Q_UNUSED(row) return true; }

    QVariant data(int role = Qt::UserRole) const;
    bool setData(const QVariant &value, int role);

    virtual bool itemSupportsWritingOfData(int role) const = 0;

    virtual QJsonObject toJson() const;
    virtual void fromJson(const QJsonObject &json);

    ItemBehavior *itemBehavior() const;
    void setItemBehavior(ItemBehavior *itemBehavior);

protected:
    void initData(const QVariant &value, int role) { writeData(value, role); }
    virtual void beforeWritingData(QVariant &value, int role);
    virtual void afterWritingData(int role);
    LP::ItemType m_type;
    LP::ItemType m_childType;
    MusicItem *m_parent;

private:
    void writeData(const QVariant &value, int role);
    QList<MusicItem*> m_children;
    ItemBehavior *m_itemBehavior;
};

class NullMusicItem : public MusicItem
{
public:
    NullMusicItem()
        : MusicItem(LP::ItemType::NoItemType, LP::ItemType::NoItemType) {}
    NullMusicItem(const MusicItem &other)
    {
        m_parent = other.parent();
        m_type = other.type();
        m_childType = other.childType();
    }
    bool itemSupportsWritingOfData(int role) const { Q_UNUSED(role) return true; }
};

#endif // MUSICITEM_H
