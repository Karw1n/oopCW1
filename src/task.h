// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2023/24)
// Department of Computer Science, Swansea University
//
// Author: <STUDENT NUMBER>
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
        std::string identifier;
        TagContainer tags;
        Date dueDate;
        bool complete;

    public:
        Task(const std::string& identifier);
        std::string getIdent() const;
        const TagContainer &getTags() const noexcept;
        // TODO might need to make other functions const
        void setIndent(std::string& identifier);
        bool addTag(std::string tag);
        bool deleteTag(std::string tag);
        unsigned int numTags() const;
        bool containsTag(std::string tag) const;
        Date getDueDate() const;
        void setDueDate(Date date);
        void setComplete(bool aBool);
        bool isComplete() const;
        bool operator==(Task& task) const;
        std::string str() const;

        //std::string Task::getTagAt(unsigned int index) const;

};

#endif // TASK_H
