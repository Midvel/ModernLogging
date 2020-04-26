#include <fmt/format.h>

#include <unistd.h>

#include "MyPreciousContainer.h"

void randomTestAction(MyPreciousContainer* conts)
{
    int n = rand() % 2;
    int ms = rand() % 1000;
    int act = rand() % 2;

    if (act)
    {
        int elem = rand() % 100;
        conts[n].Add(elem);
    }
    else
    {
        conts[n].Pop();
    }
    usleep(ms);
}

int main()
{
    fmt::print("Modern logger\n");
    MyPreciousContainer conts[2];

    for (std::size_t i = 0; i < 20; i++)
        randomTestAction(conts);


    return 0;
}
