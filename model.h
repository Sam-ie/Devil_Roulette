#ifndef MODEL_H
#define MODEL_H

#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QQueue>
#include <QRandomGenerator>
#include <chrono>

class Model
{
public:
    static Model& getInstance();
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;
    ~Model();

    void use_item(int user,int item_num,int towards_item_num=0); // 0-8
    void shoot(int shooter,int victim);
    int determined_winner();

public:
    int level; // 第几关
    int round; // 第几局
    int winning_round;

    int whos_turn;
    int blood[2]; // 0是对手，1是玩家
    QQueue<int> gun_status; // >0是实弹，=0是空包弹
    int rival_items[9];
    int player_items[9]; // 我不知道为什么items[2][9]会在运行时崩溃
    bool jump_next[2];

private:
    Model();
    bool saveSettings();
    bool loadSettings();
    void newTurn(); // 新一轮
};

#endif // MODEL_H
