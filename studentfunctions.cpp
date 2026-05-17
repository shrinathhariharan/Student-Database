#include <algorithm>
#include <iomanip>
#include "functions.h"

void addIndividualGrades(Student& student)
{
    Fraction input{};
    std::string assignmentInput{};
    bool keepInputting{true};

    do
    {
        std::cout << "Enter the grade in this format -> points given / total points\n";

        std::cout << "Enter the grade: ";
        std::cin >> input;

        student.grades.push_back(input);

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Enter the assignment name for the grade: ";
        std::getline(std::cin, assignmentInput);

        std::replace(assignmentInput.begin(), assignmentInput.end(), ' ', '-');

        student.assignmentNames.push_back(assignmentInput);

        if (exitStatement("Do you want to stop inputting (y / n)?\n"))
            keepInputting = false;
    } while (keepInputting);
}

void addStudents(Group& classA)
{
    if (exitStatement("Do you have a student file to upload (y / n)?\n"))
    {
        std::string filePath{getNameInput("Enter the file name: ")};
        uploadStudentFile(filePath, classA);
        return;
    }

    std::string newStudentName{};
    while (true)
    {
        newStudentName = getNameInput("Enter the student's name (q to exit): ");

        if (newStudentName == "q" || newStudentName == "Q")
            return;
        
        if (!classA.findStudent(newStudentName))
            break;
        std::cout << "Student already exists!\n";
    }
    classA.addStudent(newStudentName);
    
    if (exitStatement("Do you have grades to input for the student (y / n)?\n"))
        addIndividualGrades(classA[classA.studentNumber() - 1]);
}

void viewStudents(const Group& classA)
{
    if (classA.studentNumber() == 0)
    {
        std::cout << "No Students to View!\n";
        return;
    }

    std::cout << "#\tName\tAverage\n";
    for (int i{0}; i < classA.studentNumber(); ++i)
    {
        const std::optional<double> average{classA[i].getAverage()};
        std::cout << i + 1 << ".\t" << classA[i].name << '\t';
        if (average)
            std::cout << std::fixed << std::setprecision(2) << *average * 100 << "% (" << getLetterGrade(*average * 100) << std::defaultfloat << ")\n";
        else
            std::cout << "NAN% (NAN)\n";
    }
    const std::optional classAverage{classA.getAverage()};

    std::cout << "Class average: ";
    if (classAverage)
        std::cout << *classAverage * 100 << "% (" << getLetterGrade(*classAverage * 100) << ")\n";
    else
        std::cout << "NAN% (NAN)\n";
}

void removeStudent(Group& classA)
{
    if (classA.studentNumber() == 0)
    {
        std::cout << "No Students to Delete!\n";
        return;
    }

    const Student* foundStudent{findStudentName(classA)};

    if (!exitStatement("Are you sure you want to remove this student (y / n)?"))
        return;

    if (classA.deleteStudent(foundStudent->name))
        std::cout << "Successfully removed " << foundStudent->name << '\n';
    else
        std::cout << "Error: Could not successfully remove student\n";
}

void addGrade(Group& classA)
{
    Student* addGradeStudent{nullptr};

    while (true)
    {   
        std::string savedStudent{getNameInput("Enter which student to add a grade to: ")};
        addGradeStudent = classA.findStudent(savedStudent);

        if (!addGradeStudent)
            std::cout << "Invalid name. Go to 'View Students' to see student names\n";
        else
            break;
    }

    addIndividualGrades(*addGradeStudent);
}

void viewStudentDetails(const Group& classA)
{
    if (classA.studentNumber() == 0)
    {
        std::cout << "No Students to View!\n";
        return;
    }

    const Student* viewStudent{findStudentName(classA)};

    std::cout << "Here are the past assignments:\n";
    for (int i{0}; i < viewStudent->assignmentNames.size(); ++i)
    {
        const Fraction f{viewStudent->grades[i]};
        std::cout << std::left << std::setw(15) << viewStudent->assignmentNames[i] << std::right << std::setw(8) << f << 
            std::right << std::setw(10) << std::fixed << std::setprecision(2) << static_cast<double>(f) * 100 << std::defaultfloat << 
            '%' << std::right << std::setw(6) << '(' << getLetterGrade(static_cast<double>(f) * 100) << ")\n";
    }
    
    const std::optional<double> average{viewStudent->getAverage()};
    std::cout << "Total Points: " << viewStudent->getTotalPoints() << '\n';
    std::cout << viewStudent->name << "'s grade: ";
    if (average)
        std::cout << *average * 100 << "% (" << getLetterGrade(*average * 100) << ")\n";
    else
        std::cout << "NAN% (NAN)\n";
}

void editStudentInfo(Group& classA)
{
    enum EditOptions {edit_grade, edit_name, max_options};

    if (classA.studentNumber() == 0)
    {
        std::cout << "No Students to Edit\n";
        return;
    }

    Student* editStudent{nullptr};
    std::optional<int> foundAssignment{std::nullopt};

    while (true)
    {   
        std::string savedStudent{getNameInput("Enter which student to edit: ")};
        editStudent = classA.findStudent(savedStudent);

        if (!editStudent)
            std::cout << "Invalid name. Go to 'View Students' to see student names\n";
        else
            break;
    }

    while (true)
    {
        std::string savedAssignmentName{getNameInput("Enter the assignment name to find the item to edit: ")};

        foundAssignment = classA.findAssignment(editStudent, savedAssignmentName);

        if (foundAssignment)
            break;
        else
            std::cout << "Invalid Assignment Name! Select 'More Options' and look at " << editStudent->name << "'s information to find the desired assignment\n";
    }

    std::cout << editStudent->name << "'s assignment: " << editStudent->assignmentNames[*foundAssignment] << "\tGrade: " << editStudent->grades[*foundAssignment] << '\n';
    menu(true, true);

    EditOptions editOption{};
    while (true)
    {
        editOption = static_cast<EditOptions>(getInput<int>("Choose an option: ") - 1);
        if (editOption >= max_options || editOption < 0)
        {
            std::cout << "Invalid Input!\n";
            continue;
        }
        
        break;
    }

    switch (editOption)
    {
    case edit_name:
    {
        std::cout << "What do you want to rename " << editStudent->assignmentNames[*foundAssignment] << " to?\n";
        std::string newAssignmentName{getNameInput("")};
        std::replace(newAssignmentName.begin(), newAssignmentName.end(), ' ', '-');

        editStudent->assignmentNames[*foundAssignment] = newAssignmentName;

        break;
    }
    case edit_grade:
    {
        std::cout << "What do you want to change the grade " << editStudent->grades[*foundAssignment] << " to?\n";

        Fraction newGrade{};
        std::cin >> newGrade;

        editStudent->grades[*foundAssignment] = newGrade;

        break;
    }
    }
}

std::optional<bool> deleteClass(std::vector<Group>& classesTeaching)
{
    if (classesTeaching.empty())
        return std::nullopt;
    

    std::string classDeleting{getNameInput("Type Class to be Deleted: ")};
    auto it{std::find_if(classesTeaching.begin(), classesTeaching.end(), [&classDeleting](const Group& g){
        return g.getClassName() == classDeleting;
    })};

    if (it == classesTeaching.end())
        return false;
    
    classesTeaching.erase(it);

    return true;
}