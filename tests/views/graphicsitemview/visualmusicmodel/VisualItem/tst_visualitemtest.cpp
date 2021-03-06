/**
 * @author Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#include <QString>
#include "testinteraction.h"
#include "testinteractingitem.h"
#include <views/graphicsitemview/visualmusicmodel/visualitem.h>
#include <views/graphicsitemview/visualmusicmodel/interactinggraphicsitems/interactinggraphicsitem.h>
#include <QtTest/QtTest>

class VisualItemTest : public QObject
{
    Q_OBJECT

public:
    VisualItemTest();

private Q_SLOTS:
    void init();
    void cleanup();
    void testConstructor();
    void testInitialItemType();
    void testInitialGraphicalType();
    void testDefaultInlineGraphic();
    void testDefaultRowGraphics();
    void testSetGetItemType();
    void testSetGetGraphicalType();
    void testInlineGraphic();
    void testAppendRowGraphics();
    void testAppendRowSequenceChangedSignal();
    void testRowCount();
    void testRemoveLastRowGraphics();
    void testRemoveLastRowSequenceChangedSignal();
    void testSetInlineGraphicDataChangedSignal();
    void testAppendRowDataChangedSignal();
    void testSetDataInlineGraphic();
    void testSetDataRowGraphics();
    void testInsertChildImplementationForTwoInlineItems();

private:
    VisualItem *m_visualItem;
};

VisualItemTest::VisualItemTest()
    : m_visualItem(0)
{
}

void VisualItemTest::init()
{
    m_visualItem = new VisualItem();
}

void VisualItemTest::cleanup()
{
    delete m_visualItem;
}

void VisualItemTest::testConstructor()
{
    delete m_visualItem;
    m_visualItem = new VisualItem(VisualItem::VisualScoreItem);

    QVERIFY2(m_visualItem->itemType() == VisualItem::VisualScoreItem, "Item type wasn't set in constructor");
    QVERIFY2(m_visualItem->graphicalType() == VisualItem::NoGraphicalType,
             "Graphical type is not default in constructor without graphical type parameter");

    delete m_visualItem;
    m_visualItem = new VisualItem(VisualItem::VisualMeasureItem, VisualItem::GraphicalRowType);
    QVERIFY2(m_visualItem->itemType() == VisualItem::VisualMeasureItem, "Item type wasn't set in constructor");
    QVERIFY2(m_visualItem->graphicalType() == VisualItem::GraphicalRowType,
             "Graphical type wasn't set in constructor");
}

void VisualItemTest::testInitialItemType()
{
    QVERIFY2(m_visualItem->itemType() == VisualItem::NoVisualItem, "Wrong initial itemtype");
}

void VisualItemTest::testInitialGraphicalType()
{
    QVERIFY2(m_visualItem->graphicalType() == VisualItem::NoGraphicalType, "Wrong initial graphical type");
}

void VisualItemTest::testDefaultInlineGraphic()
{
    QVERIFY2(m_visualItem->inlineGraphic() == 0, "Default inline graphic is not 0");
}

void VisualItemTest::testDefaultRowGraphics()
{
    QVERIFY2(m_visualItem->rowCount() == 0, "Default row graphics is not empty");
}

void VisualItemTest::testSetGetItemType()
{
    m_visualItem->setItemType(VisualItem::VisualMeasureItem);
    QVERIFY2(m_visualItem->itemType() == VisualItem::VisualMeasureItem,
             "Failed setting item type and getting it back");
}

void VisualItemTest::testSetGetGraphicalType()
{
    m_visualItem->setGraphicalType(VisualItem::GraphicalRowType);
    QVERIFY2(m_visualItem->graphicalType() == VisualItem::GraphicalRowType,
             "Failed setting graphical type and getting it back");
}

void VisualItemTest::testInlineGraphic()
{
    InteractingGraphicsItem *graphicsItem  = new InteractingGraphicsItem();
    InteractingGraphicsItem *graphicsItem2 = new InteractingGraphicsItem();
    m_visualItem->setGraphicalType(VisualItem::GraphicalInlineType);

    QVERIFY2(m_visualItem->inlineGraphic() == 0, "Inline graphic is already set");

    // Set item
    m_visualItem->setInlineGraphic(graphicsItem);
    QVERIFY2(m_visualItem->inlineGraphic() == graphicsItem, "Failed setting inline graphic and getting it back");

    // Set new item
    m_visualItem->setInlineGraphic(graphicsItem2);
    QVERIFY2(m_visualItem->inlineGraphic() == graphicsItem2, "Failed setting new inline graphic and getting it back");

    // Set 0 item
    m_visualItem->setInlineGraphic(0);
    QVERIFY2(m_visualItem->inlineGraphic() == graphicsItem2, "Failure, setting 0 pointer was successful");

    // Test on row graphic, should always return 0 for inline graphic
    delete m_visualItem;
    m_visualItem = new VisualItem();
    m_visualItem->setGraphicalType(VisualItem::GraphicalRowType);

    Q_ASSERT(m_visualItem->inlineGraphic() == 0);
    m_visualItem->setInlineGraphic(graphicsItem);
    QVERIFY2(m_visualItem->inlineGraphic() == 0, "Failure, setting inline graphic in graphical row type was successful");

    // Test on no graphical type should also return 0 for inline graphic
    m_visualItem->setGraphicalType(VisualItem::NoGraphicalType);
    Q_ASSERT(m_visualItem->inlineGraphic() == 0);
    m_visualItem->setInlineGraphic(graphicsItem);
    QVERIFY2(m_visualItem->inlineGraphic() == 0, "Failure, setting inline graphic in no graphical item type was successful");

    delete graphicsItem;
    delete graphicsItem2;
}

void VisualItemTest::testAppendRowGraphics()
{
    InteractingGraphicsItem *graphicsItem  = new InteractingGraphicsItem();
    InteractingGraphicsItem *graphicsItem2 = new InteractingGraphicsItem();
    m_visualItem->setGraphicalType(VisualItem::GraphicalRowType);

    QVERIFY2(m_visualItem->rowCount() == 0, "There is already at least one row item");

    // Set item
    m_visualItem->appendRow(graphicsItem);
    QVERIFY2(m_visualItem->rowCount() == 1, "Failed appending row graphic");

    // Append second item => order must fit
    m_visualItem->appendRow(graphicsItem2);
    QVERIFY2(m_visualItem->rowCount() == 2, "Failed appending second row graphic");
    QVERIFY2(m_visualItem->rowGraphics().at(0) == graphicsItem, "Second row graphics was prepended");

    // Append 0 item
    m_visualItem->appendRow(0);
    QVERIFY2(m_visualItem->rowCount() == 2, "Failure, appending 0 pointer was successful");

    // Test on inline graphic, should always return empty row graphics list
    delete m_visualItem;
    m_visualItem = new VisualItem();
    m_visualItem->setGraphicalType(VisualItem::GraphicalInlineType);

    Q_ASSERT(m_visualItem->rowCount() == 0);
    m_visualItem->appendRow(graphicsItem);
    QVERIFY2(m_visualItem->rowCount() == 0, "Failure, appending row graphic on inline item was successful");

    // Test on no graphical type should also return empty list
    m_visualItem->setGraphicalType(VisualItem::NoGraphicalType);
    Q_ASSERT(m_visualItem->rowCount() == 0);
    m_visualItem->appendRow(graphicsItem);
    QVERIFY2(m_visualItem->rowCount() == 0, "Failure, appending row graphic on no graphical item was successful");

    delete graphicsItem;
    delete graphicsItem2;
}

void VisualItemTest::testAppendRowSequenceChangedSignal()
{
    QSignalSpy spy(m_visualItem, SIGNAL(rowSequenceChanged()));
    m_visualItem->setGraphicalType(VisualItem::GraphicalRowType);
    InteractingGraphicsItem *graphicsItem = new InteractingGraphicsItem;
    m_visualItem->appendRow(graphicsItem);
    Q_ASSERT(m_visualItem->rowCount() == 1);

    QVERIFY2(spy.count() == 1, "Row sequence changed wasn't emitted");

    delete graphicsItem;
}

void VisualItemTest::testRowCount()
{
    m_visualItem->setGraphicalType(VisualItem::GraphicalRowType);
    QVERIFY2(m_visualItem->rowCount() == 0, "Wrong default row count");
    InteractingGraphicsItem *graphicsItem = new InteractingGraphicsItem;
    m_visualItem->appendRow(graphicsItem);

    QVERIFY2(m_visualItem->rowCount() == 1, "Wrong row count");

    delete m_visualItem;
    m_visualItem = new VisualItem;
    m_visualItem->setGraphicalType(VisualItem::GraphicalInlineType);
    m_visualItem->setInlineGraphic(graphicsItem);

    QVERIFY2(m_visualItem->rowCount() == 0,
             "Row count for inline items must be always 0");
    delete graphicsItem;
}

void VisualItemTest::testRemoveLastRowGraphics()
{
    TestInteractingItem *graphicsItem  = new TestInteractingItem();
    TestInteraction *interaction = new TestInteraction;
    graphicsItem->setItemInteraction(interaction);
    TestInteractingItem *graphicsItem2 = new TestInteractingItem();
    TestInteraction *interaction2 = new TestInteraction;
    graphicsItem2->setItemInteraction(interaction2);
    m_visualItem->setGraphicalType(VisualItem::GraphicalRowType);

    // Should not crash when no row item added
    m_visualItem->removeLastRow();

    // Append items
    m_visualItem->appendRow(graphicsItem);
    m_visualItem->appendRow(graphicsItem2);
    Q_ASSERT(m_visualItem->rowCount() == 2);

    m_visualItem->removeLastRow();
    QSignalSpy dataChangedSpy(m_visualItem, SIGNAL(dataChanged(QVariant,int)));
    interaction2->emitDataChanged("test data", Qt::DisplayRole);
    QVERIFY2(dataChangedSpy.count() == 0, "ItemInteraction wasn't disconnected from data changed");

    delete graphicsItem;
    delete graphicsItem2;
}

void VisualItemTest::testRemoveLastRowSequenceChangedSignal()
{
    m_visualItem->setGraphicalType(VisualItem::GraphicalRowType);
    InteractingGraphicsItem *graphicsItem = new InteractingGraphicsItem;
    m_visualItem->appendRow(graphicsItem);
    Q_ASSERT(m_visualItem->rowCount() == 1);

    QSignalSpy spy(m_visualItem, SIGNAL(rowSequenceChanged()));
    m_visualItem->removeLastRow();
    QVERIFY2(spy.count() == 1, "Row sequence changed wasn't emitted");
}

void VisualItemTest::testSetInlineGraphicDataChangedSignal()
{
    QSignalSpy spy(m_visualItem, SIGNAL(dataChanged(QVariant,int)));
    m_visualItem->setGraphicalType(VisualItem::GraphicalInlineType);
    InteractingGraphicsItem interactingItem;
    TestInteraction *interaction = new TestInteraction();
    interactingItem.setItemInteraction(interaction);

    m_visualItem->setInlineGraphic(&interactingItem);
    interaction->emitDataChanged(QVariant(), LP::ScoreArranger);
    QVERIFY2(spy.count() == 1, "Data changed signal wasn't emitted");
}

void VisualItemTest::testAppendRowDataChangedSignal()
{
    QSignalSpy spy(m_visualItem, SIGNAL(dataChanged(QVariant,int)));
    m_visualItem->setGraphicalType(VisualItem::GraphicalRowType);
    InteractingGraphicsItem interactingRow;
    TestInteraction *rowInteraction = new TestInteraction();
    interactingRow.setItemInteraction(rowInteraction);

    m_visualItem->appendRow(&interactingRow);

    rowInteraction->emitDataChanged(QVariant(), LP::ScoreComposer);
    QVERIFY2(spy.count() == 1, "Data changed signal wasn't emitted for first row");
}

void VisualItemTest::testSetDataInlineGraphic()
{
    QString testData("test data");
    LP::ScoreDataRole testDataRole = LP::ScoreArranger;
    TestInteractingItem *interactingItem = new TestInteractingItem();
    QSignalSpy spy(interactingItem, SIGNAL(setDataCalled()));

    m_visualItem->setGraphicalType(VisualItem::GraphicalInlineType);
    m_visualItem->setInlineGraphic(interactingItem);
    m_visualItem->setData(testData, testDataRole);

    QVERIFY2(spy.count() == 1, "set data wasn't called on interacting graphics item");
}

void VisualItemTest::testSetDataRowGraphics()
{
    QString testData("test data");
    LP::ScoreDataRole testDataRole = LP::ScoreArranger;
    TestInteractingItem *interactingItem1 = new TestInteractingItem();
    QSignalSpy spyItem1(interactingItem1, SIGNAL(setDataCalled()));
    TestInteractingItem *interactingItem2 = new TestInteractingItem();
    QSignalSpy spyItem2(interactingItem2, SIGNAL(setDataCalled()));

    m_visualItem->setGraphicalType(VisualItem::GraphicalRowType);
    m_visualItem->appendRow(interactingItem1);
    m_visualItem->appendRow(interactingItem2);
    m_visualItem->setData(testData, testDataRole);

    QVERIFY2(spyItem1.count() == 1, "set data wasn't called on first interacting row graphics item");
    QVERIFY2(spyItem2.count() == 1, "set data wasn't called on second interacting row graphics item");
}

void VisualItemTest::testInsertChildImplementationForTwoInlineItems()
{
    // If a inline visual item is inserted as child into inline visual item parent,
    // VisualItem default implementation should call insertChildItem on the parent's
    // InteractingGraphicsItem with the child's InteractingGraphicsItem

    TestInteractingItem *parentInteractingItem = new TestInteractingItem();
    VisualItem *parentVisualItem = new VisualItem();
    parentVisualItem->setGraphicalType(VisualItem::GraphicalInlineType);
    parentVisualItem->setInlineGraphic(parentInteractingItem);

    TestInteractingItem *childInteractingItem = new TestInteractingItem();
    VisualItem *childVisualItem = new VisualItem();
    childVisualItem->setGraphicalType(VisualItem::GraphicalInlineType);
    childVisualItem->setInlineGraphic(childInteractingItem);

    QSignalSpy spy(parentInteractingItem, SIGNAL(insertChildItemCalled()));

    parentVisualItem->insertChildItem(0, childVisualItem);

    QVERIFY2(spy.count() == 1, "insertChildItem wasn't called on InteractingGraphicsItem of child VisualItem");
}

QTEST_APPLESS_MAIN(VisualItemTest)

#include "tst_visualitemtest.moc"
