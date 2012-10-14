/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

/*!
  * @class MainWindow mainwindow.h
  *
  * @fn void createObjectNames()
  * @brief Members derived from QObject, which are declared in the ui file get automatically
  *     assigned an object name. Here the other members can get an object name.
  *     The object name is used in the testclass for getting a pointer to child objects with
  *     QObject->findChild function.
  */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QMessageBox>
#include <instrumentinterface.h>
#include <symbolinterface.h>
#include <musicmodel.h>
#include <musicproxymodel.h>
#include <app/newtunedialog.h>
#include <app/addsymbolsdialog.h>
#include <app/instrumentmanager.h>
#include <views/treeview/treeview.h>

Q_IMPORT_PLUGIN(lp_greathighlandbagpipe)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_addSymbolsDialog = new AddSymbolsDialog(this);
    m_instrumentManager = new InstrumentManager(pluginsDir());

    createModelAndView();
    createConnections();
    createObjectNames();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_instrumentManager;
}

QDir MainWindow::pluginsDir()
{
    QDir pluginsDir(qApp->applicationDirPath());
    pluginsDir.cd("plugins");
    return pluginsDir;
}

void MainWindow::createModelAndView()
{
    m_treeView = new TreeView(this);

    MusicModel *musicModel = new MusicModel(this);
    MusicProxyModel *proxyModel = new MusicProxyModel(this);
    proxyModel->setSourceModel(musicModel);
    m_model = proxyModel;

    m_treeView->setModel(m_model);
    setCentralWidget(m_treeView);
}

void MainWindow::createConnections()
{
    connect(m_addSymbolsDialog, SIGNAL(insertSymbol(QString)),
            this, SLOT(insertSymbol(QString)));
}

void MainWindow::createObjectNames()
{
    m_treeView->setObjectName("treeView");
    m_model->setObjectName("musicModel");
}

InstrumentPtr MainWindow::instrumentFromCurrentIndex()
{
    QModelIndex currentIndex = m_treeView->currentIndex();
    if (currentIndex.isValid()) {
        QVariant currentInstrument = m_model->data(currentIndex, LP::tuneInstrument);
        if (currentInstrument.isValid()) {
            return currentInstrument.value<InstrumentPtr>();
        }
    }
    return InstrumentPtr();
}

MusicModelInterface *MainWindow::musicModelFromItemModel(QAbstractItemModel *model)
{
    return dynamic_cast<MusicModelInterface*>(model);
}

void MainWindow::on_fileNewAction_triggered()
{
    MusicModelInterface *musicModel;
    if ((musicModel = musicModelFromItemModel(m_model)))
        musicModel->clear();
}

void MainWindow::on_editAddTuneAction_triggered()
{
    NewTuneDialog dialog(m_instrumentManager->instrumentNames(), this);
    if (dialog.exec() == QDialog::Accepted) {

        InstrumentPtr instrument = m_instrumentManager->instrumentForName(dialog.instrumentTitle());
        MusicModelInterface *musicModel;
        musicModel = musicModelFromItemModel(m_model);

        if (instrument->type() == LP::NoInstrument ||
            !musicModel)
            return;

        QModelIndex score = musicModel->appendScore(dialog.scoreTitle());
        QModelIndex tune = musicModel->appendTuneToScore(score, instrument);
        m_treeView->setCurrentIndex(tune);
    }
}

void MainWindow::on_editAddSymbolsAction_triggered()
{
    InstrumentPtr instrument = instrumentFromCurrentIndex();
    if (!instrument.isNull() && instrument->type() != LP::NoInstrument) {
        m_addSymbolsDialog->setSymbolNames(
                    m_instrumentManager->symbolNamesForInstrument(instrument->name()));
        m_addSymbolsDialog->show();
    } else {
        QMessageBox message;
        message.setText(tr("Please select a tune"));
        message.exec();
    }
}

void MainWindow::insertSymbol(const QString &symbolName)
{
    InstrumentPtr instrument = instrumentFromCurrentIndex();
    if (instrument && instrument->type() != LP::NoInstrument) {

        Symbol *symbol = m_instrumentManager->symbolForName(instrument->name(), symbolName);
        MusicModelInterface *musicModel;
        musicModel = musicModelFromItemModel(m_model);

        if (symbol->symbolType() == LP::NoSymbolType ||
            !musicModel)
            return;

        musicModel->insertSymbol(0, m_treeView->currentIndex(), symbol);
        m_treeView->expand(m_treeView->currentIndex());
    }
}
