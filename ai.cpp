#include "ai.h"

AI& AI::getInstance() {
    static AI instance;
    return instance;
}

AI::AI()
{
    reload();
}

AI::~AI() {}

int AI::easy_mod()
{
    // 使用当前时间作为随机数生成器的种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    // 定义分布范围
    std::uniform_int_distribution<int> distribution(10, 11);

    // 生成一个随机数
    return distribution(generator);
}

int AI::normal_mod()
{
    int count_true = 0,count_false=0;
    for (int element : Model::getInstance().gun_status) {
        if (element > 0)
            count_true++;
        if (element == 0)
            count_false++;
    }
    double rate=(double)count_true/(count_true+count_false);
    if (rate<0.5)
        return 10;
    else
        return 11;
}

int AI::hard_mod()
{
    if (Model::getInstance().gun_status.head()==0)
        return 10;
    else
        return 11;
}

int AI::expert_mod()
{
    if (Model::getInstance().rival_items[6]>0&&Model::getInstance().blood[0]<=3)
        return 7;
    if (Model::getInstance().rival_items[1]>0&&Model::getInstance().blood[0]<=4)
        return 2;
    if (Model::getInstance().rival_items[7]>0)
    {
        int order[9]={7,2,4,0,1,6,5,3,8};
        for (int i=0;i<9;i++)
        {
            if (Model::getInstance().player_items[order[i]]>0)
            {
                want_steal=i;
                return 8;
            }
        }
    }
    if (Model::getInstance().rival_items[0]>0&&bullet_predict[0]>0&&bullet_predict[0]<1)
        return 1;
    if (Model::getInstance().rival_items[3]>0&&bullet_predict[0]>0&&bullet_predict[0]<1)
        return 4;

    // TODO

    if (Model::getInstance().rival_items[4]>0&&bullet_predict[0]==1)
    {
        bullet_predict[0]*=2;
        return 5;
    }
    if (bullet_predict[0]>=0.5)
        return 10;
    else
        return 11;

}

int AI::insane_mod()
{
    if (Model::getInstance().rival_items[6]>0&&Model::getInstance().blood[0]<=3&&Model::getInstance().blood[0]>1)
        return 7;
    if (Model::getInstance().rival_items[1]>0&&Model::getInstance().blood[0]<=4)
        return 2;
}

int AI::nightmare_mod()
{

}

int AI::get_action()
{
    if (Model::getInstance().level<3)
        return easy_mod();
    else if (Model::getInstance().level<6)
        return normal_mod();
    else if (Model::getInstance().level<9)
        return hard_mod();
    else if (Model::getInstance().level<13)
        return expert_mod();
    else if (Model::getInstance().level<17)
        return insane_mod();
    else if (Model::getInstance().level<20)
        return nightmare_mod();
    else
        return insane_mod();
}

void AI::set_shoot(int is_real)
{
    if (is_real!=-1)
    {
        unknown_total--;
        if (is_real>=1)
            unknown_real--;
    }
    bullet_predict.dequeue();
    for (int i = 0; i < bullet_predict.size(); i++)
    {
        if (bullet_predict[i]>0&&bullet_predict[i]<1)
            bullet_predict[i] = (double)unknown_real/unknown_total;
    }
}

void AI::bullet_known(int position, bool is_real)
{
    unknown_total--;
    if (is_real)
        unknown_real--;
    bullet_predict[position]=is_real;
    for (int i = 0; i < Model::getInstance().gun_status.size(); i++) {
        if (bullet_predict[i]>0&&bullet_predict[i]<1)
            bullet_predict[i] = (double)unknown_real/unknown_total;
    }
}

void AI::reload()
{
    unknown_total=Model::getInstance().gun_status.size();
    unknown_real = 0;
    for (int element : Model::getInstance().gun_status) {
        if (element > 0)
            unknown_real++;
    }

    // 初始化 bullet_predict 队列
    bullet_predict.resize(unknown_total, 0.0);

    // 更新 bullet_predict
    for (int i = 0; i < unknown_total; ++i) {
        bullet_predict[i] = (double)unknown_real/unknown_total;
    }
}
