#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
using namespace std;

class Customer {
public:
    string name;
    string phno;

    void add_customer();
    void display_customers();
    void dineIn(int members);
    void takeAway();
    void homeDelivery();
};

void customer_details();  

#endif
