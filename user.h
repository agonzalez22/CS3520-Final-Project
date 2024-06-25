#ifndef USER_H
#define USER_H

#include <string> 
#include <vector> 

using namespace std; 

/*
* USER CLASS DEFINITION
*/
class User {
    private: 
        string password; // PASSWORD FOR THE USER

    protected: 
        string user_type; // TYPE OF USER (RESIDENT, NON-RESIDENT, ORGANIZER, CITY, NONE)
        string email;     // EMAIL OF THE USER
        int userid;       // USER ID

    public:
        string username; // USERNAME, ALSO FUNCTIONS AS A USER ID AS THESE MUST BE UNIQUE
        string name;     // NAME OF THE USER
        double funds = 0;    // FUNDS AVAILABLE TO USER

        // DEFAULT CONSTRUCTOR FOR USER CLASS
        User();
    
        // CONSTRUCTOR FOR USER CLASS
        User(const string& username, const string& user_type, const string& name, const string& email);
        
        // CONSTRUCTOR WITH PASSWORD AND USER INFORMATION
        User(const string& username, const string& password, const string& user_type, const string& name, const string& email);

        // RETURNS THE USERNAME OF A USER
        string get_username() const;

        // RETURNS THE PASSWORD OF A USER
        string get_password() const;

        // RETURNS THE USER TYPE OF A USER: RESIDENT, NON-RESIDENT, ORGANIZER, CITY, OR NONE
        string get_type() const;

        // RETURNS THE NAME OF A USER
        string get_name() const;

        // RETURNS THE EMAIL OF A USER
        string get_email() const;

        // GETS THE FUNDS OF A USER
        double get_funds() const;

        // ALLOWS YOU TO CHANGE THE USERNAME OF A USER
        void set_user(const string& username) {
            this->username = username;
        }

        // ALLOWS YOU TO SET THE PASSWORD OF A USER
        void set_pass(const string& pword) {
            this->password = pword;
        }

        // ALLOWS YOU TO CHANGE THE USER TYPE OF A USER
        void set_utype(const string& utype) {
            this->user_type = utype;
        }

        // ALLOWS YOU TO CHANGE THE NAME OF A USER
        void set_name(const string& name) {
            this->name = name;
        }

        // ALLOWS YOU TO SET THE EMAIL ADDRESS OF A USER
        void set_mail(const string& email) {
            this->email = email;
        }

        // SUBTRACTS FROM THE FUNDS OF THE USER
        void subtract_funds(const double& price) {
            this->funds -= price;
        }

        // ISSUES A REFUND TO THE USER
        void issue_refund(const double& refund) {
            this->funds += refund;
        }

        // UPDATES USER INFO WITH INPUT GIVEN 
        void update_user_info();

        // OVERRIDES == to check if they're the same user
        bool operator==(const User& other) const;

        // DESTRUCTOR
        ~User() {}; 
};

#endif // USER_H