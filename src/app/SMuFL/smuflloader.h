/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#ifndef SMUFLLOADER_H
#define SMUFLLOADER_H

#include <QObject>
#include <QJsonObject>
#include <common/graphictypes/SMuFL/smufl.h>

class SMuFLLoader : public QObject,
                    public SMuFL
{
    Q_OBJECT
public:
    explicit SMuFLLoader(QObject *parent = 0);

    void setFont(const QFont &font);
    void setFontFromPath(const QString &path);
    void setFontPixelSize(int pixelSize);

    void loadGlyphnamesFromFile(const QString& glyphNamesFilePath);
    void loadFontMetadataFromFile(const QString& fontMetadataFilePath);

    void setInstance(SMuFL *smuflInstance);

    QFont font() const;
    quint32 codepointForGlyph(const QString &glyphname) const;
    quint32 alternateCodepointForGlyph(const QString &glyphname) const;
    Engravings engravings() const;
    GlyphData glyphData(const QString &glyphname);

private:
    void setEngravings(const Engravings &engravings);
    void setEngravingsFromJson(const QJsonObject& json);
    GlyphData glyphDataFromJson(const QJsonObject& json);
    QPointF pointFromJsonValue(const QJsonObject& json, const QString& dataName);
    quint32 codepointFromString(const QString& codepoint) const;
    quint32 codepointFromGlyphAndCodepointType(const QString& glyphname, const QString &codepointType) const;
    QFont m_font;
    Engravings m_engravings;
    QJsonObject m_glyphNames;
    QJsonObject m_fontGlyphs;
};

#endif // SMUFLLOADER_H