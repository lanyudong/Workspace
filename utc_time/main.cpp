#include <iostream>
#include <chrono>
#include <ctime>

using namespace std;

int main()
{
    time_t time_a = time(nullptr);
    unsigned int a = time_a;

    time_t time_b = time(&);

    cout << time_a << " , " << a << endl;
    cout << time_b << endl;
    return 0;
}
