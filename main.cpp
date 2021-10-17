#include <iostream>
#include <numeric>
#include <fstream>
#include "Task2.pb.h"

#include <vector>

using namespace std;

namespace Studgroup
{
    class IRepository
    {
    public:
        virtual void Open() = 0; // бинарная десериализация в файл
        virtual void Save() = 0; // бинарная сериализация в файл
        virtual ~IRepository() {};
    };

    class IMethods
    {
    public:
        virtual double GetAverageScore(const Task2::FullName &name) = 0;
        virtual std::string GetAllInfo(const Task2::FullName &name) = 0;
        virtual std::string GetAllInfo() = 0;
        virtual ~IMethods() {};
    };

    class StudentsGroup: public IRepository, public IMethods
    {
    private:
        std::vector<Task2::Student> m_students;
    public:
        void add_student(const Task2::Student &s)
        {
            m_students.push_back(s);
        }

        double GetAverageScore(const Task2::FullName &name)
        {
            auto it = std::find_if(m_students.begin(), m_students.end(),
             [&](const Task2::Student &s)
             {
                return tie(s.name().name(), s.name().surname(), s.name().patronymic()) ==
                tie(name.name(), name.surname(), name.patronymic());
              });

            if (it == m_students.end())
            {
                return 0.0;
            }

            return it->avg_score();
        }

        std::string GetAllInfo(const Task2::FullName &name)
        {
            auto it = std::find_if(m_students.begin(), m_students.end(),
             [&](const Task2::Student &s)
             {
                return tie(s.name().name(), s.name().surname(), s.name().patronymic()) ==
                tie(name.name(), name.surname(), name.patronymic());
             });

            if (it == m_students.end())
            {
                return {};
            }

            std::string temp = "Name: " + it->name().name() + ' ' + it->name().surname() + ' ' + it->name().patronymic() + '\n';
            temp += "Numbers: ";
            for (int i = 0; i < it->grades().size(); ++i)
            {
                temp += std::to_string(it->grades(i)) + " ";
            }
            temp += '\n';
            temp += "Average: " + std::to_string(it->avg_score()) + "\n\n";

            return temp;
        }

        std::string GetAllInfo()
        {
            std::string temp;

            std::for_each(m_students.begin(), m_students.end(), [&](const Task2::Student &s)
            {
                temp += GetAllInfo(s.name());
            });

            return temp;
        }

        void Save()
        {
            std::ofstream out("/home/mike/git/dzS7/task3_st", std::ios_base::binary);
            auto size = m_students.size();
            out.write(reinterpret_cast<char*>(&size), sizeof(size));
            std::for_each(m_students.begin(), m_students.end(), [&](const Task2::Student &s)
            {
                s.SerializeToOstream(&out);
            });
            out.close();
        }

        void Open()
        {
            std::ifstream in("/home/mike/git/dzS7/task3_st", std::ios_base::binary);
            size_t size = 0;
            in.read(reinterpret_cast<char*>(&size), sizeof(size));
            while (size--)
            {
                Task2::Student s;
                s.ParseFromIstream(&in);
                m_students.push_back(std::move(s));
            }
            in.close();
        }


    };
}

int main()
{
 { // --- Task 2
   // 1. Установить библиотеку protobuf.
   // 2. С помощью компилятора protobuf в отдельном пространстве имен сгенерировать классы
   //    a. FullName с полями имя, фамилия, отчество (отчество опционально).
   //    b. Student с полями полное имя, массив оценок, средний балл.
   //    c. StudentsGroup с полем массив студентов.
    cout<<"Task 2"<<endl;
    Task2::FullName ts2;
    ts2.set_name("Van");
    ts2.set_surname("Dan");
    //ts2.set_patronymic("Kirillovich");

    Task2::Student st;
    *st.mutable_name() = ts2;
    st.add_grades(5);
    st.add_grades(5);
    st.add_grades(1);
    st.add_grades(3);
    st.add_grades(4);
    st.add_grades(2);
    st.add_grades(4);
    st.add_grades(5);
    st.set_avg_score(double(std::accumulate(st.grades().begin(), st.grades().end(), 0)) / st.grades().size());

    Task2::StudentsGroup Sg;
    *Sg.add_students() = st;

    std::ofstream out("/home/mike/git/dzS7/task2_st", std::ios_base::binary);
    Sg.SerializeToOstream(&out);
    out.close();

    Task2::StudentsGroup newSg;
    std::ifstream in("/home/mike/git/dzS7/task2_st", std::ios_base::binary);
    if (newSg.ParseFromIstream(&in))
    {
        std::cout << newSg.students(0).name().name()<<" "<<newSg.students(0).name().surname()<<" "<<newSg.students(0).name().patronymic() << std::endl;
        std::cout << newSg.students(0).avg_score() << std::endl;
    }
    else
    {
        std::cout << "Error!" << std::endl;
    }
    in.close();
 }
 {   // --- Task 3
     //    3. Создать класс StudentsGroup (без использования protobuf), который реализует интерфейсы:
     //class IRepository {
     //	virtual void Open() = 0; // бинарная десериализация в файл
     //	virtual void Save() = 0; // бинарная сериализация в файл
     //};
     //class IMethods {
     // virtual double GetAverageScore(const FullName& name) = 0;
     // virtual string GetAllInfo(const FullName& name) = 0;
     // virtual string GetAllInfo() = 0;
     //};
    cout<<endl<<"Task 3"<<endl;
    Task2::FullName ts3;
    ts3.set_name("Van");
    ts3.set_surname("Dan");
    ts3.set_patronymic("Kirillovich");

    Task2::Student st;
    *st.mutable_name() = ts3;
    st.add_grades(5);
    st.add_grades(5);
    st.add_grades(1);
    st.add_grades(3);
    st.add_grades(4);
    st.add_grades(2);
    st.add_grades(4);
    st.add_grades(5);
    st.set_avg_score(double(std::accumulate(st.grades().begin(), st.grades().end(), 0)) / st.grades().size());

    Studgroup::StudentsGroup Sg;
    Sg.add_student(st);
    Sg.Save();

    Studgroup::StudentsGroup newSg;
    newSg.Open();
    std:: cout << newSg.GetAllInfo(ts3) << std::endl;
 }
}
