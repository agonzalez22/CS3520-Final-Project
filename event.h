#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <vector>
#include <queue>
#include <cmath>

#include "user.h"
#include "datetime.h"

using namespace std; 

// ENUMERATION FOR LAYOUT STYLES
enum LayoutStyle { MEETING, LECTURE, WEDDING, DANCE };

/*
* EVENT CLASS DEFINITION
*/
class Event {
    protected:
        queue<User> attendees{}; // QUEUE TO HOLD ATTENDEES
        queue<User> waitlist{};  // QUEUE TO HOLD WAITLIST

    public:
        string event_name;       // NAME OF THE EVENT
        User organizer;          // ORGANIZER OF THE EVENT
        bool pub_event;          // FLAG TO INDICATE IF EVENT IS PUBLIC
        LayoutStyle layout;      // LAYOUT STYLE OF THE EVENT
        DateTime start_time;     // START TIME OF THE EVENT
        DateTime end_time;       // END TIME OF THE EVENT
        int capacity = 40;       // DEFAULT CAPACITY OF THE EVENT
        string attendee_type;    // TYPE OF ATTENDEES (R FOR RESIDENT, N FOR NON-RESIDENT)
        bool approved = false;   // FLAG TO INDICATE IF EVENT IS APPROVED
        double ticket_price;     // PRICE OF THE TICKET
        int id; // UNIQUE ID OF EVENT 

        // DEFAULT CONSTRUCTOR FOR EVENT CLASS
        Event();

        // CONSTRUCTOR FOR EVENT CLASS
        Event(string event_name, User organizer, bool pub_event, LayoutStyle layout, 
              DateTime start_time, DateTime end_time, int capacity, double ticket_price, string attendee_type);

        // GETTER METHOD FOR ATTENDEES QUEUE
        queue<User> get_attendees(); 
        
        // GETTER METHOD FOR WAITLIST QUEUE
        queue<User> get_waitlist(); 
        
        // METHOD TO GET SIZE OF ATTENDEES QUEUE
        int get_attendees_size(); 
        
        // METHOD TO GET SIZE OF WAITLIST QUEUE
        int get_waitlist_size(); 
        
        // METHOD TO DISPLAY AN EVENT
        void display_event(); 

        // GETS THE LAYOUT OF AN EVENT IN STRING FORMAT
        string get_layout(); 

        // GENERATES A UNIQUE ID FOR AN EVENT.
        void generate_id(); 
        
        // SETS THE STATUS OF AN EVENT. APPROVED = TRUE IF FULL PAYMENT WAS GIVEN.
        void set_status(const bool& status) { this->approved = status; }

        // GETTER FOR THE ORGANIZER
        User get_organizer() { return this->organizer; }
        
        // METHOD TO GET THE DURATION OF AN EVENT IN HOURS
        int get_duration() {
            int hrs = abs(this->end_time.hour - this->start_time.hour);
            int mins = abs(this->end_time.minute - this->start_time.minute);

            return ceil(hrs + (mins / 60));
        }

        // MODIFIES THE LIST OF ATTENDEES OF AN EVENT BY REMOVING THE GIVEN USER.
        void modify_attendees(User u, bool remove_person);

        // OVERRIDES == OPERATIONS
        bool operator==(const Event& other) const { return this->id == other.id; } 

        // SETTER METHOD FOR ATTENDEES QUEUE
        void set_attendees(queue<User> a) {
            this->attendees = a; 
        }

        // SETTER METHOD FOR WAITLIST QUEUE
        void set_waitlist(queue<User> w) {
            this->waitlist = w;
        }
};

// FUNCTION TO PROMPT THE USER FOR START AND END DATES
vector<DateTime> prompt_for_dates();

#endif // EVENT_H