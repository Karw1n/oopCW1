// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2023/24)
// Department of Computer Science, Swansea University
//
// Author: <2142479>
//
// Canvas: https://canvas.swansea.ac.uk/courses/44636
// -----------------------------------------------------

#include "date.h"
#include <string>
#include <sstream>

// Provided default constructor to create an unitialised date.
Date::Date() : year(0), month(0), day(0), initialized(false) {}

// TODO write a function setDateFromString that takes a string argument in
// "YYYY-MM-DD" format and sets the appropriate member variables (year, month,
// day, and initialised). If dateString is empty reset the object to its
// uninitialised state (see default constructor). If dateString is not valid
// throw an appropriate exception.
// Example:
//  Date d = Date();
//  d.setDateString("2024-01-01");
void Date::setDateFromString(const std::string& date) {
    if (date.empty()) {
        this->initialized = false;
    } else {
        if ((date.size() != 10 && date.size() != 9) || date[4] != '-' || (date[7] != '-' && date[6] != '-')) {
            throw std::invalid_argument("Incorrect date string formated entered. Correct format: YYYY-MM-DD. Date was: " + date);
        }

        unsigned int theYear = std::stoi(date.substr(0, 4));
        unsigned int theMonth;
        if (date[7] == '-') {
            theMonth = std::stoi(date.substr(5, 2));
        } else {
            theMonth = std::stoi(date.substr(5, 1));
        }
        unsigned int theDay;
        
        if (date.size() == 10) {
            theDay = std::stoi(date.substr(8, 2));
        } else {
            theDay = std::stoi(date.substr(8, 1));
        }
        
        if (!isValidDate(theYear, theMonth, theDay)) {   
            this->initialized = false;
            throw std::invalid_argument("Incorrect date entered.");
        } else {
            this->year = theYear;
            this->month = theMonth;
            this->day = theDay;
            this->initialized = true;
        }
    } 
}

// Function to set iniialized state.
void Date::setInitialised(bool initialised) {
    this->initialized = initialised;
}

// TODO Write a function, isInitialised, that takes no parameters and returns true
//  if the Date object is initialised, and false if it is NOT initialised.
// Example:
//  Date d = Date();
//  if(d.isInitialised()) {
//    ...
//  }
const bool Date::isInitialised() const noexcept { 
    return this->initialized;
}

// TODO Write a function, str, that takes no parameters and returns a
// std::string representation of the Date object in YYYY-MM-DD format.  If the
// Date object is NOT initialised, return an empty string.
// Example:
//  Date d = Date();
//  d.setDate(2024,1,1);
//  std::cout << d.str() << std::endl;
std::string Date::str() const {
    if (this->initialized) {
        std::stringstream sttr;
        sttr << year << "-" << month << "-" << day;
        return sttr.str();
    } else {
        return "";
    }
}

// TODO create a function, setDate, that takes three parameters: year, month,
// day and sets the appropriate member variables to those parameters and also
// sets the initilized flag.
// Example:
//  Date d = Date();
//  d.setDate(2024, 1, 1);
void Date::setDate(unsigned int year, unsigned int month, unsigned int day) noexcept {
    this->year = year;
    this->month = month;
    this->day = day;
    this->initialized = true;
}

// TODO Write a function, getYear, that takes no parameters and returns year
// member variable
//  Date d = Date();
//  auto year = d.getYear();
const unsigned int Date::getYear() const noexcept {
    return year;
}

// TODO Write a function, getMonth, that takes no parameters and returns month
// member variable
//  Date d = Date();
//  auto month = d.getMonth();
const unsigned int Date::getMonth() const noexcept {
    return month;
}

// TODO Write a function, getDay, that takes no parameters and returns day
// member variable
//  Date d = Date();
//  auto day = d.getDay();
const unsigned int Date::getDay() const noexcept {
    return day;
}

// TODO Write an == operator overload for the Date class, such that two
// Date objects are equal only if they have the same member variables.
// Example
//  Date d1 = Date();
//   d1.setDate(2024, 1, 1);
//   Date d2 = Date();
//   d2.setDate(2024, 1, 2);
//   if (d1 == d2) {
//     ...
//   }
bool operator==(const Date &date1, const Date &date2) {
    return (date1.getDay() == date2.getDay() 
        && date1.getMonth() == date2.getMonth() 
        && date1.getYear() == date2.getYear());
}


// TODO Write an < operator overload for the Date class, that returns true if
// the first Date object is chronologically before the second date object.
// Otherwise return false. Note: You can ignore the 'initialised' member
// variable when drawing the comparison.

//  Date d1 = Date();
//   d1.setDate(2024, 1, 1);
//   Date d2 = Date();
//   d2.setDate(2024, 1, 2);
//   if (d1 < d2) {
//     ...
//   }
bool operator<(const Date &date1, const Date &date2) {
    return ((date1.getYear() < date2.getYear()) 
        || (date1.getMonth() < date2.getMonth() && date1.getYear() <= date2.getYear()) 
        || (date1.getDay() < date2.getDay() && date1.getMonth() == date2.getMonth() 
        && date1.getYear() <= date2.getYear()));
}

// Helper function to determine whether a given date (year, month, day) is a valid date
const bool Date::isValidDate(unsigned int year, unsigned int month, unsigned int day) {
    if (year < 1 || month < 1 || month > 12 || day < 1) {
        return false;
    }   

    if (month == 2) {
        if (isLeapYear(year)) {
            return day <= 29;
        } else {
            return day <= 28;
        }
    }

    if (month == 4 || month == 6 || month == 9 || month == 11) {
        return day <= 30;
    }

    return day <= 31;
}

// Helper function to determine whether a given year is a leap year
const bool Date::isLeapYear(unsigned int year) {
    return ((year % 4 == 0 && 100 != 0) || (year % 400 == 0));
}