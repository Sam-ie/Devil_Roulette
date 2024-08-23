#ifndef MODEL_H
#define MODEL_H

#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QQueue>
#include <QRandomGenerator>

class Model
{
public:
    Model();
    ~Model();

    int use_item(int user,int item_num,int towards_item_num); // 0-8
    int shoot(int shooter,int victim);

public:
    int level; // 第几局
    int round; // 第几轮
    int whos_turn;
    int blood[2]; // 0是对手，1是玩家
    QQueue<int> gun_status; // >0是实弹，=0是空包弹
    int rival_items[9];
    int player_items[9]; // 我不知道为什么items[2][9]会在运行时崩溃
    bool jump_next[2];

private:
    bool saveSettings();
    bool loadSettings();
    void newRound();
    int end_level();
};

#endif // MODEL_H
