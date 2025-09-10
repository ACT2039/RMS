#include <iostream>
#include <vector>
#include "fooditem.h"
#include "foodmenu.h"

using namespace std;


vector<Fooditem> foodlist;
vector<vegfooditem> vegfoodlist;
vector<nonvegfooditem> nonvegfoodlist;

void Menu::addfooditem(int fid, string fname, int fprice, string fcat) {
    Fooditem f;
    f.fid = fid;
    f.fname = fname;
    f.fprice = fprice;
    f.fcat = fcat;

    if (fcat == "vg") {
        vegfooditem vf;
        vf.fid = fid;
        vf.fname = fname;
        vf.fprice = fprice;
        vegfoodlist.push_back(vf);
    }
    else if (fcat == "nvg") {
        nonvegfooditem nvf;
        nvf.fid = fid;
        nvf.fname = fname;
        nvf.fprice = fprice;
        nonvegfoodlist.push_back(nvf);
    }
    else {
        cout << "Invalid food category!" << endl;
        return;
    }

    foodlist.push_back(f);
    cout << "Food item added successfully!" << endl;
}

void Menu::displayfooditem(int fid, string fname, int fprice, string fcat) {
    if (fcat == "vg") {
        cout << "Vegetarian Food Items:" << endl;
        for (int i = 0; i < vegfoodlist.size(); i++) {
            cout << "Food id:" << vegfoodlist[i].fid << endl;
            cout << "Food name:" << vegfoodlist[i].fname << endl;
            cout << "Food price:" << vegfoodlist[i].fprice << endl;
            cout<<"--------------------------"<<endl;
        }
    }
    else if (fcat == "nvg") {
        cout << "Non-Vegetarian Food Items:" << endl;
        for (int i = 0; i < nonvegfoodlist.size(); i++) {
            cout << "Food id:" << nonvegfoodlist[i].fid << endl;
            cout << "Food name:" << nonvegfoodlist[i].fname << endl;
            cout << "Food price:" << nonvegfoodlist[i].fprice << endl;
            cout<<"--------------------------"<<endl;
        }
    }
    else if (fcat == "all") {
        cout << "All Food Items:" << endl;
        for (int i = 0; i < foodlist.size(); i++) {
            cout << "Food id:" << foodlist[i].fid << endl;
            cout << "Food name:" << foodlist[i].fname << endl;
            cout << "Food price:" << foodlist[i].fprice << endl;
            cout << "Food category:" << foodlist[i].fcat << endl;
            cout <<"--------------------------"<<endl;
        }
    }
    else {
        cout << "Invalid food category!" << endl;
        return;
    }
}

void Menu::deletefooditem(int fid) {
    for (int i = 0; i < foodlist.size(); i++) {
        if (foodlist[i].fid == fid) {
            foodlist.erase(foodlist.begin() + i);
            cout << "Food item deleted successfully!" << endl;
            return;
        }
    }
    cout << "Food item not found!" << endl;
}