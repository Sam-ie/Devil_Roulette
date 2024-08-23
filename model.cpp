#include <model.h>

Model::Model()
{
    loadSettings();
    if (gun_status.size()==0)
        newRound();
}

Model::~Model()
{
    saveSettings();
}

int Model::use_item(int user,int item_num,int towards_item_num=0)
{
    if (user==0)
        rival_items[item_num]--;
    else if (user==1)
        player_items[item_num]--;

    switch (item_num) {
    case 1:
        blood[user]=std::min(std::min(5,2+level/2),blood[user]+1);
        break;
    case 2:
        jump_next[!user]=true;
        break;
    case 3:
        gun_status.dequeue();
        break;
    case 4:
        gun_status.head()=2;
        break;
    case 5:
        gun_status.head()=!gun_status.head();
        break;
    case 6:
        if (QRandomGenerator::global()->bounded(10)<4)
            blood[user]=std::min(std::min(5,2+level/2),blood[user]+2);
        else
            blood[user]--;
        break;
    case 7:
        if (user==0)
        {
            player_items[towards_item_num]--;
            rival_items[towards_item_num]++;
        }
        else if (user==1)
        {
            rival_items[towards_item_num]--;
            player_items[towards_item_num]++;
        }
        break;
    default:
        break;
    }
    return end_level();
}

int Model::shoot(int shooter,int victim)
{
    if (shooter==victim&&gun_status.head()==0)
        jump_next[!shooter]=true;
    if (shooter!=victim)
        blood[victim]-=gun_status.head();
    whos_turn=!whos_turn;
    if (jump_next[whos_turn])
    {
        jump_next[whos_turn]=false;
        whos_turn=!whos_turn;
    }

    if (!gun_status.size())
        newRound();
    return end_level();
}

int Model::end_level()
{
    if (blood[0]==0)
    {
        level++;
        blood[0]=blood[1]=std::min(5,2+level/2);
        gun_status.clear();
        for (int i = 0; i < 9; ++i)
        {
            rival_items[i]=0;
            player_items[i]=0;
        }
        jump_next[0]=jump_next[1]=false;
        newRound();
        return 1;
    }
    if (blood[1]==0)
    {
        level=std::max(1,level-3);
        blood[0]=blood[1]=std::min(5,2+level/2);
        gun_status.clear();
        for (int i = 0; i < 9; ++i)
        {
            rival_items[i]=0;
            player_items[i]=0;
        }
        jump_next[0]=jump_next[1]=false;
        newRound();
        return -1;
    }
    return 0;
}

void Model::newRound()
{
    int bullet_num=std::min(10,2+level+2*round);
    for (int i = 0; i < bullet_num; ++i) {
        gun_status.enqueue(QRandomGenerator::global()->bounded(2));
    }
    for (int i = 0; i < 9; ++i) // 注释此处即可各轮间继承道具
    {
        rival_items[i]=0;
        player_items[i]=0;
    }
    int items_num=std::min(8,level-1);
    if (level<=5)
    {
        for (int i = 0; i < items_num; ++i) {
            rival_items[QRandomGenerator::global()->bounded(5)]++;
            player_items[QRandomGenerator::global()->bounded(5)]++;
        }
    }
    else
    {
        for (int i = 0; i < items_num; ++i) {
            rival_items[QRandomGenerator::global()->bounded(9)]++;
            player_items[QRandomGenerator::global()->bounded(9)]++;
        }
    }
}


bool Model::saveSettings()
{
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                       + "/settings.ini";
    QDir().mkpath(QFileInfo(filePath).absolutePath()); // Ensure the directory exists
    QSettings settings(filePath, QSettings::IniFormat);

    // Save variables
    settings.setValue("level", level);
    settings.setValue("round", round);
    settings.setValue("whos_turn", whos_turn);

    // Convert and save blood array
    QVariantList bloodList;
    for (int i = 0; i < 2; ++i) {
        bloodList.append(blood[i]);
    }
    settings.setValue("blood", bloodList);

    // Save gun_status queue
    QVariantList gunStatusList;
    while (!gun_status.isEmpty()) {
        gunStatusList.append(gun_status.dequeue());
    }
    settings.setValue("gun_status", gunStatusList);

    // Convert and save items array
    QVariantList rivalItemsList;
    for (int i = 0; i < 9; ++i) {
        rivalItemsList.append(rival_items[i]);
    }
    settings.setValue("rival_items", rivalItemsList);

    QVariantList playerItemsList;
    for (int i = 0; i < 9; ++i) {
        playerItemsList.append(player_items[i]);
    }
    settings.setValue("player_items", playerItemsList);

    // Convert and save jump_next array
    QVariantList jumpNextList;
    for (int i = 0; i < 2; ++i) {
        jumpNextList.append(jump_next[i]);
    }
    settings.setValue("jump_next", jumpNextList);

    return settings.status() == QSettings::NoError;
}

bool Model::loadSettings()
{
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                       + "/settings.ini";
    QSettings settings(filePath, QSettings::IniFormat);

    // Load variables
    level = settings.value("level", 1).toInt();
    round = settings.value("round", 1).toInt();
    whos_turn = settings.value("whos_turn", 1).toInt();

    // Load blood array
    QVariantList bloodList = settings.value("blood").toList();
    for (int i = 0; i < 2; ++i) {
        if (i < bloodList.size()) {
            blood[i] = bloodList[i].toInt();
        } else {
            blood[i] = 2;
        }
    }

    // Load gun_status queue
    QVariantList gunStatusList = settings.value("gun_status").toList();
    for (auto& value : gunStatusList) {
        gun_status.enqueue(value.toInt());
    }

    // Load items array
    QVariantList rivalItemsList = settings.value("rival_items").toList();
    for (int i = 0; i < 9; ++i) {
        if (i < rivalItemsList.size()) {
            rival_items[i] = rivalItemsList[i].toInt();
        } else {
            rival_items[i] = 0;
        }
    }

    QVariantList playerItemsList = settings.value("player_items").toList();
    for (int i = 0; i < 9; ++i) {
        if (i < playerItemsList.size()) {
            player_items[i] = playerItemsList[i].toInt();
        } else {
            player_items[i] = 0;
        }
    }

    // Load jump_next array
    QVariantList jumpNextList = settings.value("jump_next").toList();
    for (int i = 0; i < 2; ++i) {
        if (i < jumpNextList.size()) {
            jump_next[i] = jumpNextList[i].toBool();
        } else {
            jump_next[i] = false;
        }
    }

    return settings.status() == QSettings::NoError;
}
