// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2023/24)
// Department of Computer Science, Swansea University
//
// Author: <2142479>
//
// Canvas: https://canvas.swansea.ac.uk/courses/44636
// -----------------------------------------------------

#include <algorithm>
#include <iostream>
#include <string>

#include "371todo.h"
#include "lib_cxxopts.hpp"

// TODO Complete this function. You have been provided some skeleton code which
//  retrieves the database file name from cxxopts.
//  1. Load the database file by calling load() on a TodoList object
//  2. Parse the 'action' argument to decide what action should be taken
//     (json, create, delete, update).  'json' is the easiest to implement, and
//     'update' is the hardest. The details of how these arguments work is in
//     the coursework specification.
//  3. Save the updated TodoList object to the JSON file if there have been
//     changes (calling save() on the TodoList object).
//
// Some commented out code has been provided. Using some of this will be
// demonstrated in the coursework video on Canvas. Remember, this coursework is
// meant to be challenging and testing your understanding of programming in C++.
// Part of the challenge is figuring things out on your own. That is a major
// part of software development.
//
// Example:
//int main(int argc, char *argv[]) { return App::run(argc, argv); }
int App::run(int argc, char *argv[]) {
  auto options = App::cxxoptsSetup();
  auto args = options.parse(argc, argv);

  // Print the help usage if requested
  if (args.count("help")) {
    std::cout << options.help() << '\n';
    return 0;
  }

  // Open the database and construct the TodoList
  const std::string db = args["db"].as<std::string>();
  TodoList tlObj{};

  // Only uncomment this once you have implemented the load function!
  tlObj.load(db);

  const Action a = parseActionArgument(args);
  switch (a) {
    case Action::CREATE: {
      if (args["project"].count()) {
        std::string projectIdent = args["project"].as<std::string>();
        if (!tlObj.containsProject(projectIdent)) {
          tlObj.newProject(projectIdent);
        }

        if (args["task"].count()) {
          std::string taskIdent = args["task"].as<std::string>();
          if (!tlObj.getProject(projectIdent).containsTask(taskIdent)) {
            tlObj.getProject(projectIdent).newTask(taskIdent);
          }
          if (args["tag"].count()) {
            std::vector<std::string> tags = splitTags(args["tag"].as<std::string>());
            for (std::string tag : tags) {
              if (!tlObj.getProject(projectIdent).getTask(taskIdent).containsTag(tag)) {
                tlObj.getProject(projectIdent).getTask(taskIdent).addTag(tag);  
              }
            }
          }

          if (args["due"].count()) {
            std::string dateAsString = args["due"].as<std::string>();
            Date dueDate = Date();
            try {
              dueDate.setDateFromString(dateAsString);
            } catch (const std::invalid_argument& e) {
              std::cerr << "Invalid date: " << dateAsString << std::endl;
              return 1;
            }
            tlObj.getProject(projectIdent).getTask(taskIdent).setDueDate(dueDate);
          }

          if (args["completed"].count()) {
            tlObj.getProject(projectIdent).getTask(taskIdent).setComplete(true);
          } 
          if (args["incomplete"].count()) {
            tlObj.getProject(projectIdent).getTask(taskIdent).setComplete(false);
          }
        }    
        tlObj.save(db);      
      } else {
        std::cerr << "Error: missing project, task, tag, due, completed/incomplete argument(s)." << std::endl;
        return 1;
      }
      
      break;
    }
    case Action::JSON: {
      if (args["project"].count()) {
        std::string projectIdent = args["project"].as<std::string>();

        if (tlObj.containsProject(projectIdent)) {
          auto& project = tlObj.getProject(projectIdent);
          if (args["task"].count()) {
            std::string taskIdent = args["task"].as<std::string>();
            
            if (project.containsTask(taskIdent)) {
              auto& task = tlObj.getProject(projectIdent).getTask(taskIdent);
              if (args["tag"].count()) {
                std::string tag = args["tag"].as<std::string>();
                if (task.containsTag(tag)) {
                  std::cout << tag << std::endl;
                } else {
                  std::cerr << "Error: invalid tag arguments(s)." << std::endl;
                  return 1;
                }
              } else {
                std::cout << task.json() << std::endl;
              }
            } else {
              std::cerr << "Error: invalid task argument(s)." << std::endl;
              return 1;
            }
          } else {
            std::cout << project.json() << std::endl;
          }
        } else {
          std::cerr << "Error: invalid project argument(s)." << std::endl;
          return 1;
        }
      } else if ((args.count("task") || args.count("tag")) && !args.count("project")) {
        std::cerr << "Error: missing project argument(s)." << std::endl;
        return 1;
      } else {
        std::cout << tlObj.json() << std::endl;
      }
      break;
    }
    case Action::UPDATE: {
      if (args["project"].count() && args["task"].count()) {
        std::string projectIdent = args["project"].as<std::string>();
        
        
        if (projectIdent.find(':') != std::string::npos) {
          std::string oldIdent = projectIdent.substr(0, projectIdent.find(':'));
          std::string newIdent = projectIdent.substr(projectIdent.find(':') + 1);
          if (tlObj.containsProject(oldIdent)) {
            tlObj.getProject(oldIdent).setIdent(newIdent);
            projectIdent = newIdent;
          } else {
            std::cerr << "Project " << oldIdent << " not found." << std::endl;
            return 1;
          }
        }

        if (args["task"].count()) {
          std::string taskIdent = args["task"].as<std::string>();
          if (taskIdent.find(':') != std::string::npos) {
            std::string oldIdent = taskIdent.substr(0, taskIdent.find(':'));
            std::string newIdent = taskIdent.substr(taskIdent.find(':') + 1);
            if (tlObj.getProject(projectIdent).containsTask(oldIdent)) {
              tlObj.getProject(projectIdent).getTask(oldIdent).setIndent(newIdent);
              taskIdent = newIdent;
              
            } else {
              std::cerr << "Task " << oldIdent << " not found." << std::endl;
              return 1;
            }
          }

          if (args.count("completed")) {
              if (tlObj.containsProject(projectIdent) && tlObj.getProject(projectIdent).containsTask(taskIdent)) {
                tlObj.getProject(projectIdent).getTask(taskIdent).setComplete(true);
              } else {
                std::cerr << "Error project or task not found." << std::endl;
                return 1;
              }
          }

          if (args.count("incomplete")) {
              if (tlObj.containsProject(projectIdent) && tlObj.getProject(projectIdent).containsTask(taskIdent)) {
                tlObj.getProject(projectIdent).getTask(taskIdent).setComplete(false);
              } else {
                std::cerr << "Error project or task not found." << std::endl;
                return 1;
              }
          }

          if (args.count("due")) {
            std::string dueDateStr = args["due"].as<std::string>();
            tlObj.getProject(projectIdent).getTask(taskIdent).getDueDate().setDateFromString(dueDateStr);
          }
        }
        tlObj.save(db);
      }
      break;
    }
    case Action::DELETE: {
      if (args.count("project")) {
        std::string projectIdent = args["project"].as<std::string>();
        if (tlObj.containsProject(projectIdent)) {
          if (args.count("task")) {
            std::string taskIdent = args["task"].as<std::string>();
            if (tlObj.getProject(projectIdent).containsTask(taskIdent)) {
              if (args.count("tag")) {
                std::string tag = args["tag"].as<std::string>();
                if (!tlObj.getProject(projectIdent).getTask(taskIdent).deleteTag(tag)) {
                  std::cerr << "Tag " << tag << " not found in task " << taskIdent << " in project " 
                    << projectIdent << std::endl;
                  return 1;
                }
              } else if (args.count("due")) {
                tlObj.getProject(projectIdent).getTask(taskIdent).getDueDate().setInitialised(false);
              } else {
                tlObj.getProject(projectIdent).deleteTask(taskIdent);
              }
            } else {
              std::cerr << "Task " << taskIdent << " not found in Project " << projectIdent << std::endl;    
              return 1;
            }
          } else {
            tlObj.deleteProject(projectIdent);
          }
        } else {
          std::cerr << "Project " << projectIdent << "not found." << std::endl;
          return 1;
        }
        tlObj.save(db);
      }
      
      break;
    }
  }
  return 0;
}

// Create a cxxopts instance. You do not need to modify this function.
//
// Example:
//  auto options = App::cxxoptsSetup();
//  auto args = options.parse(argc, argv);
cxxopts::Options App::cxxoptsSetup() {
  cxxopts::Options cxxopts("371todo", "Student ID: " + STUDENT_NUMBER + "\n");

  cxxopts.add_options()(
      "db", "Filename of the 371todo database",
      cxxopts::value<std::string>()->default_value("database.json"))(

      "action", "Action to take, can be: 'create', 'json', 'update', 'delete'.",
      cxxopts::value<std::string>())(

      "project",
      "Apply action (create, json, update, delete) to a project. If you want to "
      "add a project, set the action argument to 'create' and the project "
      "argument to your chosen project identifier.",
      cxxopts::value<std::string>())(

      "task",
      "Apply action (create, json, update, delete) to a task. If you want to add "
      "a task, set the action argument to 'create', the project argument to your "
      "chosen project identifier and the task argument to the task identifier).",

      cxxopts::value<std::string>())(

      "tag",
      "Apply action (create, json, delete) to a tag.  If you want to add an tag, "
      "set the action argument to 'create', the project argument to your chosen "
      "project identifier, the task argument to your chosen task identifier, and "
      "the tag argument to a single tag 'tag' or comma seperated list of tags: "
      "'tag1,tag2').  If you are simply retrieving a tag through the json action "
      "(and checking that it exists), set the tag argument to the tag name "
      "(e.g. 'example tag'). The action update is unsupported here.",
      cxxopts::value<std::string>())(

      "completed",
      "When creating or updating a task, set the completed flag to change the "
      "task's state to completed. This flag is not compatible in combination "
      "with the incomplete flag.",
      cxxopts::value<bool>())(

      "incomplete",
      "When creating or updating a task, set the incomplete flag to change the "
      "task's state to incomplete. This flag is not compatible in combination "
      "with the completed flag",
      cxxopts::value<bool>())(

      "due",
      "When creating or updating a task, set the due date flag to change the "
      "task's due date to the one specified as an argument (e.g. '2024-11-23')."
      "Ommitting the argument removes the due date from the task.",
      cxxopts::value<std::string>())(

      "h,help", "Print usage.");

  return cxxopts;
}

// TODO Rewrite this function so that it works. This function should
// case-insensitively check the action argument retrieved from cxxopts and
// convert this to a value from the ACTION enum. If an invalid value is given
// in a string, throw an std::invalid_argument exception.
//
// Example:
//  auto options = App::cxxoptsSetup();
//  auto args = options.parse(argc, argv);
//  App::Action action = parseActionArgument(args);
App::Action App::parseActionArgument(cxxopts::ParseResult &args) {
  // enum Action { CREATE, JSON, DELETE, UPDATE };
  std::string input = args["action"].as<std::string>();
  // Convert string to lowercase
  std::transform(input.begin(), input.end(), input.begin(), ::tolower);
  if (input == "create") {
    return Action::CREATE;
  } else if (input == "json") {
    return Action::JSON;
  } else if (input == "delete") {
    return Action::DELETE;
  } else if (input == "update") {
    return Action::UPDATE;
  } else {
    throw std::invalid_argument("action");
  }
}

// TODO Write a function, getJSON, that returns a std::string containing the
// JSON representation of a TodoList object.
//
// This function has been implemented for you, but you may wish to adjust it.
// You will have to uncomment the code, which has been left commented to ensure
// the coursework framework compiles (i.e., it calls functions that you must
// implement, once you have implemented them you may uncomment this function).
//
// Example:
//  TodoList tlObj{};
//  std::cout << getJSON(tlObj);
std::string App::getJSON(TodoList &tlObj) {
  // Only uncomment this once you have implemented the functions used!
  return tlObj.str();
}

// TODO Write a function, getJSON, that returns a std::string containing the
// JSON representation of a specific Project in a TodoList object.
//
// This function has been implemented for you, but you may wish to adjust it.
// You will have to uncomment the code, which has been left commented to ensure
// the coursework framework compiles (i.e., it calls functions that you must
// implement, once you have implemented them you may uncomment this function).
//
// Example:
//  TodoList tlObj{};
//  std::string p = "project argument value";
//  std::cout << getJSON(tlObj, p);
std::string App::getJSON(TodoList &tlObj, const std::string &p) {
  // Only uncomment this once you have implemented the functions used!
  auto pObj = tlObj.getProject(p);
  return pObj.str();
}

// TODO Write a function, getJSON, that returns a std::string containing the
//  JSON representation of a specific Task in a TodoList object.
//
// This function has been implemented for you, but you may wish to adjust it.
// You will have to uncomment the code, which has been left commented to ensure
// the coursework framework compiles (i.e., it calls functions that you must
// implement, once you have implemented them you may uncomment this function).
//
// Example:
//  TodoList tlObj{};
//  std::string p = "project argument value";
//  std::string t = "task argument value";
//  std::cout << getJSON(tlObj, p, t);
std::string App::getJSON(TodoList &tlObj, const std::string &p,
                         const std::string &t) {
  // Only uncomment this once you have implemented the functions used!
  auto pObj = tlObj.getProject(p);
  const auto tObj = pObj.getTask(t);
  return tObj.str();
}

// DONE Write a function, getJSON, that returns a std::string containing the
//  JSON representation of a specific Tag in a TodoList object. If the tag
//  does not exist, return an empty string "".
//
// This function has been implemented for you, but you may wish to adjust it.
// You will have to uncomment the code, which has been left commented to ensure
// the coursework framework compiles (i.e., it calls functions that you must
// implement, once you have implemented them you may uncomment this function).
//
// Example:
//  TodoList tlObj{};
//  std::string p = "project argument value";
//  std::string task = "task argument value";
//  std::string tag = "tag argument value";
//  std::cout << getJSON(tlObj, p, task, tag);
std::string App::getJSON(TodoList &tlObj, const std::string &p,
                         const std::string &task, const std::string &tag) {
  // Only uncomment this once you have implemented the functions used!
  auto pObj = tlObj.getProject(p);
  const auto tObj = pObj.getTask(task);
  if (tObj.containsTag(tag)) {
    return tag;
  } else {
    return "";
  }
}

// A helper function to convert the single string input of tags into 
// a list of tags.
std::vector<std::string> App::splitTags(const std::string& stringTags) {
  std::vector<std::string> tags;
  std::stringstream stringStream(stringTags);
  std::string tag;
  while (std::getline(stringStream, tag, ',')) {
      tags.push_back(tag);
  }
  return tags;
}

