#include <iostream>
#include <string>
#include <vector>
#include "windows.h"
#include <shellapi.h>
#include <fstream>

using namespace std;

bool Read_MaxPagesFile(void);
void brute5(void);
bool next(string& x);


vector<unsigned int> MaxPages;
vector<string> MaxPages_Organisation;
unsigned long MaxPages_length=0;
bool MaxPages_out_of_range=false;


//https://kickass.so/usearch/cds/

int main()
{
    Read_MaxPagesFile();
    //ShellExecute(0, "open", "curl", "https://kickass.so/", 0, SW_SHOWNORMAL);
    //ShellExecute(NULL, "open", "curl.exe", "https://kickass.so/the-good-lie-2014-720p-brrip-x264-yify-t9946721.html", NULL, SW_SHOWNORMAL);
    brute5();

    /*
    Bug Fixed!
    if(MaxPages_out_of_range==true and MaxPages_length>0)
    {
        cout << "Warning:\nThe length of MaxPages has exceeded and gave an an out_of_range error.\n"
             << "For the missing values was used the defult value of 20 pages. Are you sure,\n"
             << "that you have analyced every file? Maybe the order is also incorrect.\n"
             << "In this case it is beather to delete the MaxPages.txt\n"
             << "file and take for every value the defult value.\n"
             << "But the best thing is stil a working MaxPages.txt file." << endl;
    }
    */

    cout << "\nDone!" << endl;
    cout << "Press ENTER to close the program" << endl;
    cin.get();
    return 0;
}



bool Read_MaxPagesFile(void)
{
    char line[8];
    string line_string;
    ifstream infile ("MaxPages.txt", ios::in);
    if (!infile)
    {
        MaxPages_length=0;
        cout << "Warning: MaxPages.txt does not exist!\nAre you sur that you had created one with MaxPagesScan and\ncopyed it into this programm Folder?\n" <<
        "For this progress the defult value of 20 pages will be tacken for every\ncombination, but you'll have a lot of dublications and missing Links!\n" << endl;
        return false;
    }

    while (infile.getline(line, 8))
    {
        line_string=line;
        //cout << line_string.substr(0,3) << endl;
        //cout << line_string.substr(4,line_string.length()-4) << endl;
        MaxPages_Organisation.push_back(line_string.substr(0,3));
        MaxPages.push_back(atoi(line_string.substr(4).c_str()));
        //cout << line << endl;
    }
    MaxPages_length=MaxPages.size();
    return true;
}


void brute5(void)
{
    unsigned long Pos=0;
    int i_max;
    fstream fs;
    fs.open ("kickass_for_HTtrack.txt", fstream::out);
    string brute="http://kickass.so/usearch/aaa";//('a', 48);
    do {
        //ShellExecute(NULL, "open", "document.doc", NULL, NULL, SW_SHOWNORMAL);
        //if(brute[brute.length()-1]<'a' or brute[brute.length()-2]<'a' or brute[brute.length()-3]<'a') Nubers in conbination only mod
        {
            //cout << brute << endl; //endl is better because Buffer Overvlow but slower than << "\n"! The Best thinng is to don't show anything!
            //cout << MaxPages_Organisation.at(Pos) << endl;
            //cout << brute.substr(brute.length()-3) << endl;
            //cin.get();
            if(MaxPages_length>Pos and MaxPages_Organisation.at(Pos)==brute.substr(brute.length()-3))
            {
                i_max=MaxPages.at(Pos);
                //fs << brute << "/" << endl;
                for(int i=1; i<=i_max; i++)
                {
                    fs << brute << "/" << i << "/" << endl;
                    //fs << brute << "/" << endl;
                    //cout <<  brute << "/" << i << "/" << endl;
                }
            Pos++;
            }
            //cout << "a";
        }

        if(brute=="http://kickass.so/usearch/zzz")
        {
            fs.close();
            return;
        }

        //if (hash(brute) == desired_hash)
        //return make_pair(true, brute);
    }
    while(next(brute));
    fs.close();
    return;
}


//http://www.cplusplus.com/forum/general/102786/
bool next(string& x)
{
    size_t len = x.size();
    bool overflow = true;
    while(overflow) {
        overflow = false;
        if(--len == 0)
            return false;
        (x[len] == 'z')?(overflow = true, x[len] = 'a'):x[len] += 1;
        if(x[len]==':')
            x[len] += 39;
    }
    return true;
}
