/**
 * @author Thomas Baumann <teebaum@ymail.com>
 * 
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 * 
 */

#include <QObject>

class VisualMusicModel;

class VisualMusicModelTest : public QObject {
    Q_OBJECT
public:
    explicit VisualMusicModelTest (QObject *parent=0);

private Q_SLOTS:
    void testSetModel();
    void init();
    void cleanup();

private:
    VisualMusicModel *m_visualMusicModel;
};