#ifndef AI_H
#define AI_H

#include <QQueue>
#include "model.h"

class AI
{
public:
    static AI& getInstance();
    AI(const AI&) = delete;
    AI& operator=(const AI&) = delete;
    ~AI();

    int get_action();

    void set_shoot(int is_real);
    void bullet_known(int position,bool is_real);
    void reload();

public:
    int want_steal=0; // 1-9

private:
    AI();
    int easy_mod();
    int normal_mod();
    int hard_mod();
    int expert_mod();
    int insane_mod();
    int nightmare_mod();

    QQueue<double> bullet_predict;
    int unknown_total;
    int unknown_real;

};

#endif // AI_H
