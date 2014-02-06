/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#ifndef PITCH_H
#define PITCH_H

#include <QMetaType>
#include <QSharedPointer>

class QString;

class Pitch
{
public:
    explicit Pitch();
    explicit Pitch(int staffPos, const QString &name);
    Pitch(const Pitch &other);

    QString name() const { return m_name; }
    int staffPos() const { return m_staffPos; }

private:
    int m_staffPos;
    QString m_name;
};

typedef QSharedPointer<Pitch> PitchPtr;

Q_DECLARE_METATYPE(PitchPtr)

#endif // PITCH_H