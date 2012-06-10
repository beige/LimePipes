/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#ifndef SYMBOL_H
#define SYMBOL_H

#include <QStandardItem>
#include <Qt>
#include "model_itemtypes.h"

class Symbol : public QStandardItem
{
public:
    /*! Describees the valid DataRoles for Symbols */
    enum DataRoles {
        //* Standard Music-Sheet-Roles */
        MusicSheetSymbol = Qt::UserRole + 1,/*!< The Symbol, as it appears in the MusicSheet (QIcon) */
        PitchRole = Qt::UserRole + 2,       /*!< The "vertical" appearance of the Symbol (Pitch) */
        LengthRole = Qt::UserRole + 3,      /*!< The "horizontal" appearance of the Symbol (Length) */

        /* Code-Roles */
        LimePipesCode = Qt::UserRole + 4,   /*!< The LimePipesCode for the Symbol (QString) */
        CustomCode = Qt::UserRole + 5       /*!< A custom Code appearance, e.g. BWW-code (QString) */
    };

    explicit Symbol();
    int type() const { return SymbolType; }
    virtual QStandardItem *pitch() const;

signals:

public slots:

};

#endif // SYMBOL_H
