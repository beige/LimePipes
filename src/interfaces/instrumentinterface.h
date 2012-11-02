/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

/*!
  * @class InstrumentInterface
  * This is the plugin interface for Instruments.
  */

#ifndef INSTRUMENT_INTERFACE_H
#define INSTRUMENT_INTERFACE_H

#include <QtPlugin>
#include <interfaces/interfaceglobals.h>
#include <datatypes/instrument.h>

class QString;

class InstrumentInterface {
public:
    virtual ~InstrumentInterface() {}
    virtual InstrumentPtr instrument() const = 0;
    virtual QString name() const = 0;
};

Q_DECLARE_INTERFACE(InstrumentInterface,
                    "org.limepipes.LimePipes.InstrumentInterface/0.2")

#endif // INSTRUMENT_INTERFACE_H
