#ifndef DATETIME_H
#define DATETIME_H

#include <string>

using namespace std; 

class DateTime {
    public:
        int year;
        int month;
        int day;
        int hour;
        int minute; // FOR PRECISION
        
        // CONSTRUCTOR METHOD
        DateTime(int year, int month, int day, int hour, int minute);

        // DEFAULT CONSTRUCTOR 
        DateTime(); 

        // GETS THE DATE AND RETURNS AS A STRING
        string get_date(); 

        // GETS THE TIME AND RETURNS AS A STRING IN MILITARY TIME 
        string get_time(); 

        // GETS THE CURRENT TIME AND OVERWRITES THE CURRENT OBJECT WITH IT 
        void get_current();

        // OVERRIDE FUNCTION FOR ==, ONLY CHECKS IF THEY ARE SAME DATE
        bool operator==(const DateTime& other) const;

        // OVERRIDE FUNCTION FOR <, CHECKS TIME
        bool operator<(const DateTime& other) const; 

        // OVERRIDE FUNCTION FOR >
        bool operator>(const DateTime& other) const; 

        // OVERRIDE FUNCTION FOR -
        DateTime operator-(const DateTime& other) const;

        // OVERRIDE FUNCTION FOR >=
        bool operator>=(const DateTime& other) const;

        // REFORMATS TIME TO PREVENT OVERFLOW
        void format_time();

        // DESTRUCTOR
        ~DateTime() {}; 
};

// SPLITS A DATE STRING AND A TIME STRING INTO DateTime OBJECT
DateTime splice_date_time(string date, string time); 

#endif // DATETIME_H