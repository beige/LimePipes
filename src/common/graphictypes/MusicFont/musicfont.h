/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#ifndef MUSICFONT_H
#define MUSICFONT_H

#include <QFont>
#include <QSharedPointer>
#include "musicfonttypes.h"

enum class FontColor
{
    Normal,
    Focus,
    Selected
};

class MusicFont
{
public:
    explicit MusicFont();
    virtual ~MusicFont();

    virtual QFont font() const;
    virtual quint32 codepointForGlyph(const QString& glyphname) const;
    virtual quint32 alternateCodepointForGlyph(const QString& glyphname) const;
    virtual Engravings engravings() const;
    virtual GlyphData glyphData(const QString& glyphname);
    virtual QColor fontColor(const FontColor &color) const;

    qreal staffSpace() const;
    qreal halfStaffSpace() const;
};

typedef QSharedPointer<MusicFont> MusicFontPtr;

#endif // MUSICFONT_H
