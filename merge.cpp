#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ================= FOOD ITEM =================
class Fooditem {
public:
    int fid;
    string fname;
    int fprice;
    string fcat;
};

class vegfooditem : public Fooditem {
public:
    vegfooditem() { fcat = "vg"; }
};

class nonvegfooditem : public Fooditem {
public:
    nonvegfooditem() { fcat = "nvg"; }
};

// ================= MENU =================
class Menu {
public:
    void addfooditem(int fid, string fname, int fprice, string fcat);
    void displayfooditem(int fid, string fname, int fprice, string fcat);
    void deletefooditem(int fid);
};

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

void Menu::displayfooditem(int, string, int, string fcat) {
    if (fcat == "vg") {
        cout << "Vegetarian Food Items:" << endl;
        for (int i = 0; i < vegfoodlist.size(); i++) {
            cout << "Food id:" << vegfoodlist[i].fid << endl;
            cout << "Food name:" << vegfoodlist[i].fname << endl;
            cout << "Food price:" << vegfoodlist[i].fprice << endl;
            cout << "--------------------------" << endl;
        }
    }
    else if (fcat == "nvg") {
        cout << "Non-Vegetarian Food Items:" << endl;
        for (int i = 0; i < nonvegfoodlist.size(); i++) {
            cout << "Food id:" << nonvegfoodlist[i].fid << endl;
            cout << "Food name:" << nonvegfoodlist[i].fname << endl;
            cout << "Food price:" << nonvegfoodlist[i].fprice << endl;
            cout << "--------------------------" << endl;
        }
    }
    else if (fcat == "all") {
        cout << "All Food Items:" << endl;
        for (int i = 0; i < foodlist.size(); i++) {
            cout << "Food id:" << foodlist[i].fid << endl;
            cout << "Food name:" << foodlist[i].fname << endl;
            cout << "Food price:" << foodlist[i].fprice << endl;
            cout << "Food category:" << foodlist[i].fcat << endl;
            cout << "--------------------------" << endl;
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
    cout << "Food item not found!" << endl;
}

// ================= CUSTOMER =================

class Customer {
public:
    string name;
    string phno;
    vector<Customer> customerlist;
    Customer() {
    cout << "Enter your name: ";
    cin >> name;
    cout << "Enter your ph no: ";
    cin >> phno;
    customerlist.push_back(*this);
}


    void display_customers();
    void dineIn(int members);
    void takeAway();
    void homeDelivery();
};




void Customer::display_customers() {
    if(customerlist.size()==0){
        cout<<"No customers visited:("<<endl;
    }
    cout << "=== ALL CUSTOMERS DETAILS ===\n";
    
    for (int i = 0; i < customerlist.size(); i++) {
        cout << "S.No: " << i + 1 << endl;
        cout << "Customer name: " << customerlist[i].name << endl;
        cout << "Customer phno: " << customerlist[i].phno << endl;
        cout << endl;
    }
}

void Customer::dineIn(int members) {
    cout << "Dine-in booking for " << members << " members confirmed.\n";
}

void Customer::takeAway() {
    cout << "Take Away order placed.\n";
}

void Customer::homeDelivery() {
    cout << "Home Delivery order placed.\n";
}

void customer_details() {
    Customer c;
    c.display_customers();
}

// ================= MAIN =================
int main() {
    Menu menu;
    int choice;
    int key;
    do {
        cout << "1. Manager" << endl;
        cout << "2. Customer" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> key;

        if (key == 1) {
            do {
                cout << "1. Add Food Item" << endl;
                cout << "2. Display Food Items" << endl;
                cout << "3. Delete Food Item" << endl;
                cout << "4. All customers details" << endl;
                cout << "5. Exit" << endl;
                cout << "Enter your choice: ";
                cin >> choice;

                if (choice == 1) {
                    int fid, fprice;
                    string fname, fcat;
                    cout << "Enter food id: ";
                    cin >> fid;
                    cout << "Enter food name: ";
                    cin >> fname;
                    cout << "Enter food price: ";
                    cin >> fprice;
                    cout << "Enter food category (vg/nvg): ";
                    cin >> fcat;
                    menu.addfooditem(fid, fname, fprice, fcat);
                }
                else if (choice == 2) {
                    string fcat;
                    cout << "Enter food category to display (vg/nvg/all): ";
                    cin >> fcat;
                    menu.displayfooditem(0, "", 0, fcat);
                }
                else if (choice == 3) {
                    int fid;
                    cout << "Enter food id to delete: ";
                    cin >> fid;
                    menu.deletefooditem(fid);
                }
                else if (choice == 4) {
                    customer_details();
                }
                else if (choice == 5) {
                    cout << "Exiting Manager menu..." << endl;
                }
                else {
                    cout << "Invalid choice! Please try again." << endl;
                }
            } while (choice != 5);
        }
        else if (key == 2) {
            cout<<"Please enter your details."<<endl;
            Customer c1;

            cout << "1. Dine In" << endl;
            cout << "2. Take Away" << endl;
            cout << "3. Home Delivery" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            if (choice == 1) {
                cout << "Total how many members ?: ";
                int members;
                cin >> members;
                c1.dineIn(members);
            }
            else if (choice == 2) {
                c1.takeAway();
            }
            else if (choice == 3) {
                c1.homeDelivery();
            }

            string fcat;
            cout << "Enter food category to display (vg/nvg/all): ";
            cin >> fcat;
            menu.displayfooditem(0, "", 0, fcat);
        }
        else if (key == 3) {
            cout << "Exiting program..." << endl;
        }
        else {
            cout << "Invalid choice! Please try again." << endl;
        }
    } while (key != 3);

    return 0;
}
