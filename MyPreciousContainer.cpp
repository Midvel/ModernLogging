#include "MyPreciousContainer.h"
#include "MyPreciousLogger.h"

#define FMT_UNICODE 0

// Appenders instances
static plog::MyPreciousFileAppender<plog::MyPreciousFormatter> myPreciousFileAppender;
static plog::MyPreciousConsoleAppender<plog::MyPreciousFormatter> myPreciousConsoleAppender;

//Objects counter
int MyPreciousContainer::m_container_instance = 0;


MyPreciousContainer::MyPreciousContainer() : m_instance_n( MyPreciousContainer::m_container_instance)
{
    //Loggers are created only once
    static bool inited = false;
    if (!inited)
    {
        plog::init<PreciousLogger::PRECIOUS_FILE>(plog::debug, &myPreciousFileAppender);
        plog::init<PreciousLogger::PRECIOUS_CONSOLE>(plog::debug, &myPreciousConsoleAppender);
        inited = true;
    }
    
    MyPreciousContainer::m_container_instance++;
}

void MyPreciousContainer::Add(int&& element)
{
    m_last_added = true;
    ++m_operation;
    m_storage.emplace_back(std::move(element));
    Log();
}

void MyPreciousContainer::Add(const int& element)
{
    m_last_added = true;
    ++m_operation;
    m_storage.push_back(element);
    Log();
}

int MyPreciousContainer::Pop()
{
    if (m_storage.size())
    {
        m_last_added = false;
        ++m_operation;
        int tmp = *(m_storage.end() - 1);
        m_storage.pop_back();
        Log();
        return tmp;
    }
    else
    {
        Log(fmt::format(FMT_STRING("Instance {:d}: Failed to remove element.\n"), this->m_instance_n).c_str());
    }
    return 0;
}

//Just encaplsulation layer
void MyPreciousContainer::Log(const char* mes)
{
    if (mes)
    {
        PLOG_DEBUG_(PreciousLogger::PRECIOUS_FILE) << fmt::format(FMT_STRING("Error: {}"), mes);
        PLOG_DEBUG_(PreciousLogger::PRECIOUS_CONSOLE) << fmt::format(FMT_STRING("Error: {}"), mes );
    }
    else
    {
        PLOG_INFO_(PreciousLogger::PRECIOUS_FILE) << *this;
        PLOG_INFO_(PreciousLogger::PRECIOUS_CONSOLE) << *this;
    }
}

//Container's string representation
std::string MyPreciousContainer::Format() const
{
    fmt::memory_buffer buf1;
    fmt::format_to(buf1, FMT_STRING("Instance : {:d}."), this->m_instance_n);
    fmt::memory_buffer buf2;
    fmt::format_to(buf2, FMT_STRING("Element{:s}was {:s}."),
                this->m_last_added ? fmt::format(FMT_STRING(" {:d} "), *(this->m_storage.end() - 1)) : " ",
                this->m_last_added ? "added" : "deleted" );
    fmt::memory_buffer buf3;
    fmt::format_to(buf3, FMT_STRING("Current size: {:d}. Current operation number: {:d}."), this->m_storage.size(), this->m_operation);

    return fmt::format(FMT_STRING("{} {} {}"), fmt::to_string(buf1), fmt::to_string(buf2), fmt::to_string(buf3));
}