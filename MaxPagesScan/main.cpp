#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <ctime>
#include <stdlib.h>
#include <math.h>


using namespace std;
unsigned int Header_deaph=0;
unsigned long results=0;
bool search_sucsess=false;

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
    cout << "Start scaning...\n" << endl;
    Get_all_Files("./kickass");
    cout << "\nFound " << results << " results! :D" << endl;
    cout << "Saving...\n" << endl;
    ofstream outfile ("MaxPages.txt", ios::out|ios::binary|ios::trunc);
    outfile << outfile_ss.str();
    outfile.close();
    cout << "Done!" << endl;
    cout << "Press ENTER to close the program" << endl;
    cin.get();
    return 0;
}




//Ermittlungsart File oder Ordner selbst Erunden! :D
//Im Internet findet man dazu abgesehen von boost nichts Platformunabhängiges!
void Get_all_Files(string dir)
{
    //cout << dir << endl;
    //cin.get();
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

                myfile_Get_all_Files.open(temp, ios::in|ios::binary);
                if (myfile_Get_all_Files.is_open())
                {
                    myfile_Get_all_Files.close();
                    //cout << temp << endl;
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
    string Find1="results 1-";
    string Max_Pages="";
    string Category=path.substr(18,3); //Achtung: geht nur bei ./kickass/usearch/XXX/...
    int Max_Pages_int;
    int Find1_length=Find1.length();
    int pos1=0;
    char infile_char;

    //cout << Category << endl;
    //cin.get();

    //if(path.find("/1/")== string::npos) return;
    //cout << path << endl;

    infile_ScanFile.open(path, ios::in|ios::binary);
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
                }

                while(infile_ScanFile.get(infile_char))
                {
                    if(infile_char==' ') break;
                }


                Max_Pages="";

                while(infile_ScanFile.get(infile_char))
                {
                    if(infile_char=='<') break;
                    Max_Pages+=infile_char;
                }

                Max_Pages_int=ceil(atoi(Max_Pages.c_str())/25.0);
                results+=atoi(Max_Pages.c_str());

                outfile_ss << Category << " " << Max_Pages_int << '\n';
                cout << "[+] " << Max_Pages_int << " sites and " << Max_Pages << " results" << endl;
                Max_Pages="";
                infile_ScanFile.close();
                return;
            }
        }
        //cout << "Critical Error! Outputfile will probably be corrupted and useless for you! Sorry." << endl; //Problem with corrupted Files fixed! :D
        cout << "Error path: " << path << endl;
        //cin.get();
        infile_ScanFile.close();
    }
}
