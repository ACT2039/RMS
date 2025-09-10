#ifndef FOODMENU_H
#define FOODMENU_H

#include <string>
using namespace std;

class Menu {
public:
    void addfooditem(int fid, string fname, int fprice, string fcat);
    void displayfooditem(int fid, string fname, int fprice, string fcat);
    void deletefooditem(int fid);
};

#endif