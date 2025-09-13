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
