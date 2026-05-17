#include <algorithm>
#include "functions.h"

namespace Options
{
    using namespace std::string_view_literals;

    constexpr std::array basicList{"Add Student"sv, "Add Grade"sv, "Remove Student"sv, "Edit Student Info"sv,
        "View Students"sv, "More Options"sv, "Exit Class"sv};

    constexpr std::array moreList{"Save Student Grade"sv, "View Student Details"sv,
        "Export All Students"sv, "View Class"sv, "Rename Teacher"sv, "Rename Class"sv};
}

std::string getNameInput(std::string_view statement)
{
    std::cout << statement;

    std::string name{};
    std::getline(std::cin, name);

    return name;
}

const Student* findStudentName(const Group& classA)
{
    const Student* viewStudent{nullptr};

    while (true)
    {
        std::string savedStudent{getNameInput("Enter which student to view specific grades: ")};
        std::transform(savedStudent.begin(), savedStudent.end(), savedStudent.begin(), [](char c) {return std::tolower(c);}
                        );

        viewStudent = classA.findStudent(savedStudent);

        if (!viewStudent)
            std::cout << "Invalid name. Go to 'View Students' to see student names\n";
        else
            return viewStudent;
    }
}

bool exitStatement(std::string_view statement)
{
    const std::string_view prompt{statement.empty() ? "Are you sure you want to exit this class (y / n)?\n" : statement};
    while (true)
    {
        const char input{getInput<char>(prompt)};
        switch (std::tolower(input))
        {
        case 'y': return true;
        case 'n': return false;

        default: std::cout << "Invalid Input!\n";
        }
    }
}

void menu(bool extra, bool editMenu)
{
    if (!extra)
    {
        int i{1};
        for (auto item : Options::basicList)
            std::cout << i++ << ".\t" << item << '\n';
    }
    else if (!editMenu) //automacitcally implies that extra=true
    {
        std::cout << "More Options:\n";
        
        int i{1};
        for (auto item : Options::moreList)
            std::cout << i++ << ".\t" << item << '\n';
    }
    else if (editMenu) //if editMenu is true
    {
        std::cout << "Edit Options:\n";

        std::cout << "1.\tEdit Assignment Grade\n";
        std::cout << "2.\tEdit Assignment Name\n";
    }
}