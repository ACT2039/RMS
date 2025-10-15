#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <set>
#include <fstream>
#include<new>
using namespace std;

class Table {
public:
    int tid;
    int capacity;
    int occupied;

    Table(int id) {
        tid = id;
        capacity = 2;
        occupied = 0;
    }

    int seatsLeft() const {
        return capacity - occupied;
    }

    bool reserve(int members) {
        if (seatsLeft() >= members) {
            occupied += members;
            return true;
        }
        return false;
    }
};
class Fooditem {
public:
    int fid;
    string fname;
    int fprice;
    string fcat;

    Fooditem() {}

    Fooditem(int id, string name, int price, string cat) {
        fid = id;
        fname = name;
        fprice = price;
        fcat = cat;
    }

    friend void showFoodDetails(const Fooditem& f);
};

void showFoodDetails(const Fooditem& f) {
    cout << "[Food ID: " << f.fid
         << ", Name: " << f.fname
         << ", Price: " << f.fprice
         << ", Category: " << f.fcat << "]\n";
}

class VegFooditem : public Fooditem {
public:
    VegFooditem(int id, string name, int price) {
        fid = id;
        fname = name;
        fprice = price;
        fcat = "vg";
    }
};

class NonVegFooditem : public Fooditem {
public:
    NonVegFooditem(int id, string name, int price) {
        fid = id;
        fname = name;
        fprice = price;
        fcat = "nvg";
    }
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
        for (auto& f : foodlist) {
            if (fcat == "all" || f.fcat == fcat) {
                showFoodDetails(f);
            }
        }
    }

    void saveToFile() {
        ofstream ofs("fooditems.txt");
        for (auto& f : foodlist) {
            ofs << f.fid << " " << f.fname << " " << f.fprice << " " << f.fcat << endl;
        }
        ofs.close();
    }
};

class Order {
public:
    string cname;
    int cid;
    vector<pair<Fooditem, int>> items;
    static vector<Order*> orderlist;

    Order(string cn, int id) {
        cname = cn;
        cid = id;
    }

    void addItem(Fooditem f, int qty) {
        items.push_back({f, qty});
    }

    int getTotal() const {
        int total = 0;
        for (auto& p : items) total += p.first.fprice * p.second;
        return total;
    }

    void displayBill() const {
        cout << "\n==== BILL ====\n";
        cout << "Customer ID: " << cid << "\n";
        cout << "Customer: " << cname << "\n";
        for (auto& p : items) {
            cout << " • " << p.first.fname << " x " << p.second
                 << " = " << p.first.fprice * p.second << "\n";
        }
        cout << "Total: " << getTotal() << "\n";
    }

    void saveBillToFile(const string& phno) const {
        ofstream ofs("customers.txt", ios::app);
        ofs << "Customer ID: " << cid << " | Name: " << cname << " | Phone: " << phno << "\n";
        ofs << "Orders:\n";
        for (auto& p : items) {
            ofs << "  " << p.first.fname << " x " << p.second
                << " = " << p.first.fprice * p.second << "\n";
        }
        ofs << "Total: " << getTotal() << "\n";
        ofs << "---------------------------\n";
        ofs.close();
    }
};
class Customer {
private:
    static int nextId;
    int customerId;
    string name;
    string phone;
    int members;

public:
    Customer() {
        customerId = nextId++;
        cout << "\n--- New Customer Entry ---\n";
        cout << "Your Customer ID is: " << customerId << endl;
    }

    void getDetails(TableManager &tManager) {
        cout << "Enter your name: ";
        getline(cin, name);
        cout << "Enter your 10-digit phone number: ";
        getline(cin, phone);
        cout << "Enter total number of members: ";
        cin >> members;
        cin.ignore();

        tManager.assignTable(name, members);
    }

    void placeOrder(Menu& menu) {
        menu.displayMenu();
        Order order(name);
        
        while (true) {
            cout << "Enter Food ID to order (-1 to finish): ";
            int id;
            cin >> id;
            if (id == -1) break;

            Fooditem* item = menu.findFoodById(id);
            if (item) {
                cout << "Enter quantity for " << item->getName() << ": ";
                int qty;
                cin >> qty;
                if (qty > 0) {
                    order.addItem(*item, qty);
                }
            } else {
                cout << "Invalid Food ID. Please try again.\n";
            }
        }
        cin.ignore();
        order.printBill();
        order.saveBillToFile(customerId, phone);
    }
};

int Customer::nextId = 101;
vector<Customer*> Customer::customerlist;

class Manager {
private:
    Menu& menu;

public:
    Manager(Menu& m) : menu(m) {}

    void addFood() {
        int fid, fprice;
        string fname, fcat;
        cout << "Enter food id: ";
        cin >> fid;
        for (auto& f : menu.foodlist) {
            if (f.fid == fid) {
                cout << "Food ID already exists!\n";
                return;
            }
        }
        cout << "Enter food name: ";
        cin >> fname;
        cout << "Enter food price: ";
        cin >> fprice;
        cout << "Enter food category (vg/nvg): ";
        cin >> fcat;

        if (fcat == "vg") menu.foodlist.push_back(VegFooditem(fid, fname, fprice));
        else if (fcat == "nvg") menu.foodlist.push_back(NonVegFooditem(fid, fname, fprice));
        else {
            cout << "Invalid category! Item not added.\n";
            return;
        }

        menu.saveToFile();
        cout << "Food item added successfully!\n";
    }

    void displayMenu() {
        string fcat;
        cout << "Enter category to display (vg/nvg/all): ";
        cin >> fcat;
        menu.displayfooditem(fcat);
    }

    void viewCustomerHistory() {
        ifstream ifs("customers.txt");
        if (!ifs) {
            cout << "No customer history available.\n";
            return;
        }
        cout << "\n=== Customer Order History ===\n";
        string line;
        while (getline(ifs, line)) {
            cout << line << "\n";
        }
        ifs.close();
    }
};
int main() {
    Menu menu;
    TableManager tableManager;
    Manager manager(menu, tableManager);

    menu.loadFromFile();

    int choice;
    do {
        cout << "\n===== RESTAURANT MANAGEMENT SYSTEM =====\n"
             << "1. Manager Section\n"
             << "2. Customer Section\n"
             << "3. Exit\n"
             << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            int managerChoice;
            do {
                cout << "\n--- Manager Menu ---\n"
                     << "1. Add Food Item\n"
                     << "2. View Menu\n"
                     << "3. View Table Status\n"
                     << "4. View Waiting List\n"
                     << "5. View Customer History\n"
                     << "6. Return to Main Menu\n"
                     << "Enter choice: ";
                cin >> managerChoice;
                cin.ignore();

                if (managerChoice == 1) manager.addFoodItem();
                else if (managerChoice == 2) manager.viewMenu();
                else if (managerChoice == 3) manager.viewTables();
                else if (managerChoice == 4) manager.viewWaitingList();
                else if (managerChoice == 5) manager.viewCustomerHistory();

            } while (managerChoice != 6);

        } else if (choice == 2) {
            Customer customer;
            customer.getDetails(tableManager);
            customer.placeOrder(menu);
            cout << "Thank you! Please visit again.\n";
        }
    } while (choice != 3);

    cout << "Exiting program...\n";
    
    return 0;
}
















