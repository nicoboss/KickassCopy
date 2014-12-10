#include <iostream>
#include <string>
#include "windows.h"
#include <shellapi.h>
#include <fstream>

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
    fstream fs;
    fs.open ("kickass_for_HTtrack.txt", std::fstream::out);
    std::string brute="http://kickass.so/usearch/aaa";//('a', 48);
    do {
        //ShellExecute(NULL, "open", "document.doc", NULL, NULL, SW_SHOWNORMAL);
        cout << brute << endl;
        //fs << brute << "/" << endl;
        for(int i=5; i<11; i++)
        {
            fs << brute << "/" << i << "/" << endl;
        }
        if(brute=="http://kickass.so/usearch/zzz")
        {
            fs.close();
            exit(0);
        }

        //if (hash(brute) == desired_hash)
        //return std::make_pair(true, brute);
    }
    while(next(brute));
    //return std::make_pair(false, std::string());
    fs.close();
}




//https://kickass.so/usearch/cds/

int main()
{
    //ShellExecute(0, "open", "curl", "https://kickass.so/", 0, SW_SHOWNORMAL);
    //ShellExecute(NULL, "open", "curl.exe", "https://kickass.so/the-good-lie-2014-720p-brrip-x264-yify-t9946721.html", NULL, SW_SHOWNORMAL);
    brute5();
    return 0;
}
