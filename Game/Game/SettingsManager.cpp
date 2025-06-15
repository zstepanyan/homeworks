#include "SettingsManager.h"

SettingsManager::SettingsManager() : settings("xAI", "2048Game") {
}

void SettingsManager::saveColorIndex(const QString &key, int index) {
    settings.setValue(key, index);
}

int SettingsManager::loadColorIndex(const QString &key, int defaultIndex) {
    return settings.value(key, defaultIndex).toInt();
}

void SettingsManager::saveMaxScores(const QMap<int, int> &maxScores) {
    settings.beginGroup("MaxScores");
    for (auto it = maxScores.constBegin(); it != maxScores.constEnd(); ++it) {
        settings.setValue(QString("size%1").arg(it.key()), it.value());
    }
    
    settings.endGroup();
}

QMap<int, int> SettingsManager::loadMaxScores() {
    QMap<int, int> maxScores;
    settings.beginGroup("MaxScores");
    QStringList keys = settings.allKeys();
    for (const QString &key : keys) {
        if (key.startsWith("size")) {
            bool ok;
            int size = key.mid(4).toInt(&ok);
            if (ok && size >= 3 && size <= 8) {
                maxScores[size] = settings.value(key, 0).toInt();
            }
        }
    }
    
    settings.endGroup();
    return maxScores;
}
