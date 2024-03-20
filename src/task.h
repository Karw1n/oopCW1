// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2023/24)
// Department of Computer Science, Swansea University
//
// Author: <2142479>
//
// Canvas: https://canvas.swansea.ac.uk/courses/44636
// -----------------------------------------------------
// A Task class contains multiple 'tags' (e.g., a tag might be 'home' and
// another tag could be 'uni'). A Task also has a identifier (e.g. 'Complete
// Coursework') a completed status (completed or not) and due date. An
// uninitialised due date (see date.cpp/date.h) means the task does not have a
// due date.
// -----------------------------------------------------

#ifndef TASK_H
#define TASK_H
#include "date.h"

using TagContainer = std::vector<std::string>;

class Task {
    private:
        std::string ident;
        TagContainer tags;
        Date dueDate;
        bool complete;

    public:
        explicit Task(const std::string& identifier);
        ~Task() = default;
        
        const std::string getIdent() const noexcept;
        const TagContainer &getTags() const noexcept;
        void setIndent(std::string& identifier);
        bool addTag(std::string tag);
        bool deleteTag(std::string tag);
        const unsigned int numTags() const noexcept;
        bool containsTag(std::string tag) const;
        Date getDueDate() const noexcept;
        void setDueDate(Date date);
        void setComplete(bool aBool);
        const bool isComplete() const noexcept;
        friend bool operator==(const Task& task1, const Task& task2);
        std::string str() const;
        nlohmann::json json() const;

};

#endif // TASK_H
