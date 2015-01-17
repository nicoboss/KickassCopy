#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <dirent.h>

using namespace std;

int main()
{
    string line;
    int Pos1=string::npos;
    int Pos2=string::npos;
    int Pos3=string::npos;;
    int Pos4=string::npos;;
    int i=1;

    ofstream outfile("dailydump.csv", ios::out|ios::trunc|ios::binary);
    ifstream infile("dailydump.txt", ios::in|ios::binary);
    while(getline(infile, line))
    {
        //cout << line << endl;
        Pos1=line.find_first_of('|');
        Pos4=line.find_last_of('|');
        Pos3=line.find_last_of('|', Pos4-1);
        Pos2=line.find_last_of('|', Pos3-1);

        //cout << Pos1 << " " << Pos2 << " " << Pos3 << " " << Pos4 << endl;

        //cout << line.substr(0,Pos1+1) << '"' << line.substr(Pos1+1,Pos2-Pos1-1) << '"' << line.substr(Pos2) << endl;

        if(Pos1!=string::npos and Pos2!=string::npos and Pos3!=string::npos and Pos4!=string::npos)
        {
            outfile << i << '|' << line.substr(0,Pos1+1) << '"' << line.substr(Pos1+1,Pos2-Pos1-1) << '"' << line.substr(Pos2) << endl;
        } else {
            //cout << "Error: " << line.substr(0,Pos1+1) << '"' << line.substr(Pos1+1,Pos2-Pos1-1) << '"' << line.substr(Pos2) << '\n' << endl;
            cout << "Error: " << line << '\n' << endl;
        }

            i++;

        //system("pause");

        //outfileline
    }
    infile.close();
    outfile.close();
}
