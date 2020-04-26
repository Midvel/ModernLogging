#pragma once

#include <vector>
#include <string>

class MyPreciousContainer
{
 public:
    MyPreciousContainer();

    void Add(int&& element);

    void Add(const int& element);
    
    int Pop();

    inline int Size() const { return m_storage.size(); }

    //Secret method to get formated string representation
    std::string Format() const;
private:
    std::vector<int> m_storage;
    //Current object number
    int m_instance_n;
    
    //Fo logging purposes
    bool m_last_added = false;
    int m_operation = 0;

    void Log(const char* mes = nullptr);
    static int m_container_instance;
};

