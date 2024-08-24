#include "ai.h"

AI& AI::getInstance() {
    static AI instance;
    return instance;
}

AI::AI() {}

AI::~AI() {}

int AI::get_action()
{
    // 使用当前时间作为随机数生成器的种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    // 定义分布范围
    std::uniform_int_distribution<int> distribution(10, 11);

    // 生成一个随机数
    return distribution(generator);
    // if (Model::getInstance().gun_status.head()==0)
    //     return 10;
    // else
    //     return 11;
}
