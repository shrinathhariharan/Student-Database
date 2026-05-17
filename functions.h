#ifndef HELPERF_H
#define HELPERF_H

#include <iostream>
#include <string>
#include <string_view>
#include <limits>
#include <vector>
#include <optional>
#include <array>
#include <numeric>
#include <algorithm>
#include <cstdlib>

struct Fraction
{
    double numerator{};
    double denominator{1};

    operator double() const {return static_cast<double>(numerator) / static_cast<double>(denominator);}
};

inline std::ostream& operator<<(std::ostream& out, Fraction f)
{
    out << f.numerator << '/' << f.denominator;

    return out;
}

inline std::istream& operator>>(std::istream& in, Fraction& f)
{
    char slash{};
    Fraction input{};

    if (in >> input.numerator >> slash >> input.denominator)
    {
        if (slash == '/' && input.denominator > 0 && input.numerator >= 0)
        {
            f.numerator = input.numerator;
            f.denominator = input.denominator;
        }
        else
            in.setstate(std::ios::failbit);
    }
    else
        in.setstate(std::ios::failbit);
    
    return in;
}

struct Student
{
    std::string name{};
    std::vector<Fraction> grades{};
    std::vector<std::string> assignmentNames{};

    std::optional<double> getAverage() const
    {
        if (grades.empty())
            return std::nullopt;

        return static_cast<double>(std::accumulate(grades.begin(), grades.end(), Fraction{0, 0}));
    }

    Fraction getTotalPoints() const {return std::accumulate(grades.begin(), grades.end(), Fraction{0, 0});}
};

inline Fraction operator+(Fraction a, Fraction b)
{
    return Fraction{a.numerator + b.numerator, a.denominator + b.denominator};
}

inline bool operator==(std::string_view studentA, const Student& b) {return studentA == b.name;}
inline bool operator==(const Student& a, std::string_view studentB) {return studentB == a.name;}

class Group
{
    std::string m_teacherName{};
    std::string m_className{};
    std::vector<Student> m_students{};
public:
    Group(std::string_view name, std::string_view className) : m_teacherName{name}, m_className{className}
    {
    }
    Group() = default;

    const std::string& getTeacherName() const {return m_teacherName;}
    std::string& getTeacherName() {return m_teacherName;}

    const std::string& getClassName() const {return m_className;}
    std::string& getClassName() {return m_className;}

    void addStudent(std::string&& studentName) {m_students.push_back(Student{std::move(studentName)});}
    void addStudent(const std::string& studentName) {m_students.push_back(Student{studentName});}

    void addClassSpace(int amount) {m_students.resize(amount);}

    void clearInformation()
    {
        m_students.clear();
        m_teacherName.clear();
        m_className.clear();
    }

    bool deleteStudent(std::string_view studentName)
    {
        auto it{std::find(m_students.begin(), m_students.end(), studentName)};
        if (it != m_students.end())
        {
            m_students.erase(it);
            return true;
        }

        return false;
    }

    std::size_t studentNumber() const {return m_students.size();}

    Student* findStudent(std::string_view studentName)
    {
        std::string lowerSearch{studentName};
        std::transform(lowerSearch.begin(), lowerSearch.end(), lowerSearch.begin(),
                        [](char c){return std::tolower(c); });

        auto it{std::find_if(m_students.begin(), m_students.end(), [&](const Student& s)
        {
            std::string lowerName{s.name};
            std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), [](char c) {return std::tolower(c);} );

            return lowerName == lowerSearch;
        })};

        if (it != m_students.end())
            return &(*it);

        return nullptr;
    }

    const Student* findStudent(std::string_view studentName) const
    {
        std::string lowerSearch{studentName};
        std::transform(lowerSearch.begin(), lowerSearch.end(), lowerSearch.begin(),
                        [](char c){return std::tolower(c); });

        auto it{std::find_if(m_students.begin(), m_students.end(), [&](const Student& s)
        {
            std::string lowerName{s.name};
            std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), [](char c) {return std::tolower(c);} );

            return lowerName == lowerSearch;
        })};

        if (it != m_students.end())
            return &(*it);

        return nullptr;
    }

    void sortStudents() 
    {
        std::sort(m_students.begin(), m_students.end(), [](const Student& a, const Student& b){
            if (!a.getAverage())
                return false;
            if (!b.getAverage())
                return true;

            return *a.getAverage() > *b.getAverage();
        });
    }

    std::optional<int> findAssignment(const Student* s, std::string_view assignmentName)
    {
        if (!s)
            return std::nullopt;

        auto it{std::find(s->assignmentNames.begin(), s->assignmentNames.end(), assignmentName)};

        if (it == s->assignmentNames.end())
            return std::nullopt;

        const int idx{static_cast<int>(std::distance(s->assignmentNames.begin(), it))};

        return idx;
    }

    std::optional<double> getAverage() const 
    {
        if (m_students.empty())
            return std::nullopt;

        int studentNum{};

        double average {std::accumulate(m_students.begin(), m_students.end(), 0.0,
        [&studentNum](double acc, const Student& s) {
            if (!s.getAverage())
                return acc;

            ++studentNum;
            return acc + static_cast<double>(*s.getAverage());
        })};

        average /= studentNum;

        return average;
    }

    Student& operator[](std::size_t idx) {return m_students[idx];}
    const Student& operator[](std::size_t idx) const {return m_students[idx];}
};

namespace Options
{
    using namespace std::string_view_literals;

    enum class Basic
    {
        add_student,
        add_grade,
        remove_student,
        edit_student_info,
        view_students,
        more_options,
        exit_class,
        max_options
    };

    enum class More
    {
        save_student_grade,
        view_student_details,
        export_all_students,
        view_class, //just to see the name of the class (e.g. History)
        rename_teacher,
        rename_class,
        max_options
    };
}

void addIndividualGrades(Student& student);

void addStudents(Group& classA);

void viewStudents(const Group& classA);

void removeStudent(Group& classA);

void addGrade(Group& classA);

void viewStudentDetails(const Group& classA);

void editStudentInfo(Group& classA);

std::optional<bool> deleteClass(std::vector<Group>& classesTeaching);

template <typename T>
T getInput(std::string_view statement="")
{
    std::cout << statement;
    
    T input{};

    while (!(std::cin >> input))
    {
        std::cin.clear();
        std::cout << "Invalid Input!\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return input;
}

constexpr char getLetterGrade(double percent)
{
    if (percent < 60)
        return 'F';
    else if (percent < 70)
        return 'D';
    else if (percent < 80)
        return 'C';
    else if (percent < 90)
        return 'B';
    else
        return 'A';
}

std::string getNameInput(std::string_view statement);

const Student* findStudentName(const Group& classA);

bool exitStatement(std::string_view statement="");

void menu(bool extra, bool editMenu=false);

void uploadStudentFile(const std::string& filePath, Group& classA);

void saveStudentFile(const Group& classA);

void saveClassFile(const Group& classA);

bool uploadClassFile(Group& classA);



#endif