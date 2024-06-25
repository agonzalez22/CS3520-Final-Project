#include "utils.h"
#include "user.h"
#include "event.h"
#include "datetime.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <queue>

using namespace std;

// GETS RID OF CARRIAGE RETURNS AND NEWLINE CHARACTERS IN A STRING
string trim_r(const string &str) {
    size_t first = str.find_first_not_of("\n\r");
    if (string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of("\n\r");
    return str.substr(first, (last - first + 1));
}

// READS IN A CSV FILE AND STORES THE USERNAME AND PASSWORD INFORMATION INTO A MAP
map<string, vector<string>>& read_in_file(string filepath) {
    ifstream new_file(filepath);
    static map<string, vector<string>> a_map; // CREATING EMPTY MAP
    string line;
    int count = 1;
    while (getline(new_file, line)) {
        if (count > 2) {
            stringstream infostream(line);
            vector<string> user_info;
            string user;
            string val;
            int curr_loc = 0;
            while (getline(infostream, val, ';')) {
                if (curr_loc == 0) {
                    user = trim_r(val);
                    curr_loc++;
                } else {
                    user_info.push_back(trim_r(val));
                }
            }
            a_map[user] = user_info;
        } else {
            count++;
        }
    }
    new_file.close();
    return a_map;
}

// WRITES TO FILE
void write_to_file(map<string, vector<string>>& a_map) {
    ofstream currFile("userpass2.txt");
    if (currFile.is_open()) { // CHECKS THAT THE FILE IS OPEN
        for (auto const& person : a_map) {
            currFile << person.first << ";" << person.second[0] << ";" << person.second[1] << ";" << person.second[3] << ";" << person.second[4] << "\n";
        }
        currFile.close();
    }
}

// UPDATES A FILE
void update_file(User u) {
    ofstream currFile("userpass.txt", ios::app);
    if (currFile.is_open()) { // CHECKS THAT THE FILE IS OPEN
        currFile << "\n" << u.username << ";" << u.get_password() << ";" << u.get_type() << ";" << u.get_name() << ";" << u.get_email();
        currFile.close();
    }
}

// FINDS PASSWORD
string find_pass(string user) {
    map<string, vector<string>> existing_users = read_in_file("userpass.txt");
    for (const auto& person : existing_users) {
        if (person.first == user) {
            return person.second[0];
        }
    }
    return "null";
}

// FINDS A USER WITHIN A FILE, RETURNS THAT USER
string find_user(string pass) {
    map<string, vector<string>> existing_users = read_in_file("userpass.txt");
    for (const auto& person : existing_users) {
        if (person.second[0] == pass) {
            return person.first;
        }
    }
    return "null";
}

// CHECKS TO SEE IF A USER EXISTS WITHIN A FILE, RETURNS TRUE OR FALSE
bool does_user_exist(string user) {
    map<string, vector<string>> existing_users = read_in_file("userpass.txt");
    bool user_exists = false;
    for (const auto& person : existing_users) {
        if (person.first == user) {
            user_exists = true;
        }
    }
    return user_exists;
}

// CHECKS TO SEE IF A PASSWORD MATCHES WITH A USER
bool is_correct_pass(string user, string pass) {
    map<string, vector<string>> existing_users = read_in_file("userpass.txt");
    bool correct_pass = false;
    for (const auto& person : existing_users) {
        if (person.first == user && person.second[0] == pass) {
            correct_pass = true;
        }
    }
    return correct_pass;
}

// PROMPTS THE EXIT TO MAIN MENU MESSAGE
void prompt_exit() {
    string x;
    cout << "[PRESS ANY KEY + ENTER TO RETURN TO THE MAIN MENU]";
    cin >> x;
}

// SAVE EVENTS TO FILE
void write_event(Event e, string filename) {
    ofstream curr_file(filename, ios::app); // HARDCODED TO ENSURE WE ONLY WRITE TO events_file
    string layout_str;
    switch(e.layout) {
        case MEETING:
            layout_str = "M";
            break;
        case DANCE:
            layout_str = "D";
            break;
        case LECTURE:
            layout_str = "L";
            break;
        case WEDDING:
            layout_str = "W";
            break;
    }

    // FIRST WRITE ALL THE BASIC INFORMATION
    if (curr_file.is_open()) { // CHECKS IF OPEN
        curr_file << e.event_name << ";"
                  << e.pub_event << ";"
                  << layout_str << ";"
                  << e.start_time.get_date() << ";"
                  << e.start_time.get_time() << ";"
                  << e.end_time.get_date() << ";"
                  << e.end_time.get_time() << ";"
                  << e.capacity << ";"
                  << e.approved << ";"
                  << e.ticket_price << ";"
                  << e.attendee_type << ";"
                  << e.id << ";\n";
        curr_file.close();
    }

    if (filename == "event_files/events_file.txt") {
        // NOW WRITE THE USER, THE ATTENDEES AND THE WAITLIST
        write_user(e.organizer, "event_files/event_organizers.txt", true);
        write_iterable(e.get_attendees(), "event_files/event_attendees.txt");
        write_iterable(e.get_waitlist(), "event_files/event_waitlist.txt");
    } else {
        write_user(e.organizer, "event_files/cancelled_users.txt", true); 
        write_iterable(e.get_attendees(), "event_files/cancelled_attendees.txt");
        write_iterable(e.get_waitlist(), "event_files/cancelled_waitlist.txt");
    }
    
}

// CAN WRITE MULTIPLE OR INDIVIDUAL USERS TO A FILE
void write_user(User u, string filename, bool individual) {
    ofstream file(filename, ios::app);
    file << u.username << ","
         << u.get_password() << ","
         << u.get_type() << ","
         << u.name << ","
         << u.get_email() << "," 
         << u.funds;
    if (individual) {
        file << "\n";
    } else {
        file << ";"; // DELIMIT BY ; IF NOT INDIVIDUAL USER
    }
    file.close();
}

// WRITES A QUEUE OR VECTOR TO A FILE
void write_iterable(queue<User> q, string filename) {
    queue<User> temp_q = q;
    // LOOP THROUGH TEMP QUEUE AND WRITE TO FILE
    while(!temp_q.empty()) {
        write_user(temp_q.front(), filename, false);
        temp_q.pop();
    }
    ofstream file(filename, ios::app);
    file << "\n";
    file.close();
}

// CLEAR FILE (IMPORTANT)
void clear_file(string filename) {
    ofstream file(filename, ios::trunc);
    file.close();
}

// SPLITS A STRING BY A GIVEN DELIMITER
vector<string> split(string str, char delimiter) {
    vector<string> items;
    stringstream ss(str);
    string i;
    while (getline(ss, i, delimiter)) {
        items.push_back(i);
    }
    
    return items;
}

// READS A FILE INTO A VECTOR OF QUEUES
vector<queue<User>> read_to_queue(string filename) {
    ifstream file(filename);
    vector<queue<User>> vq;
    string line;
    while(getline(file, line)) {
        vector<string> users = split(line, ';');
        queue<User> q;
        for (string u : users) {
            vector<string> curr = split(u, ',');
            User new_u(curr[0], curr[2], curr[3], curr[4]);
            new_u.set_pass(curr[1]); 
            new_u.funds = stod(curr[5]); // SET FUNDS
            q.push(new_u); // ADD USER TO QUEUE
        }
        vq.push_back(q);
    }
    file.close();
    return vq;
}

// READS A FILE AND RETURNS LIST OF USERS FROM THAT FILE
vector<User> read_to_user(string filename) {
    ifstream file(filename);
    vector<User> uv;
    string line;
    while(getline(file, line)) {
        vector<string> user = split(line, ',');
        User u(user[0], user[2], user[3], user[4]);
        u.set_pass(user[1]);
        u.funds = stod(user[5]);
        uv.push_back(u);
    }
    file.close();
    return uv;
}

// READS FROM A FILE AND CONSTRUCTS A VECTOR OF EVENT OBJECTS
vector<Event> read_events(string filename) {
    ifstream file(filename);
    string line;
    vector<Event> ev;
    vector<User> uv; 
    vector<queue<User>> qw, qa; 
    int i = 0;
    if (filename == "event_files/events_file.txt"){
        // ORGANIZERS, ATTENDEES, AND WAITLIST EXTRACTION
        uv = read_to_user("event_files/event_organizers.txt");
        qa = read_to_queue("event_files/event_attendees.txt");
        qw = read_to_queue("event_files/event_waitlist.txt");
    
    } else {
        // ORGANIZERS, ATTENDEES, AND WAITLIST EXTRACTION FOR CANCELLED LIST
        uv = read_to_user("event_files/cancelled_users.txt");
        qa = read_to_queue("event_files/cancelled_attendees.txt");
        qw = read_to_queue("event_files/cancelled_waitlist.txt");
        
    }
    
    // CONSTRUCT EACH EVENT AND ADD TO VECTOR
    while(getline(file, line) && i < uv.size()) {
        vector<string> v = split(line, ';');
        LayoutStyle l;
        if(v[2] == "M") {
            l = MEETING;
        } else if(v[2] == "W") {
            l = WEDDING;
        } else if(v[2] == "L") {
            l = LECTURE;
        } else if(v[2] == "D") {
            l = DANCE;
        }

        string ev_name = v[0];
        User u = uv[i];
        bool pub = bool(stoi(v[1]));
        int capacity = stoi(v[7]);
        double price = stod(v[9]);
        string a_type = v[10];
        int e_id = stoi(v[11]);
        DateTime start = splice_date_time(v[3], v[4]);
        DateTime end = splice_date_time(v[5], v[6]);
        Event e(ev_name, u, pub, l, start, end, capacity, price, a_type);
        e.approved = bool(stoi(v[8])); // ADD APPROVED BOOL
        e.id = e_id; 

        e.set_attendees(qa[i]); 
        e.set_waitlist(qw[i]); 

        ev.push_back(e);
        i++;
        if (i >= uv.size()) {
            break;
        }
    }
    
    file.close();
    return ev;
}

// WRITES A SINGLE DATETIME TO A FILE
void write_datetime(DateTime dt) {
    ofstream file("event_files/cancelled_datetimes.txt", ios::app); 
    file << dt.year << ";"
         << dt.month << ";"
         << dt.day << ";"
         << dt.hour << ";"
         << dt.minute << "\n"; 
    
    file.close(); 
}

vector<DateTime> read_datetimes(string filename) {
    ifstream file(filename);
    vector<DateTime> dts;
    string line;
    while (getline(file, line)) {
        vector<string> dt = split(line, ';');
        dts.push_back(DateTime(stoi(dt[0]), stoi(dt[1]), stoi(dt[2]), stoi(dt[3]), stoi(dt[4]))); 
    }

    file.close();
    return dts;
}

// reads and writes 
vector<pair<Event, DateTime>> read_cancelled_events(string filename) {
    vector<Event> events = read_events("event_files/cancelled_events.txt");
    vector<DateTime> dts = read_datetimes("event_files/cancelled_datetimes.txt");  
    vector<pair<Event, DateTime>> cancellations = {};

    if (events.size() == dts.size()) {
        for (int i = 0; i < events.size(); i ++) {
            cancellations.push_back(make_pair(events[i], dts[i])); 
        }
    }   

    return cancellations;
}