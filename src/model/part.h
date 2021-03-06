/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#ifndef PART_H
#define PART_H

#include <musicitem.h>
#include <common/defines.h>

class Part : public MusicItem
{
public:
    explicit Part(MusicItem *parent=0);

    void setStaffType(StaffType staffType);
    void setClefType(ClefType clef);

    bool itemSupportsWritingOfData(int role) const;

private:
    void initItem();
};

#endif // PART_H
