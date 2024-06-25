#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>
#include "user.h"
#include "event.h"
#include "utils.h"

using namespace std; 

/*
* SYSTEM CLASS DEFINITION
*/
class System {
private:
    vector<Event> events; // VECTOR TO HOLD EVENTS
    double funds; // DOUBLE CONTAINING CITY FUNDS
    vector<pair<Event, DateTime>> cancelled_events; // VECTOR STORING THE CANCELLED EVENTS

public:
    // DEFAULT CONSTRUCTOR
    System(); 

    // CONSTRUCTOR WITH PARAMETERS
    System(vector<Event> events, double funds);

    // DISPLAYS ALL EVENTS IN THE SYSTEM IN A NEAT ORDER
    void display_events(DateTime curr_date, User u);

    // RUNS THE SYSTEM
    void run(); 

    // PROMPTS USER LOGIN
    User prompt_login();

    // DISPLAYS TODAY'S EVENTS
    void display_todays_events(DateTime curr_date, User u);

    // REQUESTS AN EVENT
    void request_event(User u); 

    // CHECKS FOR TIME CONFLICT WITHIN THE SYSTEM AGAINST NEW RESERVATION REQUEST
    bool is_time_conflict(vector<DateTime> d1);

    // APPROVES PENDING EVENTS
    void approve_pending_events(); 

    // FUNDS GETTER METHOD 
    double get_funds(); 

    // SETTER FOR FUNDS 
    void subtract_funds(double new_funds); 

    // CHECKS EVENT TYPE LIMIT WITHIN SYSTEM
    bool is_event_limit(DateTime curr_date, User u);

    // SAVES THE STATE OF THE SYSTEM 
    void save(); 

    // HANDLES EVENT PAYMENTS FOR ORGANIZERS
    void handle_event_payments(DateTime curr_date, User u);

    // ALLOWS YOU TO MAKE PAYMENT FOR AN EVENT 
    void make_payment_option(vector<Event>& events, User u);

    // CHECKS IF AN EVENT EXISTS IN A LIST OF EVENTS
    bool contains_event(vector<Event>& events, int id);

    // CANCELS AN EVENT
    void cancel_event_option(DateTime curr_date, vector<Event> events, User u);
    
    // CANCELLATION REQUEST MANAGER FOR THE MANAGER 
    void approve_cancellation_reqs();

    // CALCULATES THE PRICE OF AN EVENT
    double calc_price(Event event, DateTime curr_date);

    // HANDLES TICKETING FOR RESIDENTS AND NON-RESIDENTS
    void handle_tickets(DateTime curr_date, User u);

    // ALLOWS YOU TO BUY A TICKET
    void buy_ticket(DateTime curr_date, User u);

    // ALLOWS YOU TO GET A REFUND FOR YOUR TICKET
    void refund_ticket(DateTime curr_date, User u);

    // CHECKS IF USER IS ATTENDING THE GIVEN EVENT
    bool is_attending_event(User u, Event e);

    // DESTRUCTOR
    ~System() {}; 
};

// DISPLAYS MENU OPTIONS AND RETRIEVES USER CHOICE FROM MENU OPTIONS
int display_menu(string u_type);

// CHECKS TO SEE IF A GIVEN USER_TYPE IS A VALID TYPE
bool is_not_a_valid_type(string user_type);

#endif 