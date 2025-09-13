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

vector<Order> Order::orderlist;
class Order {
public:
    string cname;
    int cid;
    vector<pair<Fooditem, int>> items;
    static vector<Order> orderlist;
    Order(string cn, int id) : cname(cn), cid(id) {}
    void addItem(Fooditem f, int qty) {
        items.push_back({f, qty});
    }
    int getTotal() const {
        int total = 0;
        for (auto &p : items) total += p.first.fprice * p.second;
        return total;
    }
    void displayBill() const {
        cout << "\n==== BILL ====\n";
        cout << "Customer ID: " << cid << "\n";
        cout << "Customer: " << cname << "\n";
        cout << "Items:\n";
        for (auto &p : items) {
            cout << " • " << p.first.fname << " x " << p.second
                 << " = " << p.first.fprice * p.second << "\n";
        }
        cout << "---------------------\n";
        cout << "Total: " << getTotal() << "\n";
        cout << "=====================\n";
    }
    void saveBillToFile() const {
        ofstream ofs("bills.txt", ios::app);
        ofs << "Customer ID: " << cid << " Name: " << cname << endl;
        for (auto &p : items) {
            ofs << p.first.fname << " x" << p.second << " =" << (p.first.fprice * p.second) << endl;
        }
        ofs << "Total: " << getTotal() << endl;
        ofs << "-----------------------------" << endl;
        ofs.close();
    }
};
vector<Order> Order::orderlist;
class Customer {
public:
    static int nextId;
    int cid;
    string name;
    string phno;
    static vector<Customer> customerlist;
    Customer() {
        cid = ++nextId;
        cout << "---~ WELCOME ~---\n";
    }
    void add_details() {
        cout << "Please enter your details.\n";
        cin.ignore();
        cout << "Enter your name: ";
        getline(cin, name);
        while (true) {
            cout << "Enter your phno.: ";
            cin >> phno;
            if (phno.size() == 10) {
                break;
            } else {
                cout << "Invalid phone number. Please try again!\n";
            }
        }
        customerlist.push_back(*this);
        cout << "Your Customer ID is: " << cid << "\n";
    }
    void dineIn(int members, Menu &menu) {
        cout << "Dine-in booking for " << members << " members confirmed.\n";
        placeOrder(menu, members);
    }
    void placeOrder(Menu &menu, int members) {
        string fcat;
        cout << "Enter food category (vg/nvg/all): ";
        cin >> fcat;
        menu.displayfooditem(fcat);
        if (menu.foodlist.empty()) {
            cout << "No food items. Sorry:(\n";
            return;
        }
        cout << "Choose your food (enter food IDs, -1 to stop): ";
        vector<int> idlist;
        set<int> uniqueIds;
        int id;
        while (true) {
            cin >> id;
            if (id == -1) break;
            if (uniqueIds.count(id)) {
                cout << "Food ID " << id << " already selected, skipping duplicate.\n";
            } else {
                uniqueIds.insert(id);
                idlist.push_back(id);
            }
        }
        Order order(name, cid);
        bool found = false;
        for (int chosenId : idlist) {
            for (auto &f : menu.foodlist) {
                if (f.fid == chosenId) {
                    int qty;
                    cout << "Enter quantity for " << f.fname << ": ";
                    cin >> qty;
                    order.addItem(f, qty);
                    found = true;
                }
            }
        }
        if (found) {
            Order::orderlist.push_back(order);
            order.displayBill();
            order.saveBillToFile();
        } else {
            cout << "No valid food IDs were selected!\n";
        }
    }
};
int Customer::nextId = 0;
vector<Customer> Customer::customerlist;
class Manager {
private:
    Menu &menu;
public:
    Manager(Menu &m) : menu(m) {}
    void addFood() {
        int fid, fprice;
        string fname, fcat;
        cout << "Enter food id: ";
        cin >> fid;
        for (auto &f : menu.foodlist) {
            if (f.fid == fid) {
                cout << "Food ID already exists! Use a unique ID.\n";
                return;
            }
        }
        cout << "Enter food name: ";
        cin >> fname;
        cout << "Enter food price: ";
        cin >> fprice;
        cout << "Enter food category (vg/nvg): ";
        cin >> fcat;
        if (fcat == "vg") {
            menu.foodlist.push_back(VegFooditem(fid, fname, fprice));
        } else if (fcat == "nvg") {
            menu.foodlist.push_back(NonVegFooditem(fid, fname, fprice));
        } else {
            cout << "Invalid category!\n";
            return;
        }
        menu.saveToFile();
        cout << "Food item added successfully!\n";
    }
    void deleteFood() {
        int fid;
        cout << "Enter food id to delete: ";
        cin >> fid;
        for (int i = 0; i < menu.foodlist.size(); i++) {
            if (menu.foodlist[i].fid == fid) {
                menu.foodlist.erase(menu.foodlist.begin() + i);
                menu.saveToFile();
                cout << "Food item deleted successfully!\n";
                return;
            }
        }
        cout << "Food item not found!\n";
    }
    void displayMenu() {
        string fcat;
        cout << "Enter category to display (vg/nvg/all): ";
        cin >> fcat;
        menu.displayfooditem(fcat);
    }
    void displayOrders() {
        if (Order::orderlist.empty()) {
            cout << "No orders yet.\n";
            return;
        }
        int id;
        cout << "Enter Customer ID to view bill: ";
        cin >> id;
        for (auto &o : Order::orderlist) {
            if (o.cid == id) {
                o.displayBill();
                return;
            }
        }
        cout << "No order found for Customer ID " << id << "!\n";
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







