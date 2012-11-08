/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

/*!
 * @class MusicModel
 * @brief The implementation of the QAbstractItemModel.
 */

#include "musicmodel.h"
#include <QDebug>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <rootitem.h>
#include <score.h>
#include <tune.h>
#include <utilities/error.h>

QHash<int, QString> MusicModel::s_itemTypeTags = initItemTypeTags();

QHash<int, QString> MusicModel::initItemTypeTags()
{
    QHash<int, QString> typeTags;
    typeTags.insert(MusicItem::RootItemType, QString("LIMEPIPES"));
    typeTags.insert(MusicItem::ScoreType, QString("SCORE"));
    typeTags.insert(MusicItem::TuneType, QString("TUNE"));
    typeTags.insert(MusicItem::SymbolType, QString("SYMBOL"));
    return typeTags;
}

bool MusicModel::isMusicItemTag(const QString &tagName)
{
    return s_itemTypeTags.values().contains(tagName.toUpper());
}

bool MusicModel::isMusicItemTag(const QStringRef &tagName)
{
    return isMusicItemTag(QString().append(tagName));
}

MusicModel::MusicModel(QObject *parent)
    : QAbstractItemModel(parent), m_rootItem(0), m_columnCount(1)
{
    m_instrumentManager = new InstrumentManager(pluginsDir());
}

MusicModel::~MusicModel()
{
    delete m_rootItem;
    delete m_instrumentManager;
}

QDir MusicModel::pluginsDir()
{
    QDir pluginsDir(qApp->applicationDirPath());
    pluginsDir.cd("plugins");
    return pluginsDir;
}

Qt::ItemFlags MusicModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags theFlags = QAbstractItemModel::flags(index);
    if (index.isValid()) {
        theFlags |= Qt::ItemIsSelectable |
                Qt::ItemIsEnabled |
                Qt::ItemIsEditable;
    }
    return theFlags;
}

QModelIndex MusicModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!m_rootItem || row < 0 || column < 0 || column >= m_columnCount ||
            (parent.isValid() && parent.column() != 0))
        return QModelIndex();
    MusicItem *parentItem = itemForIndex(parent);
    Q_ASSERT(parentItem);
    if (MusicItem *item = parentItem->childAt(row))
        return createIndex(row, column, item);
    return QModelIndex();
}

QModelIndex MusicModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();
    if (MusicItem *childItem = itemForIndex(index)) {
        if (MusicItem *parentItem = childItem->parent()) {
            if (parentItem == m_rootItem)
                return QModelIndex();
            if (MusicItem *grandParentItem = parentItem->parent()) {
                int row = grandParentItem->rowOfChild(parentItem);
                return createIndex(row, 0, parentItem);
            }
        }
    }
    return QModelIndex();
}

int MusicModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return 0;
    MusicItem *parentItem = itemForIndex(parent);
    return parentItem ? parentItem->childCount() : 0;
}

int MusicModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() && parent.column() != 0 ? 0 : m_columnCount;
}

QVariant MusicModel::data(const QModelIndex &index, int role) const
{
    if (!m_rootItem || !index.isValid() || index.column() < 0 ||
            index.column() >= m_columnCount)
        return QVariant();

    if (MusicItem *item = itemForIndex(index)) {
        return item->data(role);
    }
    return QVariant();
}

bool MusicModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.column() != 0)
        return false;
    if (MusicItem *item = itemForIndex(index)) {
        item->setData(value, role);
        return true;
    }
    return false;
}

void MusicModel::setColumnCount(int columns)
{
    if (columns < 1)
        return;
    m_columnCount = columns;
}

bool MusicModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (!m_rootItem)
        return false;

    MusicItem *item = parent.isValid() ? itemForIndex(parent)
                                       : m_rootItem;

    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
        delete item->takeChild(row);
    endRemoveRows();
    return true;
}

QModelIndex MusicModel::insertScore(int row, const QString &title)
{
    createRootItemIfNotPresent();
    Q_ASSERT(m_rootItem->childType() == MusicItem::ScoreType);

    return insertItem(row, QModelIndex(), new Score(title));
}

QModelIndex MusicModel::appendScore(const QString &title)
{
    createRootItemIfNotPresent();
    return insertScore(m_rootItem->childCount(), title);
}

QModelIndex MusicModel::insertTuneIntoScore(int row, const QModelIndex &score, const QString &instrumentName)
{
    InstrumentPtr instrument = m_instrumentManager->instrumentForName(instrumentName);
    return insertItem(row, score, new Tune(instrument));
}

QModelIndex MusicModel::appendTuneToScore(const QModelIndex &score, const QString &instrumentName)
{
    if (MusicItem *item = itemForIndex(score)) {
        return insertTuneIntoScore(item->childCount(), score, instrumentName);
    }
    return QModelIndex();
}

QModelIndex MusicModel::insertTuneWithScore(int rowOfScore, const QString &scoreTitle, const QString &instrumentName)
{
    QModelIndex score = insertScore(rowOfScore, scoreTitle);
    return insertTuneIntoScore(0, score, instrumentName);
}

QModelIndex MusicModel::insertSymbol(int row, const QModelIndex &tune, const QString &symbolName)
{
    MusicItem *tuneItem = itemForIndex(tune);
    if (!tuneItem)
        return QModelIndex();

    QVariant instrumentVar = tuneItem->data(LP::tuneInstrument);
    if (!instrumentVar.isValid() &&
            !instrumentVar.canConvert<InstrumentPtr>())
        return QModelIndex();

    InstrumentPtr instrument = instrumentVar.value<InstrumentPtr>();
    Symbol *symbol = m_instrumentManager->symbolForName(instrument->name(), symbolName);

    if (symbol &&
            symbol->symbolType() == LP::NoSymbolType)
        return QModelIndex();

    return insertItem(row, tune, symbol);
}

MusicItem *MusicModel::itemForIndex(const QModelIndex &index) const
{
    if (index.isValid()) {
        if (MusicItem *item = static_cast<MusicItem*>(
                    index.internalPointer()))
            return item;
    }
    return m_rootItem;
}

void MusicModel::clear()
{
    delete m_rootItem;
    m_rootItem = 0;
    reset();
}

void MusicModel::save(const QString &filename)
{
    if (!filename.isEmpty())
        m_filename = filename;

    if (m_filename.isEmpty())
        throw LP::Error(tr("no filename specified"));

    QFile file(m_filename);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
        throw LP::Error(file.errorString());

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writeMusicItemAndChildren(&writer, m_rootItem);
    writer.writeEndDocument();
}

void MusicModel::writeMusicItemAndChildren(QXmlStreamWriter *writer, MusicItem *musicItem) const
{
    if (!musicItem ||
            musicItem->type() == MusicItem::NoItemType)
        return;

    writer->writeStartElement(tagNameOfMusicItemType(musicItem->type()));

    if (musicItem->type() == MusicItem::RootItemType)
        writer->writeAttribute("VERSION", "0.1");
    if (musicItem->type() == MusicItem::TuneType)
        writeTuneAttributes(writer, musicItem);
    if (musicItem->type() == MusicItem::SymbolType)
        writeSymbolAttributes(writer, musicItem);

    musicItem->writeItemDataToXmlStream(writer);

    foreach (MusicItem *child, musicItem->children())
        writeMusicItemAndChildren(writer, child);

    writer->writeEndElement();
}

void MusicModel::writeTuneAttributes(QXmlStreamWriter *writer, MusicItem *musicItem) const
{
    QVariant instrumentVar = musicItem->data(LP::tuneInstrument);
    if (instrumentVar.isValid() &&
            instrumentVar.canConvert<InstrumentPtr>()) {
        InstrumentPtr instrument = instrumentVar.value<InstrumentPtr>();
        writer->writeAttribute("INSTRUMENT", instrument->name());
    }
}

void MusicModel::writeSymbolAttributes(QXmlStreamWriter *writer, MusicItem *musicItem) const
{
    QVariant symbolNameVar = musicItem->data(LP::symbolName);
    if (symbolNameVar.isValid() &&
            symbolNameVar.canConvert<QString>()) {
        writer->writeAttribute("NAME", symbolNameVar.toString());
    }
}

const QString MusicModel::tagNameOfMusicItemType(MusicItem::Type type) const
{
    return s_itemTypeTags.value(type);
}

void MusicModel::load(const QString &filename)
{
    if (!filename.isEmpty())
        m_filename = filename;

    if (m_filename.isEmpty())
        throw LP::Error(tr("no filename specified"));

    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly))
        throw LP::Error(file.errorString());

    clear();
    createRootItemIfNotPresent();

    QXmlStreamReader reader(&file);
    readMusicItems(&reader, m_rootItem);
    if (reader.hasError())
        throw LP::Error(reader.errorString());

    reset();
}

void MusicModel::readMusicItems(QXmlStreamReader *reader, MusicItem *item)
{
    while (!reader->atEnd()) {
        reader->readNext();
        if (reader->isStartElement()) {
            switch (item->type()) {
            case MusicItem::RootItemType:
                if (!isMusicItemTag(reader->name()))
                    goto no_valid_document;

                processScoreTag(reader, &item);
                break;
            case MusicItem::ScoreType:
                processTuneTag(reader, &item);
                break;
            case MusicItem::TuneType:
                processSymbolTag(reader, &item);
                break;
            case MusicItem::SymbolType:
                readPitchIfSymbolHasPitch(reader, &item);
                item->readCurrentElementFromXmlStream(reader);
                break;
            case MusicItem::NoItemType:
                qWarning() << "An invalid music item can't be parent of anything.";
                break;
            }
        }
        else if (reader->isEndElement()) {
            if (isEndTagOfCurrentItem(reader, item)) {
                item = item->parent();
            }
        }
    }
no_valid_document:
    ;/* Nothing to do */
}

void MusicModel::processScoreTag(QXmlStreamReader *reader, MusicItem **item)
{
    if (isValidScoreTag(reader))
        *item = itemPointerToNewChildItem<Score>(item);
    else
        (*item)->readCurrentElementFromXmlStream(reader);
}

bool MusicModel::isValidScoreTag(QXmlStreamReader *reader)
{
    return tagHasNameOfItemType(reader->name(), MusicItem::ScoreType);
}

bool MusicModel::tagHasNameOfItemType(QStringRef tagname, MusicItem::Type type)
{
    if (tagNameOfMusicItemType(type).compare(tagname, Qt::CaseInsensitive) == 0)
        return true;
    return false;
}

template<typename T>
T *MusicModel::itemPointerToNewChildItem(MusicItem **parent)
{
    return new T(*parent);
}

void MusicModel::processTuneTag(QXmlStreamReader *reader, MusicItem **item)
{
    if (isValidTuneTag(reader))
        *item = newTuneWithInstrument(reader, *item);
    else
        (*item)->readCurrentElementFromXmlStream(reader);
}

bool MusicModel::isValidTuneTag(QXmlStreamReader *reader)
{
    if (tagHasNameOfItemType(reader->name(), MusicItem::TuneType) &&
            tagHasNonEmptyAttribute(reader, "INSTRUMENT") &&
            instrumentNameIsSupported(attributeValue(reader, "INSTRUMENT"))) {
        return true;
    }
    return false;
}

bool MusicModel::tagHasNonEmptyAttribute(QXmlStreamReader *reader, const QString &attributeName)
{
    if (!reader->attributes().hasAttribute(attributeName))
        return false;
    if (!reader->attributes().value(attributeName).isEmpty())
        return true;
    return false;
}

bool MusicModel::instrumentNameIsSupported(const QString &instrumentName)
{
    if (m_instrumentManager->instrumentNames().contains(instrumentName))
        return true;
    return false;
}

QString MusicModel::attributeValue(QXmlStreamReader *reader, const QString &attributeName)
{
    QString value;
    return value.append(reader->attributes().value(attributeName));
}

MusicItem *MusicModel::newTuneWithInstrument(QXmlStreamReader *reader, MusicItem *item)
{
    Tune *tune = itemPointerToNewChildItem<Tune>(&item);
    QString instrumentName = attributeValue(reader, "INSTRUMENT");
    tune->setInstrument(m_instrumentManager->instrumentForName(instrumentName));
    return tune;
}

void MusicModel::processSymbolTag(QXmlStreamReader *reader, MusicItem **item)
{
    if (isValidSymbolTag(reader, *item))
        *item = newSymbolForTuneItem(reader, *item);
    else
        (*item)->readCurrentElementFromXmlStream(reader);
}

void MusicModel::readPitchIfSymbolHasPitch(QXmlStreamReader *reader, MusicItem **item)
{
    Symbol *symbol = static_cast<Symbol*>(*item);
    if (!symbol)
        return;

    if (reader->name() == "PITCH" &&
            symbol->hasPitch()) {
        InstrumentPtr instrument = instrumentFromItem((*item)->parent());
        if (instrument->type() == LP::NoInstrument)
            return;

        QStringList pitchNames(instrument->pitchContext()->pitchNames());
        QString readPitchName = reader->readElementText();
        if (pitchNames.contains(readPitchName)) {
            PitchPtr pitch = instrument->pitchContext()->pitchForName(readPitchName);
            (*item)->setData(QVariant::fromValue<PitchPtr>(pitch), LP::symbolPitch);
        }
    }
}

bool MusicModel::isValidSymbolTag(QXmlStreamReader *reader, MusicItem *item)
{
    if (tagHasNameOfItemType(reader->name(), MusicItem::SymbolType) &&
            tagHasNonEmptyAttribute(reader, "NAME") &&
            symbolNameIsSupported(reader, item)) {
        return true;
    }
    return false;
}

bool MusicModel::symbolNameIsSupported(QXmlStreamReader *reader, MusicItem *tuneItem)
{
    QString symbolNameFromAttribute = attributeValue(reader, "NAME");

    InstrumentPtr instrument = instrumentFromItem(tuneItem);
    if (instrument->type() == LP::NoInstrument)
        return false;

    QStringList symbolNames =
            m_instrumentManager->symbolNamesForInstrument(instrument->name());

    if (symbolNames.contains(symbolNameFromAttribute)) {
        return true;
    }
    return false;
}

InstrumentPtr MusicModel::instrumentFromItem(MusicItem *item)
{
    QVariant instrumentVar = item->data(LP::tuneInstrument);
    if (instrumentVar.isValid() &&
            instrumentVar.canConvert<InstrumentPtr>()) {
        return instrumentVar.value<InstrumentPtr>();
    }
    return InstrumentPtr(new NullInstrument());
}

MusicItem *MusicModel::newSymbolForTuneItem(QXmlStreamReader *reader, MusicItem *item)
{
    QString symbolNameFromAttribute = attributeValue(reader, "NAME");

    InstrumentPtr instrument = instrumentFromItem(item);
    if (instrument->type() == LP::NoInstrument)
        return item;

    MusicItem *parent = item;
    item = m_instrumentManager->symbolForName(instrument->name(), symbolNameFromAttribute);
    item->setParent(parent);
    return item;
}

bool MusicModel::isEndTagOfCurrentItem(QXmlStreamReader *reader, MusicItem *item)
{
    if (isMusicItemTag(reader->name()) &&
            tagNameOfMusicItemType(item->type()).compare(reader->name(), Qt::CaseInsensitive) == 0) {
        return true;
    }
    return false;
}

bool MusicModel::isIndexScore(const QModelIndex &index) const
{
    return indexHasItemType(index, MusicItem::ScoreType);
}

bool MusicModel::isIndexTune(const QModelIndex &index) const
{
    return indexHasItemType(index, MusicItem::TuneType);
}

bool MusicModel::isIndexSymbol(const QModelIndex &index) const
{
    return indexHasItemType(index, MusicItem::SymbolType);
}

bool MusicModel::indexSupportsWritingOfData(const QModelIndex &index, int role) const
{
    if (MusicItem *item = itemForIndex(index)) {
        return item->itemSupportsWritingOfData(role);
    }
    return false;
}

bool MusicModel::indexHasItemType(const QModelIndex &index, MusicItem::Type type) const
{
    if (MusicItem *item = itemForIndex(index)) {
        if (item->type() == type) {
            return true;
        }
    }
    return false;
}

void MusicModel::createRootItemIfNotPresent()
{
    if (!m_rootItem)
        m_rootItem = new RootItem();
}

bool MusicModel::isRowValid(MusicItem *item, int row) const
{
    if (row >= 0 &&
            row <= item->childCount()) {
        return true;
    }
    return false;
}

QModelIndex MusicModel::insertItem(int row, const QModelIndex &parent, MusicItem *item)
{
    if (MusicItem *parentItem = itemForIndex(parent)) {
        if (isRowValid(parentItem, row) && parentItem->okToInsertChild(item)) {
            beginInsertRows(parent, row, row);
            parentItem->insertChild(row, item);
            endInsertRows();
            return index(row, 0, parent);
        }
    }
    return QModelIndex();
}
