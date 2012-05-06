/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#ifndef PARTTEST_H
#define PARTTEST_H

#include <QObject>
#include "../autotest.h"

class PartTest : public QObject
{
    Q_OBJECT
private slots:
    void testType();
};

DECLARE_TEST( PartTest )

#endif // PARTTEST_H
