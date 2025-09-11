#ifndef FOODITEM_H
#define FOODITEM_H

#include <string>
using namespace std;

class Fooditem
{
public:
    int fid;
    string fname;
    int fprice;
    string fcat;
};

class vegfooditem : public Fooditem
{
public:
    vegfooditem() { fcat = "vg"; }
};

class nonvegfooditem : public Fooditem
{
public:
    nonvegfooditem() { fcat = "nvg"; }
};

#endif
