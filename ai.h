#ifndef AI_H
#define AI_H

#include "model.h"

class AI
{
public:
    static AI& getInstance();
    AI(const AI&) = delete;
    AI& operator=(const AI&) = delete;
    ~AI();

    int get_action();

public:
    int want_steal=0; // 1-9


private:
    AI();
};

#endif // AI_H
