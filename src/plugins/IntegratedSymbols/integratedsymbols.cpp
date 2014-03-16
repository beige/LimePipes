/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#include <common/defines.h>
#include "integratedsymbols.h"
#include "melodynotegraphicbuilder.h"

IntegratedSymbols::IntegratedSymbols(QObject *parent)
    : QObject(parent)
{
}

QStringList IntegratedSymbols::symbolNames() const
{
    return QStringList();
}

Symbol *IntegratedSymbols::getSymbolForName(const QString &symbol)
{
    return 0;
}

SymbolGraphicBuilder *IntegratedSymbols::symbolGraphicBuilderForType(int type)
{
    if (type == LP::MelodyNote)
        return new MelodyNoteGraphicBuilder();

    return 0;
}

QVector<int> IntegratedSymbols::symbolTypes()
{
    QVector<int> types;
    types << LP::MelodyNote;

    return types;
}