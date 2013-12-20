/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#include <QFont>
#include <QSettings>
#include "scoresettings.h"

using namespace Settings::Score;

namespace
{
////////////
///  Keys
///////////

// Header and Footer
QString HeaderSettingsKey("score_header");
QString FooterSettingsKey("score_footer");

// Data type
QString TitleKey("title");
QString ComposerKey("composer");
QString ArrangerKey("arranger");
QString YearKey("year");
QString CopyrightKey("copyright");
QString TypeKey("type");

// Style
QString EnabledKey("enabled");

QString FontKey("font");
QString ColorKey("color");

// Position
QString RowKey("row");
QString AlignmentKey("alignment");

}

QHash <QString, QVariant> ScoreSettings::m_defaultValues = ScoreSettings::initDefaultValues();

QHash<QString, QVariant> ScoreSettings::initDefaultValues()
{
    using namespace Settings;

    QHash<QString, QVariant> defaultValues;
    defaultValues.insert(getKey(Header, LP::ScoreType, Enabled), true);
    defaultValues.insert(getKey(Header, LP::ScoreType, Row), 1);
    defaultValues.insert(getKey(Header, LP::ScoreType, Alignment),
                    QVariant::fromValue<TextAlignment>(TextAlignment::Left));

    defaultValues.insert(getKey(Header, LP::ScoreTitle, Enabled), true);
    defaultValues.insert(getKey(Header, LP::ScoreTitle, Row), 1);
    defaultValues.insert(getKey(Header, LP::ScoreTitle, Alignment),
                    QVariant::fromValue<TextAlignment>(TextAlignment::Center));
    defaultValues.insert(getKey(Header, LP::ScoreTitle, Font), QFont("Arial", 18, QFont::Bold));


    defaultValues.insert(getKey(Header, LP::ScoreComposer, Enabled), true);
    defaultValues.insert(getKey(Header, LP::ScoreComposer, Row), 1);
    defaultValues.insert(getKey(Header, LP::ScoreComposer, Alignment),
                         QVariant::fromValue<TextAlignment>(TextAlignment::Right));

    defaultValues.insert(getKey(Header, LP::ScoreArranger, Enabled), true);
    defaultValues.insert(getKey(Header, LP::ScoreArranger, Row), 2);
    defaultValues.insert(getKey(Header, LP::ScoreArranger, Alignment),
                         QVariant::fromValue<TextAlignment>(TextAlignment::Right));

    defaultValues.insert(getKey(Footer, LP::ScoreYear, Enabled), true);
    defaultValues.insert(getKey(Footer, LP::ScoreYear, Row), 1);
    defaultValues.insert(getKey(Footer, LP::ScoreYear, Alignment),
                         QVariant::fromValue<TextAlignment>(TextAlignment::Left));

    defaultValues.insert(getKey(Footer, LP::ScoreCopyright, Enabled), true);
    defaultValues.insert(getKey(Footer, LP::ScoreCopyright, Row), 1);
    defaultValues.insert(getKey(Footer, LP::ScoreCopyright, Alignment),
                         QVariant::fromValue<TextAlignment>(TextAlignment::Right));

    return defaultValues;
}

QString ScoreSettings::alignmentToString(Settings::TextAlignment alignment)
{
    using namespace Settings;

    switch (alignment) {
    case TextAlignment::Left:
        return "left";
    case TextAlignment::Center:
        return "center";
    case TextAlignment::Right:
        return "right";
    case TextAlignment::NoAlignment:
        break;
    }

    return "";
}

Settings::TextAlignment ScoreSettings::alignmentFromString(const QString &alignment)
{
    using namespace Settings;

    if (alignment == "left")
        return TextAlignment::Left;
    if (alignment == "center")
        return TextAlignment::Center;
    if (alignment == "right")
        return TextAlignment::Right;

    return TextAlignment::NoAlignment;
}

ScoreSettings::ScoreSettings(Area scoreArea, LP::ScoreDataRole dataRole, QObject *parent)
    : QObject(parent),
      m_scoreArea(scoreArea),
      m_dataRole(dataRole)
{
    m_settings = new QSettings(this);
}

ScoreSettings::ScoreSettings(QObject *parent)
    : QObject(parent)
{
    m_settings = new QSettings(this);
}

QVariant ScoreSettings::value(Appearance appearance)
{
    return value(m_scoreArea, m_dataRole, appearance);
}

QVariant ScoreSettings::value(Area area, LP::ScoreDataRole dataRole, Appearance appearance)
{
    QString valueKey(getKey(area, dataRole, appearance));
    QVariant value = m_settings->value(valueKey, defaultValue(valueKey));
    if (appearance == Alignment) {
        Settings::TextAlignment alignment = alignmentFromString(value.toString());
        value = QVariant::fromValue<Settings::TextAlignment>(alignment);
    }

    return value;
}

void ScoreSettings::setValue(Appearance appearance, const QVariant &value)
{
    setValue(m_scoreArea, m_dataRole, appearance, value);
}

void ScoreSettings::setValue(Area area, LP::ScoreDataRole dataRole, Appearance appearance, const QVariant &value)
{
    QString valueKey(getKey(area, dataRole, appearance));
    QVariant writeValue(value);

    if (appearance == Alignment)
        writeValue = alignmentToString(value.value<Settings::TextAlignment>());

    m_settings->setValue(valueKey, writeValue);
}

void ScoreSettings::remove(Appearance appearance)
{
    remove(m_scoreArea, m_dataRole, appearance);
}

void ScoreSettings::remove(Area area, LP::ScoreDataRole dataRole, Appearance appearance)
{
    QString valueKey(getKey(area, dataRole, appearance));
    m_settings->remove(valueKey);
}

Area ScoreSettings::scoreArea() const
{
    return m_scoreArea;
}

void ScoreSettings::setScoreArea(Area scoreArea)
{
    m_scoreArea = scoreArea;
}

LP::ScoreDataRole ScoreSettings::dataRole() const
{
    return m_dataRole;
}

void ScoreSettings::setDataRole(LP::ScoreDataRole dataRole)
{
    m_dataRole = dataRole;
}

QString ScoreSettings::getKey(Area area, LP::ScoreDataRole dataRole, Appearance appearance)
{
    QString key;
    switch (area) {
        case Header:
            key += HeaderSettingsKey;
            break;
        case Footer:
            key += FooterSettingsKey;
    }

    key += "/";

    switch (dataRole) {
    case LP::ScoreTitle: {
        key += TitleKey;
        break;
    }
    case LP::ScoreArranger: {
        key += ArrangerKey;
        break;
    }
    case LP::ScoreComposer: {
        key += ComposerKey;
        break;
    }
    case LP::ScoreCopyright: {
        key += CopyrightKey;
        break;
    }
    case LP::ScoreType: {
        key += TypeKey;
        break;
    }
    case LP::ScoreYear: {
        key += YearKey;
        break;
    }
    }

    key += "/";

    switch (appearance) {
    case Enabled: {
        key += EnabledKey;
        break;
    }
    case Font: {
        key += FontKey;
        break;
    }
    case Color: {
        key += ColorKey;
        break;
    }
    case Row: {
        key += RowKey;
        break;
    }
    case Alignment: {
        key += AlignmentKey;
        break;
    }
    }

    return key;
}

QVariant ScoreSettings::defaultValue(const QString &key)
{
    return m_defaultValues.value(key, QVariant());
}
