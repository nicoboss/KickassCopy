#include <iostream>

using namespace std;


//http://www.cplusplus.com/forum/general/102786/
bool next(std::string& x)
{
    size_t len = x.size();
    bool overflow = true;
    while(overflow) {
        overflow = false;
        if(--len == 0)
            return false;
        (x[len] == 'z')?(overflow = true, x[len] = 'a'):x[len] += 1;
    }
    return true;
}

std::pair<bool, std::string> brute5()
{
    std::string brute="https://kickass.so/usearch/XXX";//('a', 48);
    do {
        cout << brute << endl;
        //if (hash(brute) == desired_hash)
        //return std::make_pair(true, brute);
    }
    while(next(brute));
    //return std::make_pair(false, std::string());
}


//https://kickass.so/usearch/cds/

int main()
{
    cout << "Hello world!" << endl;
    brute5();
    return 0;
}
