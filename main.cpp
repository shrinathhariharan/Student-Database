#include "functions.h"

int main()
{
    std::cout << "Hello! This is a student database to keep track of all your students' grades and information!\n";
    std::vector<Group> classesTeaching{};
    bool keepRunning{true};

    constexpr std::array<std::string_view, 3> menuOptions{"Import Class File", "Delete Class", "Quit Program"};

    while (keepRunning)
    {
        std::cout << "1.\tAdd New Class\n";
        for (int i{0}; i < classesTeaching.size(); ++i)
            std::cout << i + 2 << ".\t" << "Open "<< classesTeaching[i].getClassName() << " Class (" << classesTeaching[i].studentNumber() << (classesTeaching[i].studentNumber() == 1 ? " student" : " students") << ")\n"; //so first option is 2 (1 is already there)
        for (int i{0}; i < menuOptions.size(); ++i)
            std::cout << i + classesTeaching.size() + 2 << ".\t" << menuOptions[i] << '\n';
        
        Group* classA{nullptr};

        const int option{getInput<int>("Choose an option: ")};

        if (option == 1)
        {
            classesTeaching.push_back({getNameInput("Enter teacher name:\n"), getNameInput("Enter class teaching:\n")});
            continue;
        }
        else if (option > 1 && option <= classesTeaching.size() + 1)
            classA = &classesTeaching[option - 2];
        else if (option == classesTeaching.size() + 2)
        {
            const bool addFile{exitStatement("Are you sure you want to import a new file (y / n)?\n")};
            if (!addFile)
                continue;

            Group temp{};
            if (uploadClassFile(temp))
                classesTeaching.push_back(temp);

            continue;
        }
        else if (option == classesTeaching.size() + 3)
        {
            const std::optional<bool> deletedClass{deleteClass(classesTeaching)};

            if (!deletedClass)
                std::cout << "No Classes to Delete!\n";
            else
                std::cout << (*deletedClass ? "Class Successfully Deleted\n" : "Invalid Class Name\nGo to MENU to see classes\n");

            continue;
        }
        else if (option == classesTeaching.size() + 4)
        {
            keepRunning = !exitStatement("Do you want to exit the program (y / n)?\n");
            continue;
        }
        else
        {
            std::cout << "Invalid Option!\n";
            continue;
        }
        
        bool quitClass{false};
        while (!quitClass)
        {
            std::cout << "\n\nHello, " << classA->getTeacherName() << "\n\n";

            menu(false);
            const int choice{(getInput<int>("Choose an option: ") - 1)};

            if (choice > static_cast<int>(Options::Basic::max_options))
            {
                std::cout << "Invalid Input!\n";
                continue;
            }
            switch (static_cast<Options::Basic>(choice))
            {
            case Options::Basic::add_student:
                addStudents(*classA);
                break;
            case Options::Basic::add_grade:
                addGrade(*classA);
                break;
            case Options::Basic::remove_student:
                removeStudent(*classA);
                break;
            case Options::Basic::view_students:
                classA->sortStudents();
                viewStudents(*classA);
                break;
            case Options::Basic::edit_student_info:
                editStudentInfo(*classA);
                break;
            case Options::Basic::more_options:
            {
                menu(true);
                const int moreChoice{getInput<int>("Choose an option: ") - 1};

                if (moreChoice > static_cast<int>(Options::More::max_options))
                {
                    std::cout << "Invalid Input!\n";
                    continue;
                }
                switch (static_cast<Options::More>(moreChoice))
                {
                case Options::More::save_student_grade:
                    saveStudentFile(*classA);
                    break;
                case Options::More::view_class:
                    std::cout << "You are currently in your " << classA->getClassName() << " class\n";
                    break;
                case Options::More::export_all_students:
                    saveClassFile(*classA);
                    break;
                case Options::More::view_student_details:
                    viewStudentDetails(*classA);
                    break;
                case Options::More::rename_class:
                    classA->getClassName() = getNameInput("Enter new class name: ");
                    break;
                case Options::More::rename_teacher:
                    classA->getTeacherName() = getNameInput("Enter new teacher name: ");
                    break;
                default: std::cout << "Invalid Input!\n";
                }
                break;
            }
            case Options::Basic::exit_class:
                quitClass = exitStatement();
                break;
            default: std::cout << "Invalid Input!\n";
            }
        }
    }
}