/* Alina Gonzalez & Khushi Khan 
* CS3520 6/24/2024
* DateTime functionality and any supplemental methods.
*/
#include "datetime.h"

#include <iostream> 
#include <sstream> 
#include <chrono> 
#include <iomanip> 
#include <ctime> 
#include <vector> 

using namespace std; 

// DEFAULT CONSTRUCTOR
DateTime::DateTime() : year(1900), month(1), day(1), hour(0), minute(0) {}

// CONSTRUCTOR WITH PARAMETERS
DateTime::DateTime(int year, int month, int day, int hour, int minute) : year(year), month(month), day(day), hour(hour), minute(minute) {} 

// OVERRIDE FUNCTION FOR ==, ONLY CHECKS IF THEY ARE SAME DATE
bool DateTime::operator==(const DateTime& other) const {
    // CHECK IF THE DATES ARE THE SAME
    if (year != other.year) {
        return false; 
    }

    if (month != other.month) {
        return false; 
    }

    if (day != other.day) {
        return false; 
    }

    return true; 
}

// OVERRIDE FUNCTION FOR <, CHECKS TIME
bool DateTime::operator<(const DateTime& other) const {
    if (this->hour > other.hour) {
        return false; 
    }

    if (this->minute > other.minute) {
        return false; 
    }

    return true; 
}

// OVERRIDE FUNCTION FOR >
bool DateTime::operator>(const DateTime& other) const {
    if (this->hour < other.hour) {
        return false; 
    }

    if (this->minute < other.minute) {
        return false; 
    }

    return true; 
}

// OVERRIDE FUNCTION FOR >=
bool DateTime::operator>=(const DateTime& other) const {
    // CHECK IF THE DATE AND TIMES ARE THE SAME
    bool is_this_day_same_as_that = false;

    // THIS DAY IS AFTER THAT DAY
    // THIS DAY IS THAT DAY
    // THIS DAY IS BEFORE THAT DAY 

    // if this event's DATE comes after other event's DATE
    if (year >= other.year) {
        if (month >= other.month) {
            if (day > other.day) { // AFTER OONLY
                return true;
            }
        }
    }

    // if this event's DATE is the same as other event's DATE
    // IF THIS EVENT'S DATE IS THE SAME AS OTHER'S
    if (year == other.year) {
        if (month == other.month) {
            if (day == other.day) {
                is_this_day_same_as_that = true;
            }
        }
    }

    if (is_this_day_same_as_that) {
        if (hour >= other.hour) {
            if (minute >= other.minute) {
                return true;
            }
        }
    } else {
        return false;
    }

    /*
    coopers birthday bash - 11:30 6/26/2024
    rn - 19:28 6/24/2024
    11 < 19 
    return false
    */
   return false; 
}

// FORMAT THE DATE IN A NICE WAY AND RETURN AS A STRING
string DateTime::get_date() {
    return to_string(this->month) + "/" + to_string(this->day) + "/" + to_string(this->year); 
}

// FORMATS THE TIME IN A NICE WAY AND RETURN AS A STRING
string DateTime::get_time() {
    return to_string(this->hour) + ":" + to_string(this->minute); 
}

// GET THE CURRENT DATE AND TIME
void DateTime::get_current() {
    auto now = chrono::system_clock::now(); // GET THE CURRENT TIME

    time_t now_time_t = chrono::system_clock::to_time_t(now); 

    tm now_tm = *std::localtime(&now_time_t); // CONVERT TO LOCAL TIME

    this->year = now_tm.tm_year + 1900; // YEAR SINCE 1900
    this->month = now_tm.tm_mon + 1;    // MONTHS SINCE JANUARY (0-11)
    this->day = now_tm.tm_mday;         
    this->hour = now_tm.tm_hour;        // MILITARY TIME
    this->minute = now_tm.tm_min;       
}

// SPLITS A DATE STRING AND A TIME STRING INTO DateTime OBJECT
DateTime splice_date_time(string date, string time){
    int month, day, year, minute, hour; 

    stringstream date_ss(date); 

    date_ss >> month; 
    date_ss.ignore(1); 
    date_ss >> day; 
    date_ss.ignore(1); 
    date_ss >> year; 
    
    stringstream time_ss(time); 

    time_ss >> hour; 
    time_ss.ignore(1); 
    time_ss >> minute; 

    DateTime d(year, month, day, hour, minute);

    return d;
}

// OVERRIDE - OPERATOR 
DateTime DateTime::operator-(const DateTime& other) const {
    DateTime result = *this;
    result.year -= other.year;
    result.month -= other.month;
    result.day -= other.day;
    result.hour -= other.hour;
    result.minute -= other.minute;
    result.format_time();
    return result;
}

// RE-FORMATS TIME AFTER SUBTRACTION
void DateTime::format_time() {
    if (minute < 0) {
        int borrow = (abs(minute) + 59) / 60;
        hour -= borrow;
        minute += borrow * 60;
    }

    if (hour < 0) {
        int borrow = (abs(hour) + 23) / 24;
        day -= borrow;
        hour += borrow * 24;
    }

    if (day <= 0) {
        int borrow = (abs(day) + 29) / 30;
        month -= borrow;
        day += borrow * 30;
    }

    if (month <= 0) {
        int borrow = (abs(month) + 11) / 12;
        year -= borrow;
        month += borrow * 12;
    }
}

