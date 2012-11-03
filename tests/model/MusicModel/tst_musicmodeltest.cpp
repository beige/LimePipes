/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QSignalSpy>
#include <QMetaType>
#include <QStack>
#include <QTemporaryFile>
#include <QXmlStreamReader>
#include "qt_modeltest/modeltest.h"
#include <musicmodel.h>
#include <itemdatatypes.h>
#include <instrument.h>
#include <symbol.h>
#include <score.h>
#include <tune.h>

Q_IMPORT_PLUGIN(lp_musicmodeltestplugin)

class MusicModelTest : public QObject
{
    Q_OBJECT

public:
    MusicModelTest()
        : m_model(0)
    { qRegisterMetaType<QModelIndex>("QModelIndex"); }
    
private Q_SLOTS:
    void initTestcase();
    void cleanupTestcase();
    void init();
    void cleanup();
    void testColumnCount();
    void testInsertScore();
    void testAppendScore();
    void testInsertTuneIntoScore();
    void testAppendTuneToScore();
    void testInsertTuneWithScore();
    void testInsertSymbol();
    void testCallOfOkToInsertChild();
    void testQAbstractItemModelImplementation();
    void testItemForIndex();
    void testClear();
    void testIsScore();
    void testIsTune();
    void testIsSymbol();
    void testSetColumnCount();
    void testSave();

private:
    bool isMusicItemTag(const QStringRef &tagName);
    void checkTagHierarchy(const QStringRef &parentTag, const QStringRef &tag);
    MusicModel *m_model;
    QStringList m_instrumentNames;
    QStringList m_symbolNames;
    QStringList m_musicItemTagNames;
};

void MusicModelTest::initTestcase()
{
    m_instrumentNames = m_model->instrumentNames();
    if (m_instrumentNames.isEmpty()) {
        qWarning("There was no plugin loaded by the model.");
        return;
    }

    m_symbolNames = m_model->symbolNamesForInstrument(m_instrumentNames.at(0));
    if (m_symbolNames.isEmpty()) {
        qWarning("Plugin's instrument has no symbols.");
        return;
    }

    m_musicItemTagNames = QStringList() << "SCORE" << "TUNE" << "SYMBOL" << "LIMEPIPES";
}

void MusicModelTest::cleanupTestcase()
{
}

void MusicModelTest::init()
{
    m_model = new MusicModel(this);
}

void MusicModelTest::cleanup()
{
    delete m_model;
}

void MusicModelTest::testColumnCount()
{
    QVERIFY2(m_model->columnCount(QModelIndex()) == 1, "Wrong column count");
}

void MusicModelTest::testInsertScore()
{
    QModelIndex firstScore = m_model->insertScore(0, "First Title");
    QVERIFY2(m_model->rowCount(QModelIndex()) == 1, "Score was not inserted");
    QVERIFY2(firstScore.isValid(), "No valid Modelindex was returned while inserting score");
    QVERIFY2(firstScore.row() == 0, "Score was inserted in wrong row");
    QVERIFY2(firstScore.column() == 0, "Score was inserted in wrong column");
    QVERIFY2(m_model->data(firstScore, LP::scoreTitle) == "First Title", "Failed score title");

    m_model->setData(firstScore, "Score 1", LP::scoreTitle);
    QModelIndex secondScore = m_model->insertScore(m_model->rowCount(QModelIndex()), "Second Title");
    m_model->setData(secondScore, "Score 2", LP::scoreTitle);
    QVERIFY2(secondScore.data(LP::scoreTitle) == "Score 2", "Score 2's title isn't there");
    QVERIFY2(secondScore.row() == m_model->rowCount(QModelIndex()) - 1, "Score 2 was inserted in wrong row");
    QVERIFY2(secondScore.column() == 0, "Score 2 was inserted in wrong column");

    // Now, the rowsInserted signal should not be called when inserting rows
    QSignalSpy spy(m_model, SIGNAL(rowsInserted(const QModelIndex, int, int)));

    m_model->insertScore(-1, "Failed Score1");
    m_model->insertScore(5, "Failed Score2");

    QVERIFY2(spy.count() == 0, "rowsInserted Signal was emitted" );
}

void MusicModelTest::testAppendScore()
{
    QModelIndex firstScore = m_model->appendScore("First title");
    QVERIFY2(firstScore.isValid(), "Failed appending score");
    QVERIFY2(m_model->data(firstScore, LP::scoreTitle) == "First title", "Failed getting title from appended score");
}

void MusicModelTest::testInsertTuneIntoScore()
{
    QModelIndex score = m_model->insertScore(0, "First Score");
    QVERIFY2(score.isValid(), "Failed inserting score");
    QModelIndex tune = m_model->insertTuneIntoScore(0, score, m_instrumentNames.at(0));
    InstrumentPtr instrument = m_model->data(tune, LP::tuneInstrument).value<InstrumentPtr>();

    QVERIFY2(tune.isValid(), "Failed inserting Tune");
    QVERIFY2(instrument->name() == m_instrumentNames.at(0), "Failed getting instrument back");

    // Now, the rowsInserted signal should not be called when inserting rows
    QSignalSpy spy(m_model, SIGNAL(rowsInserted(const QModelIndex, int, int)));

    m_model->insertTuneIntoScore(-1, score, m_instrumentNames.at(0));
    m_model->insertTuneIntoScore(5, score, m_instrumentNames.at(0));

    QVERIFY2(spy.count() == 0, "rowsInserted Signal was emitted" );
}

void MusicModelTest::testAppendTuneToScore()
{
    QModelIndex score = m_model->insertScore(0, "First Score");
    QVERIFY2(score.isValid(), "Failed inserting score");
    QModelIndex tune = m_model->appendTuneToScore(score, m_instrumentNames.at(0));
    InstrumentPtr instrument = m_model->data(tune, LP::tuneInstrument).value<InstrumentPtr>();

    QVERIFY2(tune.isValid(), "Failed inserting Tune");
    QVERIFY2(instrument->name() == m_instrumentNames.at(0), "Failed getting instrument back");
}

void MusicModelTest::testInsertTuneWithScore()
{
    QModelIndex tune = m_model->insertTuneWithScore(0, "First Score", m_instrumentNames.at(0));
    InstrumentPtr instrument = m_model->data(tune, LP::tuneInstrument).value<InstrumentPtr>();

    QVERIFY2(tune.isValid(), "Failed inserting Tune");
    QVERIFY2(instrument->name() == m_instrumentNames.at(0), "Failed getting score title from index");
    // New tune and new score => tune in row and column 0
    QVERIFY2(tune.column() == 0, "Tune is in wrong column");
    QVERIFY2(tune.row() == 0, "Tune is in wrong row");
}

void MusicModelTest::testInsertSymbol()
{
    QModelIndex tune = m_model->insertTuneWithScore(0, "First Score", m_instrumentNames.at(0));
    QModelIndex symbol1 = m_model->insertSymbol(0, tune, m_symbolNames.at(0));
    QVERIFY2(symbol1.isValid(), "Failed inserting symbol");

    // Now, the rowsInserted signal should not be called when inserting rows
    QSignalSpy spy(m_model, SIGNAL(rowsInserted(const QModelIndex, int, int)));

    m_model->insertSymbol(5, tune, m_symbolNames.at(0));
    m_model->insertSymbol(-1, tune, m_symbolNames.at(0));

    QVERIFY2(spy.count() == 0, "rowsInserted Signal was emitted" );
}

void MusicModelTest::testCallOfOkToInsertChild()
{
    QModelIndex tune = m_model->insertTuneWithScore(0, "First Score", m_instrumentNames.at(0));

    QModelIndex validSymbol = m_model->insertSymbol(0, tune, m_symbolNames.at(0));
    QVERIFY2(validSymbol.isValid(), "Failed inserting valid symbol");

    QModelIndex invalidSymbol = m_model->insertSymbol(0, tune, "invalid symbol name");
    QVERIFY2(invalidSymbol.isValid() == false, "Failed. It was possible to insert a invalid symbol name");
}

void MusicModelTest::testQAbstractItemModelImplementation()
{
    for (int i=0; i < 3; i++) {
        m_model->insertScore(0, "Score Title");
    }

    QModelIndex secondScore = m_model->index(1, 0, QModelIndex());
    QVERIFY2(secondScore.isValid(), "Failed getting valid score");

    QModelIndex tune1 = m_model->insertTuneIntoScore(0, secondScore, m_instrumentNames.at(0));
    m_model->insertTuneIntoScore(0, secondScore, m_instrumentNames.at(0));

    m_model->insertSymbol(0, tune1, m_symbolNames.at(0));
    m_model->insertSymbol(1, tune1, m_symbolNames.at(0));
    m_model->insertSymbol(2, tune1, m_symbolNames.at(0));
    m_model->insertSymbol(1, tune1, m_symbolNames.at(0));

    ModelTest * modelTest = new ModelTest(m_model, this);
    delete modelTest;
}

void MusicModelTest::testItemForIndex()
{
    QModelIndex scoreIndex = m_model->insertScore(0, "Random Title");
    MusicItem *score = m_model->itemForIndex(scoreIndex);
    QVERIFY2(score->data(LP::scoreTitle) == "Random Title", "Failed to get the correct item for an index");
}

void MusicModelTest::testClear()
{
    m_model->insertScore(0, "Title");
    QVERIFY2(m_model->index(0, 0, QModelIndex()).isValid() == true, "At least one item should be in the model for next test");
    m_model->clear();
    QVERIFY2(m_model->index(0, 0, QModelIndex()).isValid() == false, "Model wasn't cleared");
}

void MusicModelTest::testIsScore()
{
    QModelIndex firstScore = m_model->appendScore("First title");
    QVERIFY2(m_model->isIndexScore(firstScore), "Failed, should return true for score");
}

void MusicModelTest::testIsTune()
{
    QModelIndex score = m_model->insertScore(0, "First Score");
    QModelIndex tune = m_model->appendTuneToScore(score, m_instrumentNames.at(0));
    QVERIFY2(m_model->isIndexTune(tune), "Faile, should return true for tune");
}

void MusicModelTest::testIsSymbol()
{
    QModelIndex tune = m_model->insertTuneWithScore(0, "First Score", m_instrumentNames.at(0));
    QModelIndex symbol1 = m_model->insertSymbol(0, tune, m_symbolNames.at(0));
    QVERIFY2(m_model->isIndexSymbol(symbol1), "Failed, should return true for symbol");
}

void MusicModelTest::testSetColumnCount()
{
    QVERIFY2(m_model->columnCount(QModelIndex()) == 1, "Default column count wasn't 1");
    m_model->setColumnCount(4);
    QVERIFY2(m_model->columnCount(QModelIndex()) == 4, "Can't set column count");
}

void MusicModelTest::testSave()
{
    QTemporaryFile tempFile;
    QStack<QStringRef> musicItemTagStack;
    Q_ASSERT(tempFile.open());

    QModelIndex tune = m_model->insertTuneWithScore(0, "First Score", m_instrumentNames.at(0));
    m_model->insertSymbol(0, tune, m_symbolNames.at(0));
    m_model->insertSymbol(0, tune, m_symbolNames.at(0));
    tune = m_model->insertTuneWithScore(0, "Second Score", m_instrumentNames.at(0));
    m_model->insertSymbol(0, tune, m_symbolNames.at(0));
    m_model->insertSymbol(0, tune, m_symbolNames.at(0));

    m_model->save(tempFile.fileName());

    QXmlStreamReader reader(&tempFile);

    while (!reader.atEnd()) {
        reader.readNext();
        if (reader.isStartElement()) {
            if (isMusicItemTag(reader.name())) {
                if (musicItemTagStack.count())
                    checkTagHierarchy(musicItemTagStack.top(), reader.name());
                musicItemTagStack.push(reader.name());
            }
        }
        if (reader.isEndElement()) {
            if (isMusicItemTag(reader.name())) {
                QVERIFY2(musicItemTagStack.pop() == reader.name(), "Music items are not correctly nested");
            }
        }
    }
    QVERIFY2(!reader.hasError(), "Error occured while reading saved xml file");
}

bool MusicModelTest::isMusicItemTag(const QStringRef &tagName)
{
    QString tag = QString().append(tagName);
    return m_musicItemTagNames.contains(tag, Qt::CaseInsensitive);
}

void MusicModelTest::checkTagHierarchy(const QStringRef &parentTag, const QStringRef &tag)
{
    QString parentTagStr = QString().append(parentTag);
    QString tagStr = QString().append(tag);

    if (parentTagStr.compare("LIMEPIPES", Qt::CaseInsensitive) == 0) {
        QVERIFY2(tagStr.compare("SCORE", Qt::CaseInsensitive) == 0, "Failed, tag can't be under LIMEPIPES tag");
    }

    if (parentTagStr.compare("SCORE", Qt::CaseInsensitive) == 0) {
        QVERIFY2(tagStr.compare("TUNE", Qt::CaseInsensitive) == 0, "Failed, tag can't be under SCORE tag");
    }

    if (parentTagStr.compare("TUNE", Qt::CaseInsensitive) == 0) {
        QVERIFY2(tagStr.compare("SYMBOL", Qt::CaseInsensitive) == 0, "Failed, tag can't be under TUNE tag");
    }
}

QTEST_MAIN(MusicModelTest)

#include "tst_musicmodeltest.moc"
