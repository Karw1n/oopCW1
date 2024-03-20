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
  explicit Date();
  ~Date() = default;

  void setDateFromString(const std::string& string);
  void setInitialised(bool initialised);
  const bool isInitialised() const noexcept;
  std::string str() const;

  void setDate(unsigned int year, unsigned int month, unsigned int day) noexcept;

  const unsigned int getYear() const noexcept;
  const unsigned int getMonth() const noexcept;
  const unsigned int getDay() const noexcept;
  friend bool operator==(const Date &date1, const Date &date2);
  friend bool operator<(const Date &date1, const Date &date2);

  const bool isValidDate(unsigned int year, unsigned int month, unsigned int day);
  const bool isLeapYear(unsigned int year); 
};

#endif // DATE_H
