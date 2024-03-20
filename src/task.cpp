// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2023/24)
// Department of Computer Science, Swansea University
//
// Author: <2142479>
//
// Canvas: https://canvas.swansea.ac.uk/courses/44636
// -----------------------------------------------------

#include "task.h"
#include <string>
#include <stdexcept>
#include <iostream>
#include <sstream>

// TODO Write a constructor that takes one parameter, a string identifier
//  and initialises the object and member data.
//
// Example:
//  Task tObj{"Task Name"};
Task::Task(const std::string& ident) : ident(ident) {
    this->setComplete(false);
}

// TODO Write a function, getIdent, that returns the identifier for the Task.
//
// Example:
//  Task tObj{"Task Name"};
//  auto ident = tObj.getIdent();
const std::string Task::getIdent() const noexcept {
    return ident;
}

// Function that returns the container object
const TagContainer &Task::getTags() const noexcept {
    return tags;
}

// TODO Write a function, setIdent, that takes one parameter, a string for a new
//  Task identifier, and updates the member variable. It returns nothing.
//
// Example:
//  Task tObj{"Task Name"};
//  auto ident = tObj.setIdent("New Task Name");
void Task::setIndent(std::string& ident) {
    this->ident = ident;
}

// TODO Write a function, addTag, that takes one parameters, a tag
//  string and returns true if the entry was inserted into the
//  container or false if the tag already existed.
//
// Example:
//  Task tObj{"Task Name"};
//  tObj.addTag("tag");
bool Task::addTag(std::string tag) {
    for (auto it = tags.begin(); it != tags.end(); it++) {
        if (*it == tag) {
            return false;
        }
    }
    tags.push_back(tag); 
    return true;
}

// TODO Write a function, deleteTag, that takes one parameter, a tag
// string, deletes it from the container, and returns true if the tag
// string was deleted. If no tag exists with that name, throw an appropriate
// exception.
//
// Example:
//  Task tObj{"Task Name"};
//  tObj.addTag("tag");
//  tObj.deleteTag("tag");
bool Task::deleteTag(std::string tag) {
    auto it = std::find(tags.begin(), tags.end(), tag);
    if (it != tags.end()) {
        tags.erase(it);
        return true;
    }  else {
        throw std::out_of_range("Tag not found");
    }
}


// TODO Write a function, numTags, that takes no parameters and returns an
// unsigned int of the number of tags in the Task contains.
//
// Example:
//  Task tObj{"Task Name"};
//  tObj.numTags();
const unsigned int Task::numTags() const noexcept {
    unsigned int count = 0;
    for (auto it = this->tags.begin(); it != this->tags.end(); it++) {
        count++;
    }
    return count;
}


// TODO Write a function, containsTag, that takes one parameter, a tag string.
// If the tag exists, return true. Otherwise return false.
//
// Example:
//  Task tObj{"Task Name"};
//  tObj.addTag("tag");
//  tObj.containsTag("tag");
bool Task::containsTag(std::string tag) const {
    for (auto it = this->tags.begin(); it != this->tags.end(); it++) {
        std::string aTag = *it;
        if (tag == aTag) {
            return true;
        }
    }
    return false;
}

// TODO Write a function, getDueDate, that returns the due date for the Task.
//
// Example:
//  Task tObj{"Task Name"};
//  tObj.getDueDate();
Date Task::getDueDate() const noexcept {
    return this->dueDate;
}

// TODO Write a function, setDueDate, that takes one parameter, a Date for
// a new due date, and updates the member variable. It returns nothing.
//
// Example:
//  Task tObj{"Task Name"};
//  Date d = Date();
//  tObj.setDueDate(d);
void Task::setDueDate(Date date) {
    dueDate = date;
}

// TODO Write a function, setComplete, that takes one parameter, a bool for
// a new completed state, and updates the member variable. It returns nothing.
//
// Example:
//  Task tObj{"Task Name"};
//  tObj.setComplete(true);
void Task::setComplete(bool aBool) {
    complete = aBool;
}


// TODO Write a function, isComplete, that returns the completed state for the
// Task.
//
// Example:
//  Task tObj{"Task Name"};
//  if(tObj.isComplete()) {
//   ...
//  }
const bool Task::isComplete() const noexcept {
    return complete;
}

// TODO Write an == operator overload for the Task class, such that two
// Tasks objects are equal only if they have the same identifier, dueDate,
// completed state, and tags.
//
// Example:
//  Task tObj1{"Task Name"};
//  Task tObj2{"Task Name"};
//  if(tObj1 == tObj2) {
//   ...
//  }
bool operator==(const Task &task1, const Task &task2) {
    if (task1.getIdent() == task2.getIdent() 
        && task1.getDueDate() == task2.getDueDate() 
        && task1.isComplete() == task2.isComplete()) {
        
        for(auto it = task1.getTags().begin(); it != task1.getTags().end(); it++) {
            if (!task2.containsTag(*it)) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

// TODO Write a function, str, that takes no parameters and returns a
//  std::string of the JSON representation of the data in the Item.
//
// See the coursework specification for how this JSON should look.
//
// Example:
//  Item iObj{"itemIdent"};
//  std::string s = iObj.str();
std::string Task::str() const {

    std::string stringTags = "[";
    for (auto it = this->tags.begin(); it != this->tags.end(); it++) {
        stringTags += "\"" + *it + "\"";
        if ((it + 1) != this->tags.end()) {
            stringTags += ",";
        } else {
            stringTags += "]";
        }
    }
    std::string completeString;
    if (this->complete) {
        completeString = "true";
    } else {
        completeString = "false";
    }

    std::stringstream sttr;    
    sttr << "\"" << this->ident << "\":{\"completed\":" << completeString << ",\"dueDate\":\"" << this->dueDate.str() 
        << "\",\"tags\":" << stringTags << "}";
    
    return sttr.str();
    
}

nlohmann::json Task::json() const {
    return {
        {"completed", complete},
        {"dueDate", dueDate.str()},
        {"tags", tags}
    };
}


