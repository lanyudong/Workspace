#include <iostream>
#include <ctime>

using namespace std;

unsigned int GetUTC () {
    return time(0);
};

int main()
{
    time_t now = time(0);
    int utc = GetUTC();
    cout << "time_t\t" << now << "\nutc\t" << utc << endl;
    return 0;
}
