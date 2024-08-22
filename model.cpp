#include <model.h>

Model::Model()
{
    loadSettings();
}

Model::~Model()
{
    saveSettings();
}


bool Model::saveSettings()
{
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                       + "/settings.ini";
    QDir().mkpath(QFileInfo(filePath).absolutePath()); // 确保目录存在
    QSettings settings(filePath, QSettings::IniFormat);
    settings.setValue("level", level);

    return settings.status() == QSettings::NoError;
}

bool Model::loadSettings()
{
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                       + "/settings.ini";
    QSettings settings(filePath, QSettings::IniFormat);

    // 读取level和round
    level = settings.value("level", 1).toInt();

    return settings.status() == QSettings::NoError;
}
