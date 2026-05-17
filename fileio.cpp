#include <fstream>
#include <sstream>
#include <cassert>
#include "functions.h"

void uploadStudentFile(const std::string& filePath, Group& classA)
{
    std::ifstream file{filePath};

    std::string line, studentName{};
    if (file.is_open())
    {
        std::getline(file, line);
        if (line != "student")
        {
            std::cout << "Invalid File Type\n";
            return;
        }

        std::getline(file, studentName);
        classA.addStudent(studentName);

        Student& newStudent{classA[classA.studentNumber() - 1]};

        if (std::getline(file, line))
        {
            std::stringstream getLine{line};
            Fraction extract{};

            newStudent.grades.reserve(static_cast<int>(getLine.str().size() * 0.75)); //3 characters for every space (approximate)
            while (getLine >> extract)
                newStudent.grades.push_back(extract);
        }
        else
        {
            std::cerr << "Error: Could not successfully read file" << std::endl;
            return;
        }

        if (std::getline(file, line))
        {
            std::stringstream getLine{line};
            std::string assignmentName{};

            newStudent.assignmentNames.reserve(newStudent.grades.size());
            while (getLine >> assignmentName)
                newStudent.assignmentNames.push_back(assignmentName);
        }
        else
        {
            std::cerr << "Error: Could not successfully read file" << std::endl;
            return;
        }
        
        file.close();

        assert(newStudent.grades.size() == newStudent.assignmentNames.size() && "Student data did not match");

        std::cout << "Successfully added file with " << newStudent.name << "'s information\nGo to 'View Students' for more information\n";
    }
    else
        std::cerr << "Error: Could not open the given file" << std::endl;
}

void saveStudentFile(const Group& classA)
{
    if (classA.studentNumber() == 0)
    {
        std::cout << "No Students to Save!\n";
        return;
    }

    const Student* foundStudent{findStudentName(classA)};

    std::ofstream sFilePath{getNameInput("Enter the name of the file you want to create: ") + ".txt"};

    if (sFilePath.is_open())
    {
        sFilePath << "student\n";
        sFilePath << foundStudent->name << '\n';

        for (auto grade : foundStudent->grades)
            sFilePath << grade << ' ';
        sFilePath << '\n';
        for (const auto& assignmentName : foundStudent->assignmentNames)
            sFilePath << assignmentName << ' ';
        
        sFilePath.close();
        std::cout << "Successfully created file with " << foundStudent->name << "'s information\n";
    }
    else
        std::cerr << "Error: Could not open new file" << std::endl;
}

void saveClassFile(const Group& classA)
{
    std::ofstream cFilePath{getNameInput("Enter the name of the file you want to create: ") + ".txt"};

    if (cFilePath.is_open())
    {
        cFilePath << "class\n";

        cFilePath << classA.studentNumber() << '\n';
        cFilePath << classA.getTeacherName() << '\n' << classA.getClassName() << '\n';

        for (int i{0}; i < classA.studentNumber(); ++i)
        {
            cFilePath << classA[i].name << '\n';

            for (auto grade : classA[i].grades)
                cFilePath << grade << ' ';
            cFilePath << '\n';
            for (const auto& assignmentName : classA[i].assignmentNames)
                cFilePath << assignmentName << ' ';
            cFilePath << '\n';
        }

        cFilePath.close();
        std::cout << "Successfully created file with " << classA.getTeacherName() << "'s " << classA.getClassName() << " class's information\n";
    }
    else
        std::cerr << "Error: Could not open new file" << std::endl;
}

bool uploadClassFile(Group& classA)
{
    std::ifstream file{getNameInput("Enter the name of the file to upload: ")};

    std::string line{};
    int classSize{};
    if (file.is_open())
    {
        classA.clearInformation();

        std::getline(file, line);
        if (line != "class")
        {
            std::cout << "Invalid File Type\n";
            return false;
        }

        if (std::getline(file, line))
        {
            try
            {
                classSize = std::stoi(line);
            }
            catch (const std::exception& e)
            {
                std::cerr << "Error: Could not successfully read file" << std::endl; //in case file got changed
                return false;
            }
        }
        else
        {
            std::cerr << "Error: Could not successfully read file" << std::endl;
            return false;
        }

        std::array<std::string, 2> classNameInfo{};
        
        for (int i{0}; i < classNameInfo.size(); ++i)
        {
            std::getline(file, line);
            classNameInfo[i] = line;
        }
        classA.getTeacherName() = classNameInfo[0]; //new teacher
        classA.getClassName() = classNameInfo[1]; //new class name

        classA.addClassSpace(classSize);
        for (int i{0}; i < classSize; ++i)
        {
            std::getline(file, line);
            classA[i].name = line;

            if (std::getline(file, line))
            {
                std::stringstream getLine{line};
                Fraction extract{};
                
                while (getLine >> extract)
                    classA[i].grades.push_back(extract);
            }
            else
            {
                std::cerr << "Error: Could not successfully read file" << std::endl;
                return false;
            }

            if (std::getline(file, line))
            {
                std::stringstream getLine{line};
                std::string assignmentName{};
                
                classA[i].assignmentNames.reserve(classA[i].grades.size());
                while (getLine >> assignmentName)
                    classA[i].assignmentNames.push_back(assignmentName);
            }
            else
            {
                std::cerr << "Error: Could not successfully read file" << std::endl;
                return false;
            }

            assert(classA[i].grades.size() == classA[i].assignmentNames.size() && "Student data did not match");
        }

        file.close();
        return true;
    }
    else
    {
        std::cerr << "Error: Could not open the given file" << std::endl;
        return false;
    }
}