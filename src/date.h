// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2023/24)
// Department of Computer Science, Swansea University
//
// Author: <2142479>
//
// Canvas: https://canvas.swansea.ac.uk/courses/44636
// -----------------------------------------------------
// An Date class that contains the following member variables
// - year
// - month
// - day
// - initialised
//
// A Date object should only be set to initialised if it is a valid date.
// Otherwise it is should remain unintialised.
// -----------------------------------------------------

#ifndef DATE_H
#define DATE_H

#include "lib_json.hpp"

class Date {
private:
  unsigned int year, month, day;
  bool initialized;

public:
  Date();
  void setDateFromString(const std::string& string);
  bool isInitialised() const;
  std::string str() const;
  void setDate(unsigned int year, unsigned int month, unsigned int day);
  unsigned int getYear() const;
  unsigned int getMonth() const;
  unsigned int getDay() const;
  bool operator==(const Date &otherDate) const;
  bool operator<(const Date &otherDate) const;
};

#endif // DATE_H
