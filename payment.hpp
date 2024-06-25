#ifndef PAYMENT_HPP
#define PAYMENT_HPP

#include <string> 
#include <vector> 

using namespace std; 

class Payment {
    protected:
        double base;
        double hourly_rate;
    
    public:
        // constructor
        Payment() : base(10.0), hourly_rate(0.0) {}

        //returns the base cost of an event for a user
        double get_base() const { return this->base; }

        //returns the hourly rate depending on user 
        double get_hourly_rate() const { return this->hourly_rate; }

        //allows you to set the password of a user
        void set_charges(string user_type) {
            if (user_type == "R") {
                this->hourly_rate = 10.0;
            } else if (user_type == "N") {
                this->hourly_rate = 15.0;
            } else if (user_type == "O") {
                this->hourly_rate = 20.0;
            } else if (user_type == "C") {
                this->hourly_rate = 5.0;
            } else {
                this->hourly_rate = 0.0;
            }
        }

        //calculates the hourly rate price with the base
        double calc_rent_amt(int hours) { return base + (hourly_rate * hours); }

        // destructor
        ~Payment() {};
};

#endif // PAYMENT_HPP