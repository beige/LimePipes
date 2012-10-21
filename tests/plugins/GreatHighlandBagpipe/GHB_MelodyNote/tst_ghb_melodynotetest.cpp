/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#include <QtCore/QString>
#include <QtTest/QtTest>

#include <ghb_melodynote.h>

class GHB_MelodyNoteTest : public QObject
{
    Q_OBJECT
    
public:
    GHB_MelodyNoteTest()
        : m_melody(0) {}
    
private Q_SLOTS:
    void init();
    void cleanup();
    void testDefaultConstructor();

private:
    GHB_MelodyNote *m_melody;
};

void GHB_MelodyNoteTest::init()
{
    m_melody = new GHB_MelodyNote(PitchPtr(new Pitch(0, "Low A")), Length::_4);
}

void GHB_MelodyNoteTest::cleanup()
{
    delete m_melody;
}

void GHB_MelodyNoteTest::testDefaultConstructor()
{
    QVERIFY2(m_melody->data(LP::symbolType) == GHB::MelodyNote, "Melody Note has wrong type");
    QVERIFY2(m_melody->data(LP::symbolName) == "Melody Note", "Melody note has wrong name");
    QVERIFY2(m_melody->data(LP::symbolPitch).value<PitchPtr>()->name() == "Low A", "Failed setting pitch in constructor");
    QVERIFY2(m_melody->data(LP::symbolLength).value<Length::Value>() == Length::_4, "Failed setting length in constructor");
}

QTEST_MAIN(GHB_MelodyNoteTest)

#include "tst_ghb_melodynotetest.moc"
