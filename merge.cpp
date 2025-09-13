#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <set>
using namespace std;

class Fooditem {
public:
    int fid;
    string fname;
    int fprice;
    string fcat;
    Fooditem() {}
    Fooditem(int id, string name, int price, string cat)
        : fid(id), fname(name), fprice(price), fcat(cat) {}
};

class VegFooditem : public Fooditem {
public:
    VegFooditem(int id, string name, int price)
        : Fooditem(id, name, price, "vg") {}
};

class NonVegFooditem : public Fooditem {
public:
    NonVegFooditem(int id, string name, int price)
        : Fooditem(id, name, price, "nvg") {}
};

class Menu {
private:
    vector<Fooditem> foodlist;
public:
    friend class Manager;
    friend class Customer;
    void displayfooditem(string fcat = "all") {
        if (foodlist.empty()) {
            cout << "No food items available.\n";
            return;
        }
        cout << "\n=== MENU (" << fcat << ") ===\n";
        cout << "Id  Food name     Food Price     Food category\n";
        cout << "------------------------------------------\n";
        for (auto &f : foodlist) {
            if (fcat == "all" || f.fcat == fcat) {
                cout << f.fid << "   " << f.fname << "             " << f.fprice << "              " << f.fcat << endl;
                cout << "------------------------------------------\n";
            }
        }
    }
    void saveToFile() {
        ofstream ofs("fooditems.txt");
        for (auto &f : foodlist) {
            ofs << f.fid << " " << f.fname << " " << f.fprice << " " << f.fcat << endl;
        }
        ofs.close();
    }
};

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




