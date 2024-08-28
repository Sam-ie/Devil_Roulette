#include "ai.h"

AI& AI::getInstance() {
    static AI instance;
    return instance;
}

AI::AI() {}

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
    if (Model::getInstance().rival_items[6]>0&&Model::getInstance().blood[0]<=5)
        return 7;
    if (Model::getInstance().rival_items[1]>0&&Model::getInstance().blood[0]<=6&&Model::getInstance().blood[0]>2)
        return 2;
    if (Model::getInstance().rival_items[7]>0)
    {
        int order[9]={7,2,4,0,1,6,5,3,8};
        for (int i=0;i<9;i++)
        {
            if (Model::getInstance().player_items[order[i]]>0)
            {
                want_steal=order[i]+1;
                return 8;
            }
        }
    }
    if (Model::getInstance().rival_items[0]>0&&bullet_predict[0]>0&&bullet_predict[0]<1)
        return 1;
    if (Model::getInstance().rival_items[8]>0&&bullet_predict.size()>1)
        return 9;
    if (Model::getInstance().rival_items[4]>0&&bullet_predict[0]>=0.8)
    {
        bullet_predict[0]*=2;
        return 5;
    }
    if (bullet_predict[0]<=0.3)
        return 10;
    if (Model::getInstance().rival_items[3]>0&&bullet_predict[0]>0.3&&bullet_predict[0]<0.7)
        return 4;
    if (Model::getInstance().rival_items[5]>0&&bullet_predict[0]<0.5)
    {
        bullet_predict[0]=1-bullet_predict[0];
        return 6;
    }
    if (Model::getInstance().rival_items[2]>0&&!Model::getInstance().jump_next[1])
        return 3;
    return 11;
}

int AI::insane_mod()
{
    if (Model::getInstance().rival_items[6]>0&&Model::getInstance().blood[0]<=5&&Model::getInstance().blood[0]>1)
        return 7;
    if (Model::getInstance().rival_items[1]>0&&Model::getInstance().blood[0]<=6&&Model::getInstance().blood[0]>2)
        return 2;
    if (Model::getInstance().rival_items[7]>0)
    {
        if (Model::getInstance().player_items[7]>0)
        {
            want_steal=8;
            return 8;
        }
        if (Model::getInstance().player_items[4]>0&&Model::getInstance().rival_items[4]==0&&bullet_predict[0]>=0.8&&bullet_predict[0]<=1)
        {
            want_steal=5;
            bullet_predict[0]*=2;
            return 8;
        }
        if (Model::getInstance().player_items[0]>0&&Model::getInstance().rival_items[0]==0&&bullet_predict[0]>0.2&&bullet_predict[0]<0.8)
        {
            want_steal=1;
            return 8;
        }
        if (Model::getInstance().player_items[2]>0&&Model::getInstance().rival_items[2]==0&&bullet_predict[0]>0.3&&!Model::getInstance().jump_next[1])
        {
            want_steal=3;
            return 8;
        }
        if (Model::getInstance().player_items[1]>0&&Model::getInstance().rival_items[1]==0&&Model::getInstance().blood[0]<=6)
        {
            want_steal=2;
            return 8;
        }
        if (Model::getInstance().player_items[8]>0&&Model::getInstance().rival_items[8]==0)
        {
            want_steal=9;
            return 8;
        }
        if (Model::getInstance().player_items[6]>0&&Model::getInstance().rival_items[6]==0&&Model::getInstance().blood[0]<=6)
        {
            want_steal=7;
            return 8;
        }
        if (Model::getInstance().player_items[5]>0&&Model::getInstance().rival_items[5]==0&&bullet_predict[0]<0.5)
        {
            want_steal=6;
            bullet_predict[0]=1-bullet_predict[0];
            return 8;
        }
        if (Model::getInstance().player_items[3]>0&&Model::getInstance().rival_items[3]==0&&bullet_predict[0]>0.3&&bullet_predict[0]<0.7)
        {
            want_steal=4;
            return 8;
        }
    }
    if (Model::getInstance().rival_items[0]>0&&bullet_predict[0]>0&&bullet_predict[0]<1)
        return 1;
    if (Model::getInstance().rival_items[8]>0&&bullet_predict.size()>1&&bullet_predict[0]>=0.2&&bullet_predict[0]<=0.8)
        return 9;
    if (Model::getInstance().rival_items[4]>0&&bullet_predict[0]>=0.8&&bullet_predict[0]<=1)
    {
        bullet_predict[0]*=2;
        return 5;
    }
    if (Model::getInstance().rival_items[5]>0&&bullet_predict[0]<0.5)
    {
        bullet_predict[0]=1-bullet_predict[0];
        return 6;
    }
    if (bullet_predict[0]<=0.3)
        return 10;
    if (Model::getInstance().rival_items[3]>0&&bullet_predict[0]>0.3&&bullet_predict[0]<0.7)
        return 4;
    if (Model::getInstance().rival_items[2]>0&&!Model::getInstance().jump_next[1])
        return 3;
    return 11;
}

int AI::nightmare_mod()
{
    // bullet_predict是真实子弹状态，unknown_total和unknown_real是玩家知道的子弹状态
    bullet_predict.resize(Model::getInstance().gun_status.size(), 0.0);
    for (int i = 0; i < bullet_predict.size(); i++)
    {
        bullet_predict[i]=Model::getInstance().gun_status[i];
    }

    if (Model::getInstance().rival_items[6]>0&&Model::getInstance().blood[0]<=5&&Model::getInstance().blood[0]>1)
        return 7;
    if (Model::getInstance().rival_items[1]>0&&Model::getInstance().blood[0]<=6&&Model::getInstance().blood[0]>2)
        return 2;
    if (Model::getInstance().rival_items[7]>0)
    {
        if (Model::getInstance().player_items[7]>0)
        {
            want_steal=8;
            return 8;
        }
        if (Model::getInstance().player_items[4]>0&&Model::getInstance().rival_items[4]==0&&bullet_predict[0]>=0.8&&bullet_predict[0]<=1)
        {
            want_steal=5;
            bullet_predict[0]*=2;
            return 8;
        }
        if (Model::getInstance().player_items[0]>0&&Model::getInstance().rival_items[0]==0&&bullet_predict[0]>0.2&&bullet_predict[0]<0.8)
        {
            want_steal=1;
            return 8;
        }
        if (Model::getInstance().player_items[2]>0&&Model::getInstance().rival_items[2]==0&&bullet_predict[0]>0.3&&!Model::getInstance().jump_next[1])
        {
            want_steal=3;
            return 8;
        }
        if (Model::getInstance().player_items[1]>0&&Model::getInstance().rival_items[1]==0&&Model::getInstance().blood[0]<=6)
        {
            want_steal=2;
            return 8;
        }
        if (Model::getInstance().player_items[8]>0&&Model::getInstance().rival_items[8]==0)
        {
            want_steal=9;
            return 8;
        }
        if (Model::getInstance().player_items[6]>0&&Model::getInstance().rival_items[6]==0&&Model::getInstance().blood[0]<=6)
        {
            want_steal=7;
            return 8;
        }
        if (Model::getInstance().player_items[5]>0&&Model::getInstance().rival_items[5]==0&&bullet_predict[0]<0.5)
        {
            want_steal=6;
            bullet_predict[0]=1-bullet_predict[0];
            return 8;
        }
        if (Model::getInstance().player_items[3]>0&&Model::getInstance().rival_items[3]==0&&((double)unknown_real/unknown_total>=0.7||(double)unknown_real/unknown_total<=0.3))
        {
            want_steal=4;
            return 8;
        }
    }
    if (Model::getInstance().rival_items[0]>0&&want_steal!=1)
        return 1;
    if (Model::getInstance().rival_items[8]>0&&want_steal!=9)
        return 9;
    if (Model::getInstance().rival_items[4]>0&&bullet_predict[0]>=0.8&&bullet_predict[0]<=1)
    {
        bullet_predict[0]*=2;
        return 5;
    }
    if (Model::getInstance().rival_items[5]>0&&bullet_predict[0]<0.5)
    {
        bullet_predict[0]=1-bullet_predict[0];
        return 6;
    }
    if (bullet_predict[0]<=0.3)
        return 10;
    if (Model::getInstance().rival_items[3]>0&&((double)unknown_real/unknown_total>=0.7||(double)unknown_real/unknown_total<=0.3))
        return 4;
    if (Model::getInstance().rival_items[2]>0&&!Model::getInstance().jump_next[1])
        return 3;
    return 11;
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
    unknown_total--;
    if (is_real>=1)
        unknown_real--;
    bullet_predict.dequeue();
    for (int i = 0; i < bullet_predict.size(); i++)
    {
        if (bullet_predict[i]>0&&bullet_predict[i]<1)
            bullet_predict[i] = (double)unknown_real/unknown_total;
    }
}

void AI::bullet_known(QQueue<int> gun_status,int position, bool is_real)
{
    unknown_total--;
    if (is_real)
        unknown_real--;
    bullet_predict[position]=is_real;
    for (int i = 0; i < gun_status.size(); i++) {
        if (bullet_predict[i]>0&&bullet_predict[i]<1)
            bullet_predict[i] = (double)unknown_real/unknown_total;
    }
}

void AI::reload(QQueue<int> gun_status)
{
    unknown_total=gun_status.size();
    unknown_real = 0;
    for (int element : gun_status) {
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

bool AI::saveSettings()
{
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                       + "/settings.ini";
    QDir().mkpath(QFileInfo(filePath).absolutePath()); // Ensure the directory exists
    QSettings settings(filePath, QSettings::IniFormat);

    // Save bullet_predict queue
    QVariantList bulletPredictList;
    while (!bullet_predict.isEmpty()) {
        bulletPredictList.append(bullet_predict.dequeue());
    }
    settings.setValue("bullet_predict", bulletPredictList);

    // Save variables
    settings.setValue("unknown_total", unknown_total);
    settings.setValue("unknown_real", unknown_real);

    return settings.status() == QSettings::NoError;
}

bool AI::loadSettings()
{
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                       + "/settings.ini";
    QSettings settings(filePath, QSettings::IniFormat);

    // Load gun_status queue
    QVariantList bulletPredictList = settings.value("bullet_predict").toList();
    for (auto& value : bulletPredictList) {
        bullet_predict.enqueue(value.toInt());
    }

    // Load variables
    unknown_total = settings.value("unknown_total", 0).toInt();
    unknown_real = settings.value("unknown_real", 0).toInt();

    return settings.status() == QSettings::NoError;
}
