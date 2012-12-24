/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#include <QtGui/QApplication>
#include <QIcon>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE( musicfont_resources );  // Because model is static plugin

    QApplication app(argc, argv);
    app.setApplicationName(app.translate("main", "LimePipes"));
    app.setWindowIcon(QIcon(":/application/application_icon"));

    MainWindow w;
    w.show();

    return app.exec();
}
