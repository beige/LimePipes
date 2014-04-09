/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#ifndef VISUALITEMFACTORY_H
#define VISUALITEMFACTORY_H

#include "abstractvisualitemfactory.h"

class VisualItemFactory : public AbstractVisualItemFactory
{
public:
    explicit VisualItemFactory() {}

    VisualItem *createVisualItem(VisualItem::ItemType type);

    void setSmuflForGraphicsItem(InteractingGraphicsItem *graphicsItem);
private:
    VisualItem *newVisualScore();
    VisualItem *newVisualTune();
    VisualItem *newVisualPart();
    VisualItem *newVisualMeasure();
    VisualItem *newVisualSymbol();
};

#endif // VISUALITEMFACTORY_H
