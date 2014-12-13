#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <ctime>

using namespace std;
unsigned int Header_deaph=0;

stringstream outfile_ss;
fstream tempfile;
ifstream infile_ScanFile;
ifstream myfile_Get_all_Files;


//Header
void Get_all_Files(string dir);
void ScanFile(string path);




int main()
{
    cout << "Preparing MaxPages Scan..." << endl;
    cout << "Start scaning..." << endl;
    Get_all_Files("./kickass");
    cout << "Saving..." << endl;
    ofstream outfile ("MaxPages.txt", ios::out|ios::binary|ios::trunc);
    outfile << outfile_ss.str();
    outfile.close();
    cout << "Done!" << endl;
    return 0;
}




//Ermittlungsart File oder Ordner selbst Erunden! :D
//Im Internet findet man dazu abgesehen von boost nichts Platformunabhängiges!
void Get_all_Files(string dir)
{
    stringstream temp_ss;
    string temp;
    string temp_short;

    DIR *hdir;
    struct dirent *entry;
    hdir=opendir(dir.c_str());
    do
    {
        entry=readdir(hdir);
        if(entry) {

            temp=entry->d_name;
            if(temp=="." or temp=="..")
            {
                continue;
            }
            else
            {
                temp_ss.clear(); //important!
                temp_ss.str(string()); //= temp_ss.str(""); but maybe faster on bad compilers

                temp_ss << dir << "/" << entry->d_name;
                temp=temp_ss.str();

                myfile_Get_all_Files.open(temp.c_str(), ios::in|ios::binary);
                if (myfile_Get_all_Files.is_open())
                {
                    myfile_Get_all_Files.close();
                    cout << temp << endl;
                    ScanFile(temp);
                } else {
                myfile_Get_all_Files.close();
                //Header_deaph++;
                Get_all_Files(temp);
            }
        }
    }

    } while(entry);
    //Header_deaph--;
    return;
}





void ScanFile(string path)
{
    string Find1="Page 1 of ";
    string Max_Pages="";
    int Find1_length=Find1.length();
    int pos1=0;
    char infile_char;

    infile_ScanFile.open(path.c_str(), ios::in|ios::binary);
    if (infile_ScanFile.is_open()) {
        while (infile_ScanFile.get(infile_char))
        {
            if(infile_char==Find1[pos1])
            {
                pos1++;
            }else{
                pos1=0;
            }

            if(pos1==Find1_length)
            {
                while(infile_ScanFile.get(infile_char))
                {
                    if(infile_char==' ') break;
                    Max_Pages+=infile_char;
                    pos1++;
                }

                outfile_ss << Max_Pages << '\n';
                cout << Max_Pages << endl;
                Max_Pages="";
            }else {
                cout << Find1 << endl;
            }

        }
    }
}
