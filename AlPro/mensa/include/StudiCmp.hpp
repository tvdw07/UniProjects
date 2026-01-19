#pragma once
#include "Studi.hpp"

class StudiCmp {
public:
    bool operator() (const Studi& x, const Studi& y) const {
        return x<y;
    }
};
