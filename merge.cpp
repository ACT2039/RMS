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
vector<Order*> Order::orderlist;

class Customer {
public:
    static int nextId;
    int cid;
    string name;
    string phno;
    int members;
    static vector<Customer*> customerlist;

    Customer() {
        cid = ++nextId;
        cout << "---~ WELCOME ~---\n";
    }

    void add_details(TableManager &tmanager) {
        cin.ignore();
        cout << "Enter your name: ";
        getline(cin, name);
        while (true) {
            cout << "Enter your phno.: ";
            cin >> phno;
            if (phno.size() == 10) break;
            cout << "Invalid phone number. Try again!\n";
        }

        
        char viewChoice;
        cout << "Do you want to view available tables before reservation? (y/n): ";
        cin >> viewChoice;
        if (viewChoice == 'y' || viewChoice == 'Y') {
            tmanager.displayAvailableTables();
        }

        cout << "Enter total members: ";
        cin >> members;

        
        tmanager.assignTable(name, members);

        customerlist.push_back(this);
        cout << "Your Customer ID is: " << cid << "\n";
    }

   void placeOrder(Menu& menu) {
    string fcat;
    cout << "Enter food category (vg/nvg/all): ";
    cin >> fcat;

    
    menu.displayfooditem(fcat);


    bool anyAvailable = false;
    for (auto &f : menu.foodlist) {
        if (fcat == "all" || f.fcat == fcat) {
            anyAvailable = true;
            break;
        }
    }

    if (!anyAvailable) {
        cout << "No food items available in this category. Returning to menu.\n";
        return;  
    }

    vector<int> idlist;
    set<int> uniqueIds;
    int id;
    cout << "Choose food IDs (-1 to stop): ";
    while (true) {
        cin >> id;
        if (id == -1) break;
        if (!uniqueIds.count(id)) {
            uniqueIds.insert(id);
            idlist.push_back(id);
        }
    }

    Order* order = new Order(name, cid);
    for (int chosenId : idlist) {
        for (auto& f : menu.foodlist) {
            if (f.fid == chosenId) {
                int qty;
                cout << "Enter quantity for " << f.fname << ": ";
                cin >> qty;
                order->addItem(f, qty);
            }
        }
    }

    Order::orderlist.push_back(order);
    order->displayBill();
    order->saveBillToFile(phno);
}

        

        

       
};
int Customer::nextId = 0;
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
    Manager manager(menu);
    TableManager tmanager(5); 

    int key;
    do {
        cout << "\n1. Manager\n2. Customer\n3. Exit\nEnter choice: ";
        cin >> key;
        if (key == 1) {
            int choice;
            do {
                cout << "\n--- Manager Menu ---\n";
                cout << "1. Add Food Item\n";
                cout << "2. Display Menu\n";
                cout << "3. View Customer History\n";
                cout << "4. View Table Status\n";
                cout << "5. View Waiting Queue\n";
                cout << "6. Exit\n";
                cout << "Enter: ";
                cin >> choice;
                if (choice == 1) manager.addFood();
                else if (choice == 2) manager.displayMenu();
                else if (choice == 3) manager.viewCustomerHistory();
                else if (choice == 4) tmanager.displayAvailableTables();
                else if (choice == 5) tmanager.displayWaitingQueue();
            } while (choice != 6);
        }
        else if (key == 2) {
            int custChoice;
            do {
                cout << "\n--- Customer Menu ---\n";
                cout << "1. New Customer Reservation\n";
                cout << "2. View Available Tables\n";
                cout << "3. Exit to Main Menu\n";
                cout << "Enter choice: ";
                cin >> custChoice;

                if (custChoice == 1) {
                    try {
                        Customer* c1 = new Customer();
                        c1->add_details(tmanager);   
                        c1->placeOrder(menu);
                        delete c1;
                    }
                    catch (bad_alloc& e) {
                        cerr << "Memory allocation failed: " << e.what() << '\n';
                    }
                }
                else if (custChoice == 2) {
                    tmanager.displayAvailableTables();
                }
            } while (custChoice != 3);
        }
    } while (key != 3);

    for (auto c : Customer::customerlist) delete c;
    for (auto o : Order::orderlist) delete o;

    cout << "Exiting program...\n";
    return 0;
}















