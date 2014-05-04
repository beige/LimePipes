/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <common/graphictypes/MusicFont/musicfont.h>
#include <common/pluginmanagerinterface.h>

class QTreeView;
class QDir;
class Instrument;
class MusicModel;
class AddSymbolsDialog;
class NewTuneDialog;
class AboutDialog;
class QAbstractItemModel;
class MusicModelInterface;
class GraphicsItemView;
class GraphicsScene;
class SettingsDialog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_fileNewAction_triggered();
    void on_fileOpenAction_triggered();
    void on_fileSaveAction_triggered();
    void on_fileSaveAsAction_triggered();
    void on_editAddTuneAction_triggered();
    void on_editAddTunePartAction_triggered();
    void on_editAddSymbolsAction_triggered();
    void on_editUndoAction_triggered();
    void on_editRedoAction_triggered();
    void on_helpAboutAction_triggered();
    void on_editSettingsAction_triggered();
    void on_editCreateTestScore_triggered();
    void insertSymbol(int symbolType);
    void setWindowModifiedForUndoStackCleanState(bool clean);

private:
    void initMusicFont();
    void createModelAndView();
    void createMenusAndToolBars();
    void createConnections();
    void createObjectNames();
    void loadFile(const QString &fileName);
    bool saveFile();
    bool saveFileAs();
    bool okToClearData();
    void updateUi();
    QString instrumentFromParentOfCurrentIndex();
    MusicModelInterface *musicModelFromItemModel(QAbstractItemModel *model);

    Ui::MainWindow *ui;
    PluginManager m_pluginManager;
    QTreeView *m_treeView;
    GraphicsItemView *m_graphicsItemView;
    GraphicsScene *m_graphicsScene;
    QAbstractItemModel *m_proxyModel;
    QAbstractItemModel *m_model;
    AddSymbolsDialog *m_addSymbolsDialog;
    AboutDialog *m_aboutDialog;
    SettingsDialog *m_settingsDialog;
    MusicFontPtr m_musicFont;
};

#endif // MAINWINDOW_H
