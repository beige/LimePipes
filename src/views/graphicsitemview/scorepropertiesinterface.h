/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#ifndef SCOREPROPERTIESINTERFACE_H
#define SCOREPROPERTIESINTERFACE_H

class QString;

class ScorePropertiesInterface
{
public:
    explicit ScorePropertiesInterface() {}
    virtual ~ScorePropertiesInterface() {}

    virtual QString title() const = 0;
    virtual void setTitle(const QString& title) = 0;

    virtual QString composer() const = 0;
    virtual void setComposer(const QString& composer) = 0;
};

#endif // SCOREPROPERTIESINTERFACE_H
