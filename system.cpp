/* Alina Gonzalez & Khushi Khan 
* CS3520 - 6/24/25
* System Class Methods and supplementary methods associated 
* with System class functionality 
*/
#include "system.h"
#include "event.h"
#include "user.h"
#include "datetime.h"
#include "payment.hpp"

#include <algorithm>
#include <string> 
#include <iostream> 
#include <vector>

using namespace std; 

// DEFAULT CONSTRUCTOR FOR SYSTEM CLASS
System::System() : events({}), funds(1000.00){
    this->events = read_events("event_files/events_file.txt"); 
    this->cancelled_events = read_cancelled_events("event_files/cancelled_events.txt"); 
}

// CONSTRUCTOR METHOD - SYSTEM CLASS TAKES A VECTOR OF EVENTS
System::System(vector<Event> events, double funds) : events(events), funds(funds){}

// DISPLAY ALL EVENTS FOR A GIVEN USER
void System::display_events(DateTime curr_date, User u) {
    int curr_day = curr_date.day; 
    int week_day = curr_day + 7;
    for (Event e : events) {
        if (e.approved && curr_date.day <= e.start_time.day && e.start_time.day < week_day){ // DISPLAY ONLY APPROVED EVENTS 
            if (e.pub_event){ // DISPLAY ONLY PUBLIC EVENTS 
                if ((e.attendee_type == "R" && u.get_type() == "R") || e.attendee_type == "N") { //DISPLAY EVENTS BASED ON USER TYPE
                    e.display_event();
                    cout << endl;
                } 
            }
        } 
    }
}

// METHOD - DISPLAY TODAYS EVENT FOR A GIVEN USER
void System::display_todays_events(DateTime curr_date, User u) {
    for (Event e : events) {
        if (e.start_time == curr_date) { // CHECK IF EVENTS STARTS TODAY
            if (e.approved) { // DISPLAY ONLY APPROVED EVENTS
                if (e.pub_event){ // DISPLAY ONLY PUBLIC EVENTS
                    if ((e.attendee_type == "R" && u.get_type() == "R") || e.attendee_type == "N") { // DISPLAY EVENTS BASED ON USER TYPE
                        e.display_event(); 
                        cout << endl;
                    }
                }
            }
        }
    }
}

// METHOD TO RUN SYSTEM, ONLY END IF USER CHOOSES TO QUIT
void System::run() {
    User u = this->prompt_login(); // PROMPT USER LOGIN
    DateTime curr_time; // INITIALIZE CURRENT TIME
    curr_time.get_current(); // GET CURRENT TIME
    bool status = true; // RUN STATUS
    string t = u.get_type(); // GET USER TYPE 


    while (status) {
        this->events = read_events("event_files/events_file.txt");
        this->cancelled_events = read_cancelled_events("event_files/cancelled_events.txt");  
        int choice = display_menu(u.get_type()); // DISPLAY MENU AND GET USER CHOICE
        switch (choice) {
            case 0: // ADMIN 
                if (u.get_type() == "F"){ // CHECK IF USER IS FACILITY MANAGER
                    int option = 3;
                    while (option != 1 && option != 2) {
                        cout << "Would you like to: " << endl
                            << "\t(1) Approve pending events" << endl
                            << "\t(2) Approve cancellation requests" << endl;
                        cin >> option;
                    }

                    if (option == 1) {
                        this->approve_pending_events(); 
                    } else {
                        this->approve_cancellation_reqs();
                    }
                } else {
                    cout << "You are not permitted to access that page!" << endl; 
                }
                break; 
            
            case 1: // PLAN AND REQUEST EVENT
                if (t == "O" || t == "C") {
                    if(!this->is_event_limit(curr_time, u)){ // IF MAX EVENTS NOT FILLED FOR SPECIFIC USER TYPE 
                        this->request_event(u); 
                    }
                } else {
                    cout << "You are not permitted to request events! Please try another option." << endl; 
                }

                break; 
                
            case 2: // VIEW TODAY'S EVENTS
                this->display_todays_events(curr_time, u); 
                prompt_exit(); 
                break; 

            case 3: // VIEW ALL EVENTS
                this->display_events(curr_time, u); 
                prompt_exit();
                break; 

            case 4: // MAKE PAYMENT/BUY/CANCEL TICKET
                if (t == "O" || t == "C") {
                    this->handle_event_payments(curr_time, u);
                } else if (t == "R" || t == "N") {
                    this->handle_tickets(curr_time, u);
                } else {
                    cout << "You are not an organizer, resident, or non-resident! Please try another option." << endl;
                }
                prompt_exit();
                break;
                
            case 5: // UPDATE USER INFO
                u.update_user_info();
                break;

            case 6: // LOG OUT 
                cout << "\nGoodbye " << u.name << "!\n" << endl; 
                u = this->prompt_login(); // RESET U 
                t = u.get_type(); // RESET TYPE
                break;

            case 7: // QUIT PROGRAM
                status = false; 
                cout << "\nGoodbye " << u.name << "!" << endl;
                break; 
        }
        this->save(); 
    }
    
}

// SAVES THE STATE OF THE SYSTEM 
void System::save() {
    // CLEAR ALL FILES 
    clear_file("event_files/event_attendees.txt");
    clear_file("event_files/event_organizers.txt");
    clear_file("event_files/event_waitlist.txt");
    clear_file("event_files/events_file.txt");

    // REWRITE ALL FILES 
    for(Event e : this->events) {
        write_event(e, "event_files/events_file.txt"); 
    }
}

// METHOD - PROMPT USER LOGIN
User System::prompt_login() {
    // INTIT USER INFORMATION VARIABLES
    string name, user, pass, user_type, email;

    // READ EXISTING USERS FROM FILE
    map<string, vector<string>> existing_users = read_in_file("userpass.txt");

    // WELCOME MESSAGE
    cout << "Welcome to Newton Events" << endl;
    cout << "Please enter your username. If you have an existing login, the system will automatically log you in. If not, the system will create a new user for you." << endl;
    cin >> user;

    // CHECK IF USER EXISTS
    bool user_exists = does_user_exist(user);

    // CHECK IF USER EXISTS 
    if (user_exists) {
        // IF USER EXISTS ASK FOR PASSWORD
        while (true) {
            cout << "Please enter your password: " << endl;
            cin >> pass;
            if (is_correct_pass(user, pass)) { // IF CORRECT CONTINUE
                break;
            } else { // IF INCORRECT TRY AGAIN
                cout << "Incorrect password. Please try again." << endl;
            }
        }
        // REASSIGN READ FILE VARIABLES   
        vector<string> u_info = existing_users[user];

        // WELCOME BACK STATEMENT WITH USER'S NAME
        cout << "\nWelcome back " << u_info[2] << "!!!\n" << endl;
        
        User existing_u(user, u_info[1], u_info[2], u_info[3]);
        existing_u.set_pass(u_info[0]);

        return existing_u;
    } else {
        // IF USER DOESN'T EXISTS, CREATE NEW REGISTRATION
        // REGISTER USER FULL NAME
        cout << "We can't seem to find you in the system. Please enter your full name for your new account: " << endl;
        cin.ignore();
        getline(cin, name);
        
        // REGISTER USER PASSWORD
        cout << "Please enter your new password: " << endl;
        cin >> pass;

        cout << "Your new password is: " << pass << endl;

        // REGISTER/PROMPT FOR USER TYPE
        cout << "Please enter your user type from the options below: \n"
             << "\t(R): Resident\n"
             << "\t(N): Nonresident\n"
             << "\t(O): Organization\n"
             << "\t(C): City\n"
             << "\t(F): Facility Manager\n"
             << "\t(X): None of the above\n" << endl;
        cin >> user_type;

        // VALIDATE USER TYPE INPUT
        while (!is_not_a_valid_type(user_type)) {
            cout << "Incorrect user type. Try again " << endl;
            cin >> user_type;
        }

        //REGISTER USER EMAIL
        cout << "Please enter your email: " << endl;
        cin >> email;

        // CONSTRUCT NEW USER
        User new_u(user, user_type, name, email);
        new_u.set_pass(pass);

        update_file(new_u); 

        return new_u; 
    }
}

// METHOD - REQUEST EVENT IN SYSTEM
void System::request_event(User u) {
    string event_name, layout_str, a_type; 
    bool pub_event; 
    LayoutStyle l; 
    DateTime start, end; 
    int capacity, attendee_num, pub_int; 
    double ticket_price; 
    
    cout << "Please provide all the necessary information to get your event approved." << endl; 
    cout << "NOTES:" << endl
         << "\t- Your user will be listed as the event's organizer" << endl 
         << "\t- City gets priority over citizens for this facility!" << endl
         << endl; 

    cout << "Event name: ";
    cin.ignore();
    getline(cin, event_name);
    cout << endl; 

    cout << "Will this event be open to (1) Residents only or (2) Non-Residents may attend? "; 
    cin >> attendee_num;  
    if (attendee_num == 1) {a_type = "R";}
    else {a_type = "N"; }
    cout << endl; 

    cout << "Is this a (1) PUBLIC event or (2) PRIVATE event? Note that private events will NOT have ticket sales. "; 
    cin >> pub_int; 
    cout << endl; 
    if (pub_int == 1) {pub_event=true;} else {pub_event=false;}

    cout << "What is the event capacity? Max is 40. "; 
    cin >> capacity; 
    cout << endl; 
    if (capacity > 40) { capacity= 40;}

    bool time_conflict = true; 
    while (time_conflict) {
        vector<DateTime> start_end = prompt_for_dates();
        time_conflict = this->is_time_conflict(start_end); 
        if (time_conflict) {
            cout << "That time slot is not available! Please select another." << endl; 
        } else {
            start = start_end[0]; 
            end = start_end[1];
        }
    }

    cout << "What is the layout style?" << endl
         << "\t(M) Meeting" << endl 
         << "\t(L) Lecture" << endl 
         << "\t(D) Dance" << endl; 
    
    // WEDDING STYLE NOT AVAILABLE TO CITY OR ORAGNIZATIONS
    if (u.get_type() != "C" && u.get_type() != "O") {
        cout << "\t(W) Wedding" << endl;
    }
    
    cout << "Enter your choice: "; 
    cin >> layout_str; 
    cout << endl; 

    if (layout_str == "M") {l = MEETING;} 
    else if (layout_str == "L") {l = LECTURE;}
    else if (layout_str == "W") {l = WEDDING;}
    else if (layout_str == "D") {l = DANCE;}

    // 
    cout << "What will your ticket pricing be? If the event is private, list 0: "; 
    cin >> ticket_price; 
    cout << endl; 

    Event e(event_name, u, pub_event, l, start, end, capacity, ticket_price, a_type); 
    e.approved = false; // DEFAULT APPROVAL SHOULD BE FALSE

    this->events.push_back(e); 
}

// METHOD TO CHECK IF THERE IS A TIME CONFLICT BETWEEN A GIVEN DATE RANGE AND EXISTING EVENTS
bool System::is_time_conflict(vector<DateTime> d1) {
    DateTime start = d1[0], end = d1[1]; 
    for (Event e : this->events) {
        // FIRST CHECK IF THEY ARE APPROVED 
        if (e.approved) {
            // SECOND CHECK IF THEY ARE ON THE SAME DAY
            if (e.start_time == start && end == e.end_time) {
                // THIRD CHECK IF THEY ARE WITHIN THE SAME TIMES
                if(start < e.start_time && end > e.start_time) {
                    return true; 
                } else if (start > e.start_time && start < e.end_time) {
                    return true; 
                } else if (end > e.start_time && end < e.end_time) {
                    return true; 
                }
            }
        }
    }
    return false; // NO CONFLICT
}

// METHOD TO ALLOW FACILITY MANAGER TO APPROVE PENDING EVENT REQUESTS
void System::approve_pending_events() {
    // FIRST DISPLAY CITY-HOSTED EVENTS IN FIRST COME FIRST SERVE ORDER!!!
    for (Event& e : this->events) { // hope this reference works ;w;
        string answer; 
        if (!e.approved && e.organizer.get_type() == "C"){ // DISPLAY PENDING EVENTS ONLY
            vector<DateTime> d = {e.start_time, e.end_time};
            if (!is_time_conflict(d)){ // CHECK FOR TIME CONFLICT
                e.display_event(); 
                cout << endl; 
                
                cout << "Approve? [y/n]: ";
                cin >> answer; 

                if (answer == "y" || answer == "Y") {
                    e.approved = true; // UPDATE TO APPROVED STATUS
                }
            }
        }
    }

    // SECOND DISPLAY NON-CITY HOSTED EVENTS & APPROVE, ALSO FIRST COME FIRST SERVE ORDER 
    for (Event& e : this->events) { 
        string answer; 
        if (!e.approved){ // DISPLAY PENDING EVENTS ONLY
            vector<DateTime> d = {e.start_time, e.end_time};
            if (!is_time_conflict(d)){ // CHECK FOR TIME CONFLICT
                e.display_event(); 
                cout << endl; 
                
                cout << "Approve? [y/n]: ";
                cin >> answer; 

                if (answer == "y" || answer == "Y") {
                    e.approved = true; // UPDATE TO APPROVED STATUS
                }
            }
        }
    }
}

// COUNTS EVENT HOURS WITHIN THE WEEK, SEES IF WE HIT THE MAX LIMIT
bool System::is_event_limit(DateTime curr_date, User u) {
    int week_day = curr_date.day + 7; // GET THE DATE A WEEK FROM CURRENT DAY 
    double total_hours = 0; 
    for (Event e : this->events) {
        if (e.approved && e.organizer.get_type() == u.get_type() && e.start_time.day < week_day && e.start_time.month == curr_date.month && curr_date.year == e.start_time.year) {
            total_hours += e.get_duration(); 
        }
    }

    // CHECK TO SEE BASED ON THE EVENT ORGANIZER TYPE WHAT THE MAX LIMIT IS AND IF THE SYSTEM HAS REACHED IT FOR THE WEEK.
    if (u.get_type() == "C") {
        return total_hours > 48.0; 
    } else if (u.get_type() == "O") {
        return total_hours > 36.0; 
    } else if (u.get_type() == "R") {
        return total_hours > 24.0; 
    }
    return false; 
}

// GETTER METHOD FOR FUNDS ATTRIBUTE
double System::get_funds() {
    return this->funds; 
}

// SETTER METHOD FOR FUNDS ATTRIBUTE 
void System::subtract_funds(double new_funds) {
    this->funds = new_funds; 
}

// METHOD - DISPLAY MENU AND GET USER INPUT
int display_menu(string u_type) {
    int user_choice; 
 
    if (u_type == "F") { // DISPLAY THIS MENU OPTION IF FACILITY MANAGER
        cout << "\t(0) Approve pending events/cancellatio" << endl; 
    }
    cout << "\t(1) Plan & Request an event" << endl
         << "\t(2) View Today's Schedule" << endl 
         << "\t(3) View All Events" << endl 
         << "\t(4) Make Payment or Buy/Cancel Ticket" << endl 
         << "\t(5) Update user information" << endl
         << "\t(6) Log out" << endl
         << "\t(7) Quit" << endl
         << endl
         << "Enter your choice (numbers only!): "; 

    cin >> user_choice; // GET USER CHOICE

    return user_choice; 
}

// RUNS EVEN PAYMENT OPTION
void System::handle_event_payments(DateTime curr_date, User u) {
    /*TODO
    - cost of reserving an event (service = 10, res = 10, nr = 15, org = 20, city = 5)
    - requests confirmed only when full payment is made
    - event cancelled?
        - 1 week before? NO PENALTY
        - within 7 days? 1% penalty of rent amount
        - within 24 hrs? NO REFUND
        - service charge NON-REFUNDABLE
    */

    int choice = 3;
    while (choice != 1 && choice != 2) {
        cout << "Please enter one of the two numbers below. Would you like to: " << endl
        << "\t(1) Make a payment" << endl
        << "\t(2) Cancel an event" << endl
        << "\t\tCancellation policy: Service charges ($10) are non-refundable." << endl
        << "\t\tIf the event is canceled 1 week before, no penalty will be incurred." << endl
        << "\t\tIf the event is cancelled within the 7 day period before an event,\n" << 
            "there will be a penalty of 1% your rent amount." << endl
        << "\t\tIf the event is cancelled within 24 hours of the event, no refund will be issued." << endl;
        cin >> choice;
    }

    cout << "Here are your events: " << endl;

    vector<Event> users_events = {};
    for (Event& e : this->events) {

        if (e.start_time.operator>=(curr_date)) { // **
            if (e.approved) { // ONLY PAY IF EVENT IS GOOD TO GO!
                if (e.organizer == u) { // CHECKS IF 
                    users_events.push_back(e);
                    e.display_event();
                    cout << endl;
                }
            }
        }
    }

    if (choice == 1) {
        make_payment_option(users_events, u);
    } else {
        cancel_event_option(curr_date, users_events, u);
    }
}

// RUNS THE MAKE PAYMENT OPTION
void System::make_payment_option(vector<Event>& events, User u) {
    int id;
    cout << "Please enter the id of the event you'd like to make a payment for: " << endl;
    cin >> id;

    while (!contains_event(events, id)) {
        cout << "We were unable to find that event. Please try again: " << endl;
        cin >> id;
    }

    Event eve{};
    for (Event& event : events) {
        if (event.id == id) {
            eve = event;
            break;
        }
    }

    Payment payment;

    cout << "Press any key to confirm you'd like to make a payment for: " << eve.event_name << endl;

    payment.set_charges(u.get_type());
    double event_cost = payment.calc_rent_amt(eve.get_duration());
    u.subtract_funds(event_cost);
    eve.set_status(true);

    cout << "Payment processed. You have $" << u.funds << " remaining in your account" << endl;
}

// CHECKS TO SEE IF HAS EVENT ID
bool System::contains_event(vector<Event>& events, int id) {
    bool event_found = false;
    for (Event e : events) {
        int curr_id = e.id;   
        if (curr_id == id) {
            event_found = true;  
        } 
    }

    return event_found;
}

// CANCEL EVENTS 
void System::cancel_event_option(DateTime curr_date, vector<Event> events, User u) {
    int id;
    cout << "Please enter the id of the event you'd like to cancel: " << endl;
    cin >> id;

    while (!contains_event(events, id)) {
        cout << "We were unable to find that event. Please try again: " << endl;
        cin >> id;
    }

    Event eve{};
    for (Event& event : events) {
        if (event.id == id) {
            eve = event;
        }
    }

    cout << "Press any key to confirm you'd like to cancel: " << eve.event_name << endl;

    cancelled_events.push_back(make_pair(eve, curr_date));
    write_event(eve, "event_files/cancelled_events.txt");
    write_datetime(curr_date); 
    cout << "The Facility Manager will review your cancellation request for: " << eve.event_name << endl;
}

// APPROVE CANCELLATIONS
void System::approve_cancellation_reqs() {
    cout << "Here are the events people requested to cancel:" << endl;
    
    for (auto pr : this->cancelled_events) {
        pr.first.display_event();
    }

    cout << "Press any key to allow all events to be cancelled" << endl;

    // ISSUES (POSSIBLE) REFUNDS TO ORGANIZER
    for (Event& e : events) {
        for (auto pr : this->cancelled_events) {
            if (e == pr.first) { 
                double refund = calc_price(e, pr.second);

                e.get_organizer().issue_refund(refund);
            }
        }
    }
    
    vector <Event> new_events; 
    for (Event& e : events) {
        bool is_cancelled = false; 
        for (auto pr : this->cancelled_events) {
            if (e == pr.first) {
                queue<User> tempqueue = e.get_attendees();

                // ISSUES REFUNDS TO ALL ATTENDEES
                while (!tempqueue.empty()) {
                    User u = e.get_attendees().front();
                    e.get_attendees().pop();
                    tempqueue.pop();

                    u.issue_refund(e.ticket_price);
                    
                    e.get_attendees().push(u);
                }

                // events.erase(remove(events.begin(), events.end(), e), events.end());
                is_cancelled = true; 
            } 
        }
        if(!is_cancelled) {
            new_events.push_back(e);
        }
    }

    // REASSIGNS THE EVENTS TO THE NON-CANCELLED EVENTS
    this->events = new_events;

    cout << "\nAll your events were cancelled." << endl;
    prompt_exit();

    // erase the cancellation files
    clear_file("event_files/cancelled_attendees.txt"); 
    clear_file("event_files/cancelled_datetimes.txt");
    clear_file("event_files/cancelled_events.txt");
    clear_file("event_files/cancelled_users.txt"); 
    clear_file("event_files/cancelled_waitlist.txt"); 
    this->cancelled_events = {};  

    this->save(); 
}

// CALCULATES THE AMOUNTS FOR REFUNDS CANCELLATIONS 
double System::calc_price(Event event, DateTime curr_date) {
    /*
    - event cancelled?
        - 1 week before? NO PENALTY
        - within 7 days? 1% penalty of rent amount
        - within 24 hrs? NO REFUND
        - service charge NON-REFUNDABLE
    */
    DateTime day_window = event.start_time - DateTime(0, 0, 1, 0, 0);
    DateTime week_window = event.start_time - DateTime(0, 7, 0, 0, 0);

    Payment payment;
    payment.set_charges(event.get_organizer().get_type());

    if (curr_date.operator>(day_window)) {
        return 0;
    } else if (curr_date.operator>(week_window)) {
        return payment.calc_rent_amt(event.get_duration()) * .99;
    } else if (curr_date.operator<(week_window)) {
        return payment.calc_rent_amt(event.get_duration());
    }
    return 0;
}

// HANDLES TICKETING: BUYING AND REFUNDING 
void System::handle_tickets(DateTime curr_date, User u) {
    int choice = 3;
    while (choice != 1 && choice != 2) {
        cout << "Please enter one of the two numbers below. Would you like to: " << endl
        << "\t(1) Buy a ticket" << endl
        << "\t(2) Get a refund for a ticket" << endl;
        cin >> choice;
    }

    if (choice == 1) {
        buy_ticket(curr_date, u);
    } else {
        refund_ticket(curr_date, u);
    }
}

// RUNS PURCHASE TICKET FUNCTIONALITY
void System::buy_ticket(DateTime curr_date, User u) {
    cout << "Upcoming events: " << endl;
    vector<Event> attendable_events = {};
    for (Event& e : this->events) {
        if (e.start_time.operator>=(curr_date)) {
            if (e.approved) {
                if ((e.attendee_type == "R" && u.get_type() == "R") || e.attendee_type == "N") {
                    attendable_events.push_back(e);
                    e.display_event();
                    cout << endl;
                }
            }
        }
    }

    int id;
    bool found = true;
    while (found) {
        cout << "Please enter the id of the event you'd like to purchase a ticket for: " << endl;
        cin >> id;

        if (contains_event(attendable_events, id)) {
            break;
        } else {
            cout << "We were unable to find that event. Please try again." << endl;
        }
    }

    Event eve;
    vector<Event> new_events; 
    for (Event& event : this->events) {
        if (event.id == id) {
            event.modify_attendees(u, false);
            eve = event;
            new_events.push_back(eve); 
        } else {
            new_events.push_back(event); 
        }
    }

    this->events = new_events; 

    cout << "The price of a ticket for " << eve.event_name << " is $" << eve.ticket_price << endl;
    cout << "Press any key to pay for this ticket: " << endl;

    u.subtract_funds(eve.ticket_price); //DONT KNOW IF THIS WILL ACTUALLY UPDATE PROPERLY 

    cout << "Payment complete. Your remaining account balance is: $" << u.get_funds() << endl;
    this->save(); 
}

// REFUNDS A TICKET 
void System::refund_ticket(DateTime curr_date, User u) {
    cout << "Your upcoming events: ";

    vector<Event> attendable_events = {};
    for (Event e : this->events) {
        if (e.start_time.operator>=(curr_date)) {  
            if (e.approved) {
                if (e.pub_event) {
                    if (is_attending_event(u, e)) {
                        attendable_events.push_back(e);
                        e.display_event();
                        cout << endl;
                    }
                }
            }
        }
    }

    int id;
    bool found = true;
    while (found) {
        cout << "Please enter the id of the event you'd like to get a refund for: " << endl;
        cin >> id;

        if (contains_event(attendable_events, id)) {
            break;
        } else {
            cout << "We were unable to find that event. Please try again." << endl;
        }
    }

    Event eve; 
    vector<Event> new_events; 
    for (Event event : events) {
        if (event.id == id) {
            event.modify_attendees(u, true);
            eve = event;
            new_events.push_back(eve); 
        } else {
            new_events.push_back(event); 
        }
    }

    // REASSIGNS EVENTS TO NEW EVENTS LIST 
    this->events = new_events;

    cout << "The price of a ticket for " << eve.event_name << " is $" << eve.ticket_price << endl;
    cout << "Press any key to confirm a refund for this ticket: " << endl;

    u.issue_refund(eve.ticket_price); //DONT KNOW IF THIS WILL ACTUALLY UPDATE PROPERLY 
    eve.modify_attendees(u, true);

    cout << "Refund complete. Your remaining account balance is: $" << u.get_funds() << endl;
    this->save(); 
}

// CHECKS TO SEE IF A USER IS ATTENDING EVENTS
bool System::is_attending_event(User u, Event e) {
    queue<User> tempqueue = e.get_attendees();

    while (!tempqueue.empty()) {
        User a_user = tempqueue.front();
        tempqueue.pop();

        if (a_user == u) { 
            return true;
        }
    }

    return false;
}



