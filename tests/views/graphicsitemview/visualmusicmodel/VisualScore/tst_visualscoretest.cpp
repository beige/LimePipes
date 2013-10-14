/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#include <QString>
#include <QtTest/QtTest>
#include <model/musicmodel.h>
#include <views/graphicsitemview/visualmusicmodel/visualscore.h>

Q_IMPORT_PLUGIN(lp_greathighlandbagpipe)

class VisualScoreTest : public QObject
{
    Q_OBJECT

public:
    VisualScoreTest();

private Q_SLOTS:
    void init();
    void cleanup();
    void testType();
    void testSetDataFromIndex();

private:
    VisualScore *m_visualScore;
    MusicModel *m_musicModel;
};

VisualScoreTest::VisualScoreTest()
{
}

void VisualScoreTest::init()
{
    m_visualScore = new VisualScore();
    m_musicModel = new MusicModel();
}

void VisualScoreTest::cleanup()
{
    delete m_visualScore;
    delete m_musicModel;
}

void VisualScoreTest::testType()
{
    QVERIFY2(m_visualScore->type() == AbstractVisualItem::VisualScoreItem, "Visual score returned wrong type");
}

void VisualScoreTest::testSetDataFromIndex()
{
    Q_ASSERT(m_musicModel->instrumentNames().count());

    QString scoreTitle("Testscore");
    QModelIndex scoreIndex = m_musicModel->insertScore(0, scoreTitle);
    QString scoreComposer("Composer");
    QString scoreArranger("Arranger");
    QString scoreYear("2013");
    QString scoreCopyright("TB");
    TimeSignature scoreTimeSignature(TimeSignature::_9_8);

    m_musicModel->setData(scoreIndex, scoreComposer, LP::scoreComposer);
    m_musicModel->setData(scoreIndex, scoreArranger, LP::scoreArranger);
    m_musicModel->setData(scoreIndex, scoreYear, LP::scoreYear);
    m_musicModel->setData(scoreIndex, scoreCopyright, LP::scoreCopyright);
    m_musicModel->setData(scoreIndex, QVariant::fromValue<TimeSignature>(scoreTimeSignature),
                          LP::scoreTimeSignature);

    m_visualScore->setDataFromIndex(scoreIndex);

    QVERIFY2(m_visualScore->title() == scoreTitle, "Score title wasn't set");
    QVERIFY2(m_visualScore->composer() == scoreComposer, "Score composer wasn't set");
    QVERIFY2(m_visualScore->arranger() == scoreArranger, "Score arranger wasn't set");
    QVERIFY2(m_visualScore->year() == scoreYear, "Score year wasn't set");
    QVERIFY2(m_visualScore->copyright() == scoreCopyright, "Score copyright wasn't set");
    QVERIFY2(m_visualScore->timeSignature() == scoreTimeSignature, "Score time signature wasn't set");
}

QTEST_MAIN(VisualScoreTest)

#include "tst_visualscoretest.moc"
