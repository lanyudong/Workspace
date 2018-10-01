#include <iostream>
#include <map>
#include <string>

using namespace std;

int main()
{
    map <string , map <string, int> > maps;
    map <string, int> m;
    m["key"] = 10;
    cout << m["key"] << endl;
    m["key"] = m["key"] + 5;
    maps["key"] = m;
    maps["cards"]["debt"] = 4855;
    cout << maps["key"]["key"] << endl;
    cout << maps["cards"]["debt"] << endl;
    return 0;
}
