/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

/*!
  * @class InstrumentInterface
  * This is the plugin interface for Symbols.
  */

#ifndef SYMBOLINTERFACE_H
#define SYMBOLINTERFACE_H

#include <QtPlugin>
#include <interfaceglobals.h>
#include <model/symbol.h>

class QStringList;
class QString;

class SymbolInterface {
public:
    virtual QStringList symbols() = 0;
    virtual Symbol *getSymbol( const QString &symbol ) = 0;
};

Q_DECLARE_INTERFACE(SymbolInterface,
                    "org.limepipes.LimePipes.SymbolInterface/0.1")

#endif // SYMBOLINTERFACE_H