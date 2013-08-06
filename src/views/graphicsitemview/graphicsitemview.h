/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#ifndef GRAPHICSITEMVIEW_H
#define GRAPHICSITEMVIEW_H

#include <QAbstractItemView>
#include "visualmusicmodel/visualmusicmodelinterface.h"

class GraphicsView;

class GraphicsItemView : public QAbstractItemView
{
    Q_OBJECT
public:
    explicit GraphicsItemView(QWidget *parent = 0);
    ~GraphicsItemView();

    QRect visualRect(const QModelIndex &index) const;
    void scrollTo(const QModelIndex &index, ScrollHint hint);
    QModelIndex indexAt(const QPoint &point) const;
    QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);
    int horizontalOffset() const;
    int verticalOffset() const;
    bool isIndexHidden(const QModelIndex &index) const;
    void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command);
    QRegion visualRegionForSelection(const QItemSelection &selection) const;

    void setVisualMusicModel(VisualMusicModelInterface *visualMusicModel);
    VisualMusicModelInterface *visualMusicModel() const;

    void rowsInserted(const QModelIndex &parent, int start, int end);

private:
    void handleInsertScores(int start, int end);
    void handleInsertTunes(const QModelIndex& scoreIndex, int start, int end);
    void handleInsertPartIntoTune(const QModelIndex& tuneIndex, int start, int end);
    void handleInsertMeasureIntoPart(const QModelIndex& partIndex, int start, int end);
    void handleInsertSymbolIntoMeasure(const QModelIndex& measureIndex, int start, int end);
    GraphicsView *m_graphicsView;
    VisualMusicModelInterface *m_visualMusicModel;
};

#endif // GRAPHICSITEMVIEW_H