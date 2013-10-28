/**
 * @author Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#ifndef SCOREPROPERTIESITEM_H
#define SCOREPROPERTIESITEM_H

#include <QObject>
#include <QString>
#include "scorepropertiesinterface.h"

class ScorePropertiesItem : public QObject,
                            public ScorePropertiesInterface
{
    Q_OBJECT
public:
    explicit ScorePropertiesItem(QObject *parent = 0);
    ~ScorePropertiesItem() {}

    void linkWithItem(ScorePropertiesItem *item);
    const ScorePropertiesItem *linkedItem() const;

signals:
    void titleChanged(const QString& newTitle);
    void composerChanged(const QString& newComposer);
    void arrangerChanged(const QString& newArranger);
    void yearChanged(const QString& newYear);
    void copyrightChanged(const QString& newCopyright);
    void timeSignatureChanged(const TimeSignature& newTimeSignature);

public slots:
    virtual void setTitle(const QString &title);
    virtual void setComposer(const QString &composer);
    virtual void setArranger(const QString &arranger);
    virtual void setYear(const QString &year);
    virtual void setCopyright(const QString &copyright);
    virtual void setTimeSignature(const TimeSignature &timeSig);

    // ScorePropertiesInterface interface
public:
    QString title() const;
    QString composer() const;
    QString arranger() const;
    QString year() const;
    QString copyright() const;
    TimeSignature timeSignature() const;

private:
    QString m_title;
    QString m_composer;
    QString m_arranger;
    QString m_year;
    QString m_copyright;
    TimeSignature m_timeSignature;
    const ScorePropertiesItem *m_linkedItem;
};

#endif // SCOREPROPERTIESITEM_H