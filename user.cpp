#include <iostream> 
#include <string>

#include "user.h"
#include "utils.h"
#include "system.h"

using namespace std; 

// DEFAULT CONSTRUCTOR FOR USER CLASS
User::User() : username(""), user_type(""), name(""), email("") {};

// CONSTRUCTOR FOR USER CLASS
User::User(const string& username, const string& user_type, const string& name, const string& email)
    : username(username), user_type(user_type), name(name), email(email) {
        if (this->funds == 0.0) {
            this->funds = 100.00; 
        }
    }

// CONSTRUCTOR WITH PASSWORD AND USER INFORMATION
User::User(const string& username, const string& password, const string& user_type, const string& name, const string& email)
    : username(username), password(password), user_type(user_type), name(name), email(email) {
        if (this->funds == 0.0) {
            this->funds = 100.00; 
        }
    }

// GETTER METHOD FOR USERNAME
string User::get_username() const {
    return this->username;
}

// GETTER METHOD FOR PASSWORD
string User::get_password() const {
    return this->password;
}

// GETTER METHOD FOR USER TYPE
string User::get_type() const {
    return this->user_type;
}

// GETTER METHOD FOR NAME
string User::get_name() const {
    return this->name;
}

// GETTER METHOD FOR EMAIL
string User::get_email() const {
    return this->email;
}

// GETTER METHOD FOR FUNDS
double User::get_funds() const {
    return this->funds;
}

// FUNCTION TO CHECK IF USER TYPE IS VALID
bool is_not_a_valid_type(string user_type) {
    return (user_type == "R" || user_type == "N" || user_type == "O" || user_type == "C" || user_type == "X" || user_type == "F");
}

// METHOD TO UPDATE USER INFORMATION
void User::update_user_info() {
    map<string, vector<string>>& existing_info = read_in_file("userpass.txt");
    string user, pass, u_type, a_name, an_email;
    
    // PROMPT FOR NEW USERNAME
    cout << "Please enter a new username or type X to keep the same username: " << endl;
    cin >> user;

    if (user == "X") {
        set_user(this->get_username());
    } else {
        set_user(user);
    }

    // PROMPT FOR NEW PASSWORD
    cout << "Please enter a new password or type X to keep the same password: " << endl;
    cin >> pass;

    if (pass == "X") {
        set_pass(this->get_password());
    } else {
        set_pass(user);
    }

    // PROMPT FOR USER TYPE
    cout << "Please enter your user type from the options below: \n"
        << "\t(R): Resident\n" 
        << "\t(N): Nonresident\n"
        << "\t(O): Organizer\n" 
        << "\t(C): City\n" 
        << "\t(X): None of the above\n" 
        << "\t(A): To keep previous user type\n" << endl;
    cin >> u_type;

    if (u_type == "A") {
        set_utype(this->get_type());
    } else {
        while (!is_not_a_valid_type(u_type)) {
            cout << "Incorrect user type. Try again " << endl;
            cin >> u_type;
        }

        set_utype(u_type);
    }

    // PROMPT FOR NEW NAME
    cout << "Please enter a new name or type X to keep the same name: " << endl;
    getline(cin, a_name);

    if (a_name != "X") {
        set_name(a_name);
    }

    // PROMPT FOR NEW EMAIL
    cout << "Please enter a new email or type X to keep the same email: " << endl;
    cin >> an_email;

    if (a_name != "X") {
        this->email = an_email;
    }

    // UPDATE EXISTING USER INFORMATION
    existing_info[user] = {pass, u_type, a_name, an_email};

    write_to_file(existing_info);
}

bool User::operator==(const User& other) const {
    return this->username == other.username;
}