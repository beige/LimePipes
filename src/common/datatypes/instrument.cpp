/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#include "instrument.h"

/*! @class Instrument
    @brief A class which provides a Instrument.
*/

Instrument::Instrument()
    : m_type(LP::NoInstrument),
      m_name(QString("No Instrument")),
      m_pitchContext(PitchContextPtr(new PitchContext()))
{
}

Instrument::Instrument(LP::InstrumentType type, const QString &name, PitchContextPtr pitchContext)
    : m_type(type),
      m_name(name),
      m_pitchContext(pitchContext)
{
}

Instrument::Instrument(const Instrument &other)
{
    this->m_type = other.m_type;
    this->m_name = other.m_name;
    this->m_pitchContext = other.m_pitchContext;
}

StaffType Instrument::staffType() const
{
    return m_pitchContext->staffType();
}

ClefType Instrument::defaultClef() const
{
    return m_pitchContext->defaultClef();
}
