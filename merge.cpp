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


    string formatPrice(int price) const {
        
        return to_string(price) + string(".00");
    }


    string padRight(const string &s, size_t width) const {
        if (s.length() >= width) return s.substr(0, width);
        return s + string(width - s.length(), ' ');
    }
    string padLeft(const string &s, size_t width) const {
        if (s.length() >= width) return s.substr(0, width);
        return string(width - s.length(), ' ') + s;
    }

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
        if (items.empty()) {
            cout << "\nNo items ordered yet!\n";
            return;
        }

    
        size_t nameWidth = 20; 
        for (const auto& p : items) {
            size_t needed = p.first.getName().length() + 2;
            if (needed > nameWidth) nameWidth = needed;
        }
        if (nameWidth > 35) nameWidth = 35; 

        const size_t qtyWidth = 6;
        const size_t priceWidth = 10;
        const size_t lineTotalWidth = 12;
        const size_t gap = 2; 

        size_t tableWidth = nameWidth + gap + qtyWidth + gap + priceWidth + gap + lineTotalWidth;

        
        cout << "\n" << string(tableWidth, '=') << "\n";
        cout << padRight("BILL SUMMARY", tableWidth) << "\n";
        cout << string(tableWidth, '=') << "\n";
        cout << "Customer Name: " << customerName << "\n";
        cout << string(tableWidth, '-') << "\n";

        string h_item = padRight("Item", nameWidth);
        string h_qty  = padRight("Qty", qtyWidth);
        string h_price = padRight("Price", priceWidth);
        string h_total = padRight("Total", lineTotalWidth);

        cout << h_item << string(gap, ' ')
             << h_qty  << string(gap, ' ')
             << h_price << string(gap, ' ')
             << h_total << "\n";

        cout << string(tableWidth, '-') << "\n";

        
        for (const auto& p : items) {
            const string &iname = p.first.getName();
            string s_name = padRight(iname, nameWidth);

            string s_qty = padRight(to_string(p.second), qtyWidth);

            string s_price = padRight(formatPrice(p.first.getPrice()), priceWidth);

            int lineTotal = p.first.getPrice() * p.second;
            string s_lineTotal = padLeft(formatPrice(lineTotal), lineTotalWidth);

            cout << s_name << string(gap, ' ')
                 << s_qty  << string(gap, ' ')
                 << s_price << string(gap, ' ')
                 << s_lineTotal << "\n";
        }

        cout << string(tableWidth, '-') << "\n";

        
        string totalLabel = "Total Amount:";
        string totalValue = formatPrice(getTotal());
        string summaryLine = padLeft(totalLabel, tableWidth - totalValue.length()) + totalValue;
        cout << summaryLine << "\n";

        cout << string(tableWidth, '=') << "\n";
        cout << padRight("Thank you for dining with us!", tableWidth) << "\n";
        cout << string(tableWidth, '=') << "\n\n";
    }

    void saveBillToFile(int custId, const string& custPhone) const {
        try {
            ofstream file("customer_history.txt", ios::app);
            if (!file) throw "Unable to open customer history file.";
            file << "Customer ID: " << custId
                 << " | Name: " << customerName
                 << " | Phone: " << custPhone << "\n";
            for (const auto& p : items) {
                file << "  - Item: " << p.first.getName()
                     << " | Qty: " << p.second
                     << " | Price: " << p.first.getPrice() << "\n";
            }
            file << "Total Bill: " << getTotal() << "\n";
            file << string(40, '-') << "\n";
        } catch (const char* msg) {
            cerr << "Error: " << msg << endl;
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
        try {
            cout << "Enter your name: ";
            getline(cin, name);
            cout << "Enter your 10-digit phone number: ";
            getline(cin, phone);
            cout << "Enter total number of members: ";
            cin >> members;
            cin.ignore();
            if (phone.length() != 10) throw "Invalid phone number.";
            if (members <= 0) throw "Invalid number of members.";
            tManager.assignTable(name, members);
        } catch (const char* msg) {
            cerr << "Error: " << msg << endl;
        }
    }

    void placeOrder(Menu& menu) {
        try {
            menu.displayMenu();
            Order order(name);
            while (true) {
                cout << "Enter Food ID to order (-1 to finish): ";
                int id;
                cin >> id;
                if (id == -1) break;
                Fooditem* item = menu.findFoodById(id);
                if (!item) throw "Invalid Food ID entered.";
                cout << "Enter quantity for " << item->getName() << ": ";
                int qty;
                cin >> qty;
                if (qty <= 0) throw "Quantity must be positive.";
                order.addItem(*item, qty);
            }
            cin.ignore();
            order.printBill();
            order.saveBillToFile(customerId, phone);
        } catch (const char* msg) {
            cerr << "Error: " << msg << endl;
        }
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
        try {
            cout << "Enter food id: ";
            int id;
            cin >> id;
            if (menu.findFoodById(id) != nullptr) throw "Food ID already exists.";
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
            if (price <= 0) throw "Invalid price entered.";
            if (catChoice == 1) menu.addFoodItem(new VegFooditem(id, name, price));
            else if (catChoice == 2) menu.addFoodItem(new NonVegFooditem(id, name, price));
            else throw "Invalid category choice.";
            menu.saveToFile();
            cout << "Food item added successfully!\n";
        } catch (const char* msg) {
            cerr << "Error: " << msg << endl;
        }
    }
    
    void viewCustomerHistory() {
        try {
            ifstream file("customer_history.txt");
            if (!file) throw "Customer history file not found.";
            cout << "\n--- Customer Order History ---\n";
            string line;
            while (getline(file, line)) {
                cout << line << endl;
            }
            cout << "------------------------------\n";
        } catch (const char* msg) {
            cerr << "Error: " << msg << endl;
        }
    }

    void viewMenu() { 
        menu.displayMenu(); }
    void viewTables() { 
        tableManager.displayAvailableTables(); }
    void viewWaitingList() {
        tableManager.displayWaitingQueue(); }
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
            authenticateManager();
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
