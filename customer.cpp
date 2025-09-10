#include <iostream>
#include <vector>
#include "customer.h"

using namespace std;

vector<Customer> customerlist;

void Customer::add_customer() {
    cout << "Enter your name: ";
    cin >> name;
    cout << "Enter your ph no: ";
    cin >> phno;
    customerlist.push_back(*this);
}

void Customer::display_customers() {
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
