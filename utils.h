#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <map>
#include <queue>

#include "user.h"
#include "event.h"

using namespace std; 

// TRIMS THE \r AND \n CHARACTERS IN A STRING
string trim_r(const string &str);

// READS IN A FILE AND CREATES A MAP OF USERNAMES AND PASSWORDS
map<string, vector<string>>& read_in_file(string filepath);

// WRITES AN ENTIRE MAP OF USERNAMES AND PASSWORDS TO A FILE
void write_to_file(map<string, vector<string>>& a_map);

// UPDATES A FILE WITH NEW INFORMATION
void update_file(User u);

// FINDS THE PASSWORD OF A GIVEN USER. WILL RETURN "NULL" IF THE USER COULDN'T BE FOUND
string find_pass(string user);

// FINDS THE USERNAME ASSOCIATED WITH A GIVEN PASSWORD. WILL RETURN "NULL" IF THE GIVEN PASSWORD COULDN'T BE FOUND
string find_user(string pass);

// CHECKS TO SEE IF A USERNAME EXISTS IN THE CURRENT FILE STORING USERNAMES AND PASSWORDS
bool does_user_exist(string user);

// CHECKS TO SEE IF A GIVEN PASSWORD MATCHES THE GIVEN USERNAME
bool is_correct_pass(string user, string pass);

// PROMPTS THE EXIT MENU
void prompt_exit();

// WRITES THE EVENT TO A FILE
void write_event(Event e, string filename);

// WRITES A USER TO A FILE (CAN DO MULTIPLE, OR INDIVIDUALS)
void write_user(User u, string filename, bool individual);

// WRITES AN ITERABLE (QUEUE)
void write_iterable(queue<User> q, string filename);

// READS EVENTS FROM A FILE
vector<Event> read_event();

// READS USERS FROM A FILE INTO A VECTOR
vector<User> read_to_user(string filename);

// READS A FILE INTO A VECTOR OF QUEUES
vector<queue<User>> read_to_queue(string filename);

// SPLITS A STRING BY A GIVEN DELIMITER
vector<string> split(string str, char delimiter);

// CLEARS A FILE
void clear_file(string filename);

// READS EVENTS FROM A FILE AND CONSTRUCTS A VECTOR OF EVENT OBJECTS
vector<Event> read_events(string filename);

// WRITES A SINGLE DATETIME TO A FIXED FILE
void write_datetime(DateTime dt);

// READS ALL DATETIMES FROM FILE
vector<DateTime> read_datetimes(string filename); 

// READS ALL CANCELLED EVENTS AND CONSTRUCTS INTO VECTOR OF PAIRS
vector<pair<Event, DateTime>> read_cancelled_events(string filename);

#endif // UTILS_H