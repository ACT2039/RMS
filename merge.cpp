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

class TableManager {
private:
    vector<Table> tables;
    vector<string> waitingQueue;

public:
    TableManager(int totalTables = 5) {
        for (int i = 1; i <= totalTables; i++) {
            tables.push_back(Table(i));
        }
    }

    void displayAvailableTables() const {
        cout << "\n=== Available Tables ===\n";
        for (const auto &t : tables) {
            cout << "Table " << t.getId() << " | Seats Left: " << t.getSeatsLeft() << "\n";
        }
    }

    void assignTable(const string& customerName, int members) {
        int remainingMembers = members;
        
        for (auto &t : tables) {
            if (remainingMembers == 0) break;

            int seatsAvailable = t.getSeatsLeft();
            if (seatsAvailable > 0) {
                int membersToSeat = min(remainingMembers, seatsAvailable);
                t.reserveSeats(membersToSeat);
                cout << membersToSeat << " member(s) of " << customerName 
                     << "'s group assigned to Table " << t.getId() << ".\n";
                remainingMembers -= membersToSeat;
            }
        }
        
        if (remainingMembers > 0) {
            cout << "Sorry, not enough total seats available. Adding " << remainingMembers 
                 << " remaining member(s) to the waiting queue.\n";
            waitingQueue.push_back(customerName + " (" + to_string(remainingMembers) + " members)");
        }
    }

    void displayWaitingQueue() const {
        if (waitingQueue.empty()) {
            cout << "The waiting queue is empty.\n";
        } else {
            cout << "\n=== Waiting Queue ===\n";
            for (const auto &entry : waitingQueue) {
                cout << "- " << entry << "\n";
            }
        }
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
private:
    string customerName;
    vector<pair<Fooditem, int>> items;

public:
    Order(string cname) : customerName(cname) {}

    void addItem(const Fooditem& item, int quantity) {
        items.push_back({item, quantity});
    }

    int getTotal() const {
        int total = 0;
        for (const auto& p : items) {
            total += p.first.getPrice() * p.second;
        }
        return total;
    }

    void printBill() const {
        cout << "\n------ BILL for " << customerName << " ------\n";
        for (const auto& p : items) {
            int itemTotal = p.first.getPrice() * p.second;
            cout << " - " << p.first.getName() << " x " << p.second << " = " << itemTotal << "\n";
        }
        cout << "-------------------------\n";
        cout << "Total Amount: " << getTotal() << "\n";
        cout << "-------------------------\n";
    }

    void saveBillToFile(int custId, const string& custPhone) const {
        ofstream file("customer_history.txt", ios::app);
        if (file.is_open()) {
            file << "Customer ID: " << custId << " | Name: " << customerName << " | Phone: " << custPhone << "\n";
            for (const auto& p : items) {
                file << "  - Item: " << p.first.getName() << " | Qty: " << p.second 
                     << " | Price: " << p.first.getPrice() << "\n";
            }
            file << "Total Bill: " << getTotal() << "\n";
            file << "-------------------------------------------\n";
        }
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
    TableManager& tableManager;

public:
    Manager(Menu& m, TableManager& tm) : menu(m), tableManager(tm) {}

    void addFoodItem() {
        cout << "Enter food id: ";
        int id;
        cin >> id;
        if (menu.findFoodById(id) != nullptr) {
            cout << "Food ID already exists!\n";
            cin.ignore();
            return;
        }
        
        cout << "Enter food name: ";
        string name;
        cin.ignore();
        getline(cin, name);

        cout << "Enter food price: ";
        int price;
        cin >> price;

        cout << "Enter category (1 for Veg, 2 for Non-Veg): ";
        int catChoice;
        cin >> catChoice;
        cin.ignore();

        if (catChoice == 1) {
            menu.addFoodItem(new VegFooditem(id, name, price));
        } else if (catChoice == 2) {
            menu.addFoodItem(new NonVegFooditem(id, name, price));
        } else {
            cout << "Invalid category!\n";
            return;
        }

        menu.saveToFile();
        cout << "Food item added successfully!\n";
    }
    
    void viewCustomerHistory() {
        ifstream file("customer_history.txt");
        if (!file.is_open()) {
            cout << "No customer history found.\n";
            return;
        }
        cout << "\n--- Customer Order History ---\n";
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        cout << "------------------------------\n";
    }

    void viewMenu() {
        menu.displayMenu();
    }
    
    void viewTables() {
        tableManager.displayAvailableTables();
    }
    
    void viewWaitingList() {
        tableManager.displayWaitingQueue();
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



















