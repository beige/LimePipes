/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#include <QFont>
#include "scorepropertiessettingspage.h"
#include "scorepropertieswidget.h"
#include <common/scoresettings.h>
#include "ui_scorepropertiessettingspage.h"

using namespace Settings;
using namespace Settings::Score;

ScorePropertiesSettingsPage::ScorePropertiesSettingsPage(Area area, QWidget *parent)
    : QWidget(parent),
      m_scoreArea(area),
      m_scoreSettings(0),
      ui(new Ui::ScorePropertiesSettingsPage)
{
    ui->setupUi(this);

    m_scoreSettings = new ScoreSettings(this);
    m_scoreSettings->setScoreArea(area);

    initUi();
    initPropertiesWidgetsWithSettings();
    connectPropertyWidgets();
}

ScorePropertiesSettingsPage::ScorePropertiesSettingsPage(QWidget *parent)
    : QWidget(parent),
      m_scoreArea(Header),
      ui(new Ui::ScorePropertiesSettingsPage)
{
    ui->setupUi(this);

    m_scoreSettings = new ScoreSettings(this);

    initUi();
    initPropertiesWidgetsWithSettings();
    connectPropertyWidgets();
}

void ScorePropertiesSettingsPage::initUi()
{
    appendPropertiesWidget(LP::ScoreTitle, tr("Title"));
    appendPropertiesWidget(LP::ScoreType, tr("Type"));
    appendPropertiesWidget(LP::ScoreComposer, tr("Composer"));
    appendPropertiesWidget(LP::ScoreArranger, tr("Arranger"));
    appendPropertiesWidget(LP::ScoreYear, tr("Year"));
    appendPropertiesWidget(LP::ScoreCopyright, tr("Copyright"));
}

void ScorePropertiesSettingsPage::appendPropertiesWidget(LP::ScoreDataRole dataRole, const QString& text)
{
    ScorePropertiesWidget *propertyWidget = new ScorePropertiesWidget();
    propertyWidget->setText(text);

    m_propertiesWidgets.insert(dataRole, propertyWidget);
    ui->verticalLayout->addWidget(propertyWidget);
}

void ScorePropertiesSettingsPage::connectPropertyWidgets()
{
    foreach (ScorePropertiesWidget *propertyWidget, m_propertiesWidgets) {
        connect(propertyWidget, &ScorePropertiesWidget::rowChanged,
                [this, propertyWidget] { checkAllPropertyWidgetPositionValues(); });
        connect(propertyWidget, &ScorePropertiesWidget::alignmentChanged,
                [this, propertyWidget] { checkAllPropertyWidgetPositionValues(); });
        connect(propertyWidget, &ScorePropertiesWidget::enabledChanged,
                [this, propertyWidget] { propertyWidgetEnabledChanged(propertyWidget); });
        connect(propertyWidget, &ScorePropertiesWidget::colorChanged,
                [this, propertyWidget] { propertyWidgetColorChanged(propertyWidget); });
        connect(propertyWidget, &ScorePropertiesWidget::fontChanged,
                [this, propertyWidget] { propertyWidgetFontChanged(propertyWidget); });
    }
}

void ScorePropertiesSettingsPage::checkAllPropertyWidgetPositionValues()
{
    foreach (ScorePropertiesWidget *propertyWidget, m_propertiesWidgets.values()) {
        if (!propertyWidget->isWidgetEnabled()) {
            propertyWidget->setPositionIsInUseMessage(false);
            continue;
        }

        int row = propertyWidget->row();
        TextAlignment alignment = propertyWidget->alignment();
        if (propertyWidgetsWithPosition(row, alignment) > 1) {
            propertyWidget->setPositionIsInUseMessage(true);
        } else {
            LP::ScoreDataRole dataRole = m_propertiesWidgets.key(propertyWidget);
            m_scoreSettings->setValue(m_scoreArea, dataRole, Row, propertyWidget->row());
            m_scoreSettings->setValue(m_scoreArea, dataRole, Alignment,
                                      QVariant::fromValue<TextAlignment>(propertyWidget->alignment()));
            propertyWidget->setPositionIsInUseMessage(false);
        }
    }
}

void ScorePropertiesSettingsPage::propertyWidgetEnabledChanged(ScorePropertiesWidget *propertyWidget)
{
    bool enabled = propertyWidget->isWidgetEnabled();

    if (enabled) {
        checkAllPropertyWidgetPositionValues();
        propertyWidgetColorChanged(propertyWidget);
        propertyWidgetFontChanged(propertyWidget);
    }
}

void ScorePropertiesSettingsPage::propertyWidgetColorChanged(ScorePropertiesWidget *propertyWidget)
{
    LP::ScoreDataRole dataRole = m_propertiesWidgets.key(propertyWidget);
    m_scoreSettings->setValue(m_scoreArea, dataRole, Color, propertyWidget->color());
}

void ScorePropertiesSettingsPage::propertyWidgetFontChanged(ScorePropertiesWidget *propertyWidget)
{
    LP::ScoreDataRole dataRole = m_propertiesWidgets.key(propertyWidget);
    m_scoreSettings->setValue(m_scoreArea, dataRole, Font, propertyWidget->font());
}

void ScorePropertiesSettingsPage::initPropertiesWidgetsWithSettings()
{
    foreach (LP::ScoreDataRole dataRole, m_propertiesWidgets.keys()) {
        initPropertiesWidgetWithSettings(dataRole, m_propertiesWidgets.value(dataRole));
    }
}

void ScorePropertiesSettingsPage::initPropertiesWidgetWithSettings(LP::ScoreDataRole dataRole, ScorePropertiesWidget *widget)
{
    m_scoreSettings->setScoreArea(m_scoreArea);
    m_scoreSettings->setDataRole(dataRole);

    widget->setWidgetEnabled(m_scoreSettings->value(Enabled).toBool());
    widget->setFont(m_scoreSettings->value(Font).value<QFont>());
    widget->setColor(m_scoreSettings->value(Color).value<QColor>());
    widget->setAlignment(m_scoreSettings->value(Alignment).value<Settings::TextAlignment>());
    widget->setRow(m_scoreSettings->value(Row).toInt());
}

int ScorePropertiesSettingsPage::propertyWidgetsWithPosition(int row, TextAlignment alignment)
{
    int count = 0;
    foreach (ScorePropertiesWidget *widget, m_propertiesWidgets.values()) {
        if (!widget->isWidgetEnabled())
            continue;

        if (widget->row() == row && widget->alignment() == alignment)
            count++;
    }

    return count;
}

ScorePropertiesSettingsPage::~ScorePropertiesSettingsPage()
{
    delete ui;
}

void ScorePropertiesSettingsPage::setScoreArea(Area area)
{
    if (m_scoreArea == area)
        return;

    m_scoreArea = area;
    m_scoreSettings->setScoreArea(area);
    initPropertiesWidgetsWithSettings();
}

Area ScorePropertiesSettingsPage::scoreArea() const
{
    return m_scoreArea;
}