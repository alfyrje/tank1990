#ifndef eagle_h
#define eagle_h

#include "object.h"

class Eagle : public Object {
public:
    Eagle();
    Eagle(double x, double y);

    void destroy();
    void update(int dt);
    bool type;
    bool flag;
};

#endif
