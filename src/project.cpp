// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2023/24)
// Department of Computer Science, Swansea University
//
// Author: <STUDENT NUMBER>
//
// Canvas: https://canvas.swansea.ac.uk/courses/44636
// -----------------------------------------------------

#include "project.h"

// TODO Write a constructor that takes one parameter, a string identifier and
// initialises the object and member data.
//
// Example:
//  Project p{"projectIdent"};
Project::Project(std::string ident) : ident(ident) {}

// TODO Write a function, size, that takes no parameters and returns an unsigned
// int of the number of Tasks in the Project contains.
//
// Example:
//  Project p{"projectIdent"};
//  auto size = p.size();
unsigned int Project::size() const noexcept{
    return this->tasks.size();
}


// TODO Write a function, getIdent, that returns the identifier for the Project.
//
// Example:
//  Project pObj{"projectIdent"};
//  auto ident = pObj.getIdent();
const std::string& Project::getIdent() const noexcept {
    return this->ident;
}

// TODO Write a function, setIdent, that takes one parameter, a string for a new
// Project identifier, and updates the member variable. It returns nothing.
//
// Example:
//  Project pObj{"projectIdent"};
//  pObj.setIdent("projectIdent2");
void Project::setIdent(std::string pIdent) noexcept {
    this->ident = pIdent;
}

// A function that returns the TaskContainer
const TaskContainer& Project::getTasks() const noexcept {
    return tasks;
}

// TODO Write a function, newTask, that takes one parameter, a Task identifier,
// (a string) and returns the Task object as a reference. If an object with the
// same identifier already exists, then the existing object should be returned.
// Throw a std::runtime_error if the Task object cannot be inserted into the
// container for whatever reason.
//
// Example:
//  Project pObj{"projectIdent"};
//  pObj.newTask("newTaskName");
Task &Project::newTask(const std::string &tIdent) {
    for (auto& task : this->tasks) {
        if (task.getIdent() == tIdent) {
            return task;
        }
    }

    try {
        tasks.push_back(Task(tIdent));
    } catch(const std::runtime_error& e)
    {
        throw e;
    }

    return tasks.back();
    
}

TaskContainer::iterator Project::findTask(const std::string &tIdent) {
    for (auto it = this->tasks.begin(); it != this->tasks.begin(); it++) {
        Task task = *it;
        if (task.getIdent() == tIdent) {
            return it;
        }
    }
    
    throw std::runtime_error("Task not found");
    
}

bool Project::containsTask(const std::string &tIdent) const noexcept {
    for (auto it = this->tasks.begin(); it != this->tasks.end(); it++) {
        Task task = *it;
        if (task.getIdent() == tIdent) {
            return true;
        }
    }
    return false;
    
    return false;
}

// TODO Write a function, addTask, that takes one parameter, a Task object, and
// returns true if the object was successfully inserted. If an object with the
// same identifier already exists, then:
//  - the tags should be merged
//  - completed status overwritten by the task being added
//  - dueDate overwritten by the task being added
//  - false should be returned.
//
// Example:
//  Project pObj{"projectIdent"};
//  Task tObj{"taskIdent"};
//  pObj.addItem(tObj);
bool Project::addTask(Task task) {
    for (Task& aTask : this->tasks) {
        if (aTask.getIdent() == task.getIdent()) {
            for (const auto& tag : task.getTags()) {
                if (!aTask.containsTag(tag)) {
                    aTask.addTag(tag);
                }
            }
            aTask.setComplete(task.isComplete());
            aTask.setDueDate(task.getDueDate());
            return false;
        }
    }   
    this->tasks.push_back(task);
    return true;
}

// TODO Write a function, getTask, that takes one parameter, a Task identifier
// (a string) and returns the Task as a reference. If no Task exists, throw an
// appropriate exception.
//
// Hint:
//  See the test scripts for the exception expected.
//
// Example:
//  Project pObj{"projectIdent"};
//  pObj.newTask("newTaskName");
//  auto tObj = pObj.getTask("newTaskName");
Task& Project::getTask(const std::string &tIdent) {
    if (this->tasks.empty()) {
        throw std::out_of_range("Task list is empty");
    }

    for (auto& task : this->tasks) {
        if (task.getIdent() == tIdent) {
            return task;
        }
    }
    throw std::out_of_range("Task not found");
}



// TODO Write a function, deleteTask, that takes one parameter, a Task
// identifier (a string), deletes it from the container, and returns true if the
// Task was deleted. If no Task exists, throw an appropriate exception.
//
// Example:
//  Project pObj{"projectIdent"};
//  pObj.newTask("newTaskName");
//  bool result = pObj.deleteTask("newTaskName");
bool Project::deleteTask(const std::string &tIdent) {
    for (auto it = this->tasks.begin(); it != this->tasks.end(); ++it) {
        Task task = *it;
        if (task.getIdent() == tIdent) {
            this->tasks.erase(it);
            return true;
        }
    }
    throw std::out_of_range("Task not found");
}


// TODO Write an == operator overload for the Project class, such that two
// Project objects are equal only if they have the same identifier and same
// Tasks.
//
// Example:
//  Project pObj{"projectIdent1"};
//  pObj.newTask("newTaskName");
//  Project pObj1{"projectIdent2"};
//  if(pObj1 == pObj2) {
//    ...
//  }
bool operator==(const Project &c1, const Project &c2) {
    if (c1.getIdent() != c2.getIdent()) {
        return false;
    }

    for (const Task& task : c1.getTasks()) {
        if (!c2.containsTask(task.getIdent())) {
            return false;
        }
    }

    for (const Task& task : c2.getTasks()) {
        if (!c1.containsTask(task.getIdent())) {
            return false;
        }
    }
    return true;
}


// TODO Write a function, str, that takes no parameters and returns a
// std::string of the JSON representation of the data in the Project.
//
// See the coursework specification for how this JSON should look.
//
// Example:
//  Project pObj{"projectIdent"};
//  std::string s = pObj.str();
std::string Project::str() const {
    std::string stringTasks;
    for (auto it = this->tasks.begin(); it != this->tasks.end(); it++) {
        Task task = *it;
        stringTasks += task.str();
        if ((it + 1) != this->tasks.end()) {
            stringTasks += ",";
        } else {
            stringTasks += "}";
        }
    }

    std::stringstream sttr;    
    sttr << "\"" << this->ident << "\":" << stringTasks;
    
    return sttr.str();
}

nlohmann::json Project::json() const {
    nlohmann::json projectJson;
    for (const Task& task : tasks) {
        projectJson[task.getIdent()] = task.json();
    }
    return projectJson;
}

