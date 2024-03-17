// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2023/24)
// Department of Computer Science, Swansea University
//
// Author: <STUDENT NUMBER>
//
// Canvas: https://canvas.swansea.ac.uk/courses/44636
// -----------------------------------------------------
#include "todolist.h"

// TODO Write a TodoList constructor that takes no parameters and constructs an
//  empty todolist.
//
// Example:
//  TodoList tObj{};
TodoList::TodoList() {}

// TODO Write a function, size, that takes no parameters and returns an unsigned
//  int of the number of projects the TodoList contains.
//
// Example:
//  TodoList tObj{};
//  auto size = tObj.size();
unsigned int TodoList::size() const noexcept {
    return this->projects.size();
}


// TODO Write a function, newProject, that takes one parameter, a project
//  identifier, and returns the Project object as a reference. If an object
//  with the same identifier already exists, then the existing object should be
//  returned. Throw a std::runtime_error if the Project object cannot be
//  inserted into the container.
//
// Example:
//  TodoList tObj{};
//  tObj.newProject("projectIdent");
Project & TodoList::newProject(const std::string &tIdent) {
    for (auto& project : this->projects) {
        if (project.getIdent() == tIdent) {
            return project;
        }
    }

    try {
        projects.push_back(Project(tIdent));
    } catch(const std::runtime_error& e)
    {
        throw e;
    }
    return projects.back();
}

// TODO Write a function, addProject, that takes one parameter, a Project
//  object, and returns true if the object was successfully inserted. If an
//  object with the same identifier already exists, then the contents should be
//  merged (see also Project::addTask) and then return false. Throw a
//  std::runtime_error if the Project object cannot be inserted into the
//  container for whatever reason.
//
// Example:
//  TodoList tObj{};
//  Project cObj{"projectIdent"};
//  tObj.addProject(cObj);
bool TodoList::addProject(Project project) {
    for (Project& aProject : this->projects) {
        if (aProject.getIdent() == project.getIdent()) {
            for (const auto& task : project.getTasks()) {
                if (!aProject.containsTask(task.getIdent())) {
                    aProject.addTask(task);
                }
            }
            return false;
        }
    }
    
    try {
        this->projects.push_back(project);
    } catch(const std::runtime_error& e) {
        throw e;
    }
    return true;
}

// A function, containsProject, that takes one parameter, a Project 
// indentifier and returns a bool on whether the project is in the TodoList
bool TodoList::containsProject(const std::string &tIdent) const noexcept {
    for (auto it = this->projects.begin(); it != this->projects.end(); it++) {
        Project project = *it;
        if (project.getIdent() == tIdent) {
            return true;
        }
    }
    return false;
}

// TODO Write a function, getProject, that takes one parameter, a Project
//  identifier and returns the Project. If no Project exists, throw an
//  appropriate exception.
//
// Example:
//  TodoList tObj{};
//  tObj.newProject("projectIdent");
//  auto cObj = tObj.getProject("projectIdent");
Project& TodoList::getProject(const std::string &tIdent) {
    if (this->projects.empty()) {
        throw std::out_of_range("Project list is empty");
    }

    for (auto it = this->projects.begin(); it != this->projects.end(); it++) {
        Project& project = *it;
        if (project.getIdent() == tIdent) {
            return project;
        }
    }
    throw std::out_of_range("Project " + tIdent + " not found");
}


const ProjectContainer& TodoList::getProjects() const noexcept {
    return projects;
}


// TODO Write a function, deleteProject, that takes one parameter, a Project
//  identifier, and deletes it from the container, and returns true if the
//  Project was deleted. If no Project exists, throw an appropriate exception.
//
// Example:
//  TodoList tObj{};
//  tObj.newProject("projectIdent");
//  tObj.deleteProject("projectIdent");
bool TodoList::deleteProject(const std::string &tIdent) {
    for (auto it = this->projects.begin(); it != this->projects.end(); it++) {
        Project& project = *it;
        if (project.getIdent() == tIdent) {
            this->projects.erase(it);
            return true;
        }
    }
    throw std::out_of_range("Project not found");
}



// TODO Write a function, load, that takes one parameter, a std::string,
//  containing the filename for the database. Open the file, read the contents,
//  and populates the container for this TodoList. If the file does open throw
//  an appropriate exception (either std::runtime_error or a derived class).
//
// A note on clashes:
//  If you encounter two projects with the same key, the projects should be
//  merged (not replaced!). If you encounter two tasks with the same identifier
//  in the same project, only the tags of the tasks should be merged; it is undefined
//  as to which values are picked for the completed status and due date (pick either).
//  Two tasks in different projects can have the same key.
//
// JSON formatting:
//  The JSON file has the following format (see the sample database.json file
//  also provided with the coursework framework):
// {
//     "Project 1" : {
//         "Task 1":  {
//             "completed": true,
//             "due": "2024-11-23",
//             "tags" : ["uni",
//                       "c++",
//                       "programming",
//                       "standard library"
//                       ...
//                      ]
//             ...
//         },
//         "Task 2":  {
//             "completed": false,
//             "tags" : ["uni",
//                       "c++",
//                       "programming",
//                       "inheritance"
//                       ...
//                      ]
//         },
//         ...
//     },
//     "Project 2": {
//         "Task 1": {
//             "completed": true
//         }
//         ...
//     }
// }
//
// More help:
//  To help you with this function, I've selected the nlohmann::json
//  library that you must use for your coursework. Read up on how to use it
//  here: https://github.com/nlohmann/json. You may not use any other external
//  library other than the one I have provided. You may choose to process the
//  JSON yourself without the help of the library but I guarantee this will be
//  more work.
//
//  Understanding how to use external libraries is part of this coursework! You
//  will need to use this file to deserialize the JSON from string
//  to the JSON object provided by this library. Don't just look at the code
//  provided below, or in the README on the GitHub and despair. Google search,
//  look around, try code out in a separate file to all figure out how to use
//  this library.
//
//  Once you have deserialized the JSON string into an object, you will need to
//  loop through this object, constructing Project and Task objects according
//  to the JSON data in the file.
//
// Example:
//  TodoList tObj{};
//  tObj.load("database.json");
void TodoList::load(const std::string& fileName) {
    // open the JSON file
        std::ifstream file(fileName);
        if (!file.is_open()) {
            throw std::runtime_error(fileName + " failed to open!");
        }

        nlohmann::json data = json::parse(file);

        for (auto projectIt = data.begin(); projectIt != data.end(); projectIt++) {
            std::string projectIdent = projectIt.key();
            
            if (!containsProject(projectIdent)) {
                Project newProject = Project(projectIdent);
                for (auto taskIt = projectIt.value().begin(); taskIt != projectIt.value().end(); taskIt++) {
                    std::string taskIdent = taskIt.key();
                    nlohmann::json taskMembers = taskIt.value();
                    
                    if (!newProject.containsTask(taskIdent)) {
                        Task newTask = Task(taskIdent);
                        newTask.setComplete(taskMembers["completed"]);
                        std::string dueDate = taskMembers.value("dueDate", "0000-00-00");
                        Date date = Date();
                        if (!(dueDate == "0000-00-00")) {
                            date.setDateFromString(dueDate);
                            newTask.setDueDate(date);
                        }
                        
                        for (const auto& tag : taskMembers["tags"]) {
                            newTask.addTag(tag);
                        }

                        newProject.addTask(newTask);
                    } else { // Merge task
                        TaskContainer::iterator it = newProject.findTask(taskIdent);
                        Task& existingTask = *it;
                        for (const auto& tag : taskMembers["tags"]) {        
                            existingTask.addTag(tag);
                        }
                    }
                }
                this->addProject(newProject);
            } else { // Merge Projects
                Project& existingProject = getProject(projectIdent);
                for (auto taskIt = projectIt.value().begin(); taskIt != projectIt.value().end(); taskIt++) {
                    std::string taskIdent = taskIt.key();
                    nlohmann::json taskMembers = taskIt.value();
                    Task newTask = Task(taskIdent);
                    std::string dueDate = taskMembers.value("dueDate", "0000-00-00");
                    Date date = Date();
                        if (!(dueDate == "0000-00-00")) {
                            date.setDateFromString(dueDate);
                        }
                    newTask.setDueDate(date);
                    for (const auto& tag : taskMembers["tags"]) {
                        newTask.addTag(tag);
                    }
                    existingProject.addTask(newTask);
                }
            }
        }
}



// TODO Write a function, save, that takes one parameter, the path of the file
//  to write the database to. The function should serialise the TodoList object
//  as JSON.
//
// Example:
//  TodoList tObj{};
//  tObj.load("database.json");
//  ...
//  tObj.save("database.json");
void TodoList::save(const std::string& fileName) {
 // open the JSON file
    std::ofstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error(fileName + " failed to open!");
    }

    nlohmann::json projectsJson;
    for (Project& project : projects) {
        nlohmann::json projectJson;  
        for (const Task& task : project.getTasks()) {
            projectJson[task.getIdent()] = task.json();
        }
        projectsJson[project.getIdent()] = project.json();

    }

    file << projectsJson << std::endl;
    file.close();
    
}

// TODO Write an == operator overload for the TodoList class, such that two
//  TodoList objects are equal only if they have the exact same data.
//
// Example:
//  TodoList tObj1{};
//  TodoList tObj2{};
//  if(tObj1 == tObj2) {
//    ...
//  }
bool operator==(const TodoList &c1, const TodoList &c2) {
    if (c1.size() != c2.size()) {
        return false;
    }

    for (const Project& project : c1.getProjects()) {
        if (!(c2.containsProject(project.getIdent()))) {
            return false;
        }
    }

    for (const Project& project : c2.getProjects()) {
        if (!(c1.containsProject(project.getIdent()))) {
            return false;
        }
    }


    return true;
} 


// TODO Write a function, str, that takes no parameters and returns a
//  std::string of the JSON representation of the data in the TodoList.
//
// Hint:
//  See the coursework specification for how this JSON should look.
//
// Example:
//  TodoList tObj{};
//  std::string s = tObj.str();
std::string TodoList::str() const {
    std::string stringProjects = "{";
    for (auto it = this->projects.begin(); it != this->projects.end(); it++) {
        Project project = *it;
        stringProjects += project.str();
        if ((it + 1) != this->projects.end()) {
            stringProjects += ",";
        } else {
            stringProjects += "}";
        }
    }

    std::stringstream sttr;    
    sttr << stringProjects;
    
    return sttr.str();
}

//
nlohmann::json TodoList::json() const {
    nlohmann::json todoListJson;
    for (const Project& project : projects) {
        todoListJson[project.getIdent()] = project.json();
    }
    return todoListJson;
}