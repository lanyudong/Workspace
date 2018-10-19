#include <iostream>
#include "include/SunSpecModel.h"

using namespace std;

int main()
{
    SunSpecModel model(1, 2);
    bool match = model == "common";
    cout << match << endl;
    return 0;
}
