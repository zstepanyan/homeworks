#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QSettings>
#include <QMap>

class SettingsManager {
public:
    SettingsManager();
    void saveColorIndex(const QString &key, int index);
    int loadColorIndex(const QString &key, int defaultIndex);
    void saveMaxScores(const QMap<int, int> &maxScores);
    QMap<int, int> loadMaxScores();

private:
    QSettings settings;
};

#endif
