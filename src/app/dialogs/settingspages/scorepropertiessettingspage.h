/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#ifndef SCOREPROPERTIESSETTINGSPAGE_H
#define SCOREPROPERTIESSETTINGSPAGE_H

#include <QHash>
#include <QWidget>
#include <common/itemdataroles.h>
#include <common/settingdefines.h>

namespace Ui {
class ScorePropertiesSettingsPage;
}

class ScorePropertiesWidget;
class ScoreSettings;

class ScorePropertiesSettingsPage : public QWidget
{
    Q_OBJECT
    friend class ScorePropertiesSettingsPageTest;

public:
    explicit ScorePropertiesSettingsPage(QWidget *parent = 0);
    ~ScorePropertiesSettingsPage();

    void setScoreArea(Settings::Score::Area area);
    Settings::Score::Area scoreArea() const;

private slots:
    void propertyWidgetEnabledChanged(ScorePropertiesWidget *propertyWidget);
    void propertyWidgetColorChanged(ScorePropertiesWidget *propertyWidget);
    void propertyWidgetFontChanged(ScorePropertiesWidget *propertyWidget);

private:
    void initUi();
    void createConnections();
    void appendPropertiesWidget(LP::ScoreDataRole dataRole, const QString &text);
    void initPropertiesWidgetsWithSettings();
    void initPropertiesWidgetWithSettings(LP::ScoreDataRole dataRole, ScorePropertiesWidget *widget);
    int enabledPropertyWidgetsWithPosition(int row, Settings::TextAlignment alignment);
    void checkAllPropertyWidgetPositionValues();
    QHash<LP::ScoreDataRole, ScorePropertiesWidget*> m_propertiesWidgets;
    Settings::Score::Area m_scoreArea;
    ScoreSettings *m_scoreSettings;
    Ui::ScorePropertiesSettingsPage *ui;
};

#endif // SCOREPROPERTIESSETTINGSPAGE_H
