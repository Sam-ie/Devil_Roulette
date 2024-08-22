#ifndef MODEL_H
#define MODEL_H

#include <QSettings>
#include <QStandardPaths>
#include <QDir>

class Model
{
public:
    Model();
    ~Model();

    bool saveSettings();
    bool loadSettings();

public:
    int level;
    int round;
    int blood[2];

};

#endif // MODEL_H
