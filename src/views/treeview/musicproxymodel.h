/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#ifndef MUSICPROXYMODEL_H
#define MUSICPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <common/pluginmanagerinterface.h>
#include <musicmodelinterface.h>

class MusicModel;

class MusicProxyModel : public QSortFilterProxyModel,
        public MusicModelInterface
{
    Q_OBJECT
public:
    enum Columns {
        ItemColumn = 0,
        PitchColumn,
        LengthColumn
    };

    explicit MusicProxyModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void setSourceModel(QAbstractItemModel *sourceModel);

    // MusicModelInterface
    QModelIndex insertScore(int row, const QString &title);
    QModelIndex appendScore(const QString &title);
    QModelIndex insertTuneIntoScore(int row, const QModelIndex &score, const QString &instrumentName);
    QModelIndex insertTuneWithScore(int rowOfScore, const QString &scoreTitle, const QString &instrumentName);
    QModelIndex appendTuneToScore(const QModelIndex &score, const QString &instrumentName);
    QModelIndex insertPartIntoTune(int row, const QModelIndex &tune, int measures, bool withRepeat=false);
    QModelIndex appendPartToTune(const QModelIndex &tune, int measures, bool withRepeat=false);
    QModelIndex insertMeasureIntoPart(int row, const QModelIndex &part);
    QModelIndex appendMeasureToPart(const QModelIndex &part);
    QModelIndex insertSymbolIntoMeasure(int row, const QModelIndex &measure, int type);
    QModelIndex appendSymbolToMeasure(const QModelIndex &measure, int type);

    MusicItem *itemForIndex(const QModelIndex &index) const;

    bool isIndexScore(const QModelIndex &index) const;
    bool isIndexTune(const QModelIndex &index) const;
    bool isIndexPart(const QModelIndex &index) const;
    bool isIndexMeasure(const QModelIndex &index) const;
    bool isIndexSymbol(const QModelIndex &index) const;
    bool indexSupportsWritingOfData(const QModelIndex &index, int role) const;

    void clear();

    void save(const QString &filename=QString());
    void load(const QString &filename);

    QUndoStack *undoStack() const;

    PluginManager pluginManager() const;
    void setPluginManager(const PluginManager &pluginManager);

private:
    MusicModel *musicModel() const;
    QVariant itemColumnData(const QModelIndex &index, int role) const;
    QVariant pitchColumnData(const QModelIndex &index, int role) const;
    QVariant lengthColumnData(const QModelIndex &index, int role) const;
    PluginManager m_pluginManager;
};

#endif // MUSICPROXYMODEL_H
