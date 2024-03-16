// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2023/24)
// Department of Computer Science, Swansea University
//
// Author: <2142479>
//
// Canvas: https://canvas.swansea.ac.uk/courses/44636
// -----------------------------------------------------
// The root object that holds all data for 371todo. This
// class contains Projects, which in turn hold Tasks,
// which have a name, due date, completed status, and tags.
// -----------------------------------------------------

#ifndef TODOLIST_H
#define TODOLIST_H

#include <map>
#include <string>
#include <utility>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>

#include "lib_json.hpp"

#include "project.h"

using ProjectContainer = std::vector<Project>;
using json = nlohmann::json;

class TodoList {
    ProjectContainer projects;

    public:
        explicit TodoList();
        ~TodoList() = default;

        unsigned int size() const noexcept;
        Project& newProject(const std::string &tIdent);

        bool addProject(Project project);
        bool containsProject(const std::string &tIdent) const noexcept;
        Project& getProject(const std::string &tIdent);
        bool deleteProject(const std::string &tIdent);
        void load(const std::string& fileName);
        void save(const std::string& fileName);

        friend bool operator==(const TodoList &c1, const TodoList &c2);
        std::string str() const;

};

#endif // TODOLIST_H
