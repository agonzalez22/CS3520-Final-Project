#include <iostream>
#include <queue>
#include <cstdlib>
#include <random>
#include <ctime>

#include "event.h"
#include "user.h"
#include "datetime.h"

using namespace std; 

// DEFAULT CONSTUCTOR METHOD 
Event::Event() : event_name(""), organizer(User()), pub_event(false), layout(MEETING), start_time(DateTime()), end_time(DateTime()), capacity(40), ticket_price(0.0), attendee_type("X") { this->generate_id(); }

// CONSTRUCTOR METHOD
Event::Event(string event_name, User organizer, bool pub_event, LayoutStyle layout, 
            DateTime start_time, DateTime end_time, int capacity, double ticket_price, string attendee_type) 
            : event_name(event_name), organizer(organizer), pub_event(pub_event), layout(layout), 
              start_time(start_time), end_time(end_time), capacity(capacity), ticket_price(ticket_price), 
              attendee_type(attendee_type) {
                this->generate_id(); 
              }

// DISPLAYS THE EVENTS
void Event::display_event() {
    cout << this->event_name << endl; 
    cout << string(30, '-') << endl; 
    cout << "Organized by: " << this->organizer.name << endl; 
    cout << "Date: " << this->start_time.get_date() << endl; 
    cout << "Time: " << this->start_time.get_time() << " - " << this->end_time.get_time() << endl; 
    cout << "Price: $" << this->ticket_price << endl; 
    cout << "Capacity: " << this->get_attendees_size() << "/" << this->capacity << endl; 
    cout << "Event type: " << this->get_layout() << endl;
    cout << "Waitlist capacity: " << this->get_waitlist_size() << "/0" << endl;  
    cout << "Event ID: " << this->id << endl; 
}

// GETS THE LAYOUT ENUM AND RETURNS APPROPRIATE STRING FORM
string Event::get_layout() {
    switch(this->layout) {
        case MEETING: 
            return "Meeting"; 
        case LECTURE: 
            return "Lecture"; 
        case WEDDING: 
            return "Wedding"; 
        case DANCE: 
            return "Dance"; 
        default: 
            return "None"; 
    }
    return "None"; 
}

// RETURNS THE SIZE OF THE ATTENDEES QUEUE
int Event::get_attendees_size() {
    return this->get_attendees().size(); 
}

// RETURNS THE SIZE OF THE WAITLIST QUEUE
int Event::get_waitlist_size() {
    return this->get_waitlist().size(); 
}

// ATTENDEES GETTER METHOD
queue<User> Event::get_attendees() {
    return attendees; 
}

// WAITLIST GETTER METHOD
queue<User> Event::get_waitlist() {
    return waitlist; 
}

// PROMPT USER FOR EVENT DATES AND CONVERT TO DateTime
vector<DateTime> prompt_for_dates() {
    DateTime start, end; 
    string start_date, start_time, end_date, end_time;
    cout << "PLEASE PROVIDE TIMES IN MILITARY 24 HOUR TIME!\n" << endl;

    // GET THE START DATE
    cout << "What is the start date [MM/DD/YYYY]: "; 
    cin >> start_date; 
    cout << endl;  

    // GET THE START TIME
    cout << "What is the start time [HH:MM]: "; 
    cin >> start_time; 
    cout << endl; 

    // GET THE END DATE
    cout << "What is the end date [MM/DD/YYYY]: "; 
    cin >> end_date; 
    cout << endl; 

    // GET THE END TIME
    cout << "What is the end time [HH:MM]: "; 
    cin >> end_time; 
    cout << endl; 

    // CONVERT DATE AND TIME STRINGS TO DateTime OBJECTS
    start = splice_date_time(start_date, start_time); 
    end = splice_date_time(end_date, end_time); 

    return vector<DateTime>{start, end};
}

// GENERATES A UNIQUE ID FOR AN EVENT 
void Event::generate_id() {
    srand(time(0)); // SEED RANDOM w/ CURR TIME
    this->id = rand() % 1001;
}


// REMOVES THE GIVEN USER FROM THE QUEUE OF ATTENDEES 
void Event::modify_attendees(User u, bool remove_person) {
    queue<User> tempqueue;

    while (!get_attendees().empty()) {
        User a_user = this->attendees.front();
        this->attendees.pop();

        if (remove_person) {
            if (a_user == u) { // CHECKS TO SEE IF CURRENT USER IS THE GIVEN USER
                ; 
            } else {
                tempqueue.push(a_user); // PUSH TO NEW QUEUE
            }
        } 
    }

    if (remove_person) {
        // ASSIGNS ATTENDEES LIST TO NEWLY MADE QUEUE
        this->set_attendees(tempqueue); 
    } else {
        this->attendees.push(u);
    }
}