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
    Get_all_Files("./search/all");
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
    string name;
    string temp;
    string temp_short;

    DIR *hdir;
    struct dirent *entry;
    hdir=opendir(dir.c_str());
    do
    {
        entry=readdir(hdir);
        if(entry) {

            name=entry->d_name;
            if(name=="." or name=="..")
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
                    if(name=="index.html")
                    {
                        ScanFile(temp);
                    } else {
                        //cout << entry->d_name << endl;
                    }
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
    string Find1="<span style=\"color:#AAA; font-weight:normal; font-size:0.9em;\">(1&#8212;";
    string Max_Pages="";
    string Category=path.substr(13,3); //Achtung: geht nur bei ./kickass/usearch/XXX/...
    int Max_Pages_int;
    int Find1_length=Find1.length();
    int pos1=0;
    int i=0;
    char infile_char;
    char Char1;
    char Char2;

    //cout << Category << endl;
    //cin.get();

    //if(path.find("/1/")== string::npos) return;
    //cout << path << endl;
    //cout << path << " ";
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

                /*
                infile_ScanFile.get(Char1);
                infile_ScanFile.get(Char2);

                //cout << Char1 << Char2;
                if(Char1=='3' and Char2=='0')
                {
                    //cout << "Hallo" << endl;
                    for(i=0;i<14;i++)
                    {
                        infile_ScanFile.get(infile_char);
                    }
                } else {
                        while(infile_ScanFile.get(infile_char))
                        {
                            if(infile_char=='f') break;
                        }
                        infile_ScanFile.get(infile_char);
                }
                */


                while(infile_ScanFile.get(infile_char))
                {
                    if(infile_char=='f') break;
                }
                infile_ScanFile.get(infile_char);

                infile_ScanFile.get(infile_char);
                if(infile_char=='m')
                {
                    for(i=0;i<9;i++)
                    {
                        infile_ScanFile.get(infile_char);
                    }
                }


                Max_Pages="";


                do {
                    if(infile_char==')' or infile_char=='<') break;
                    if(infile_char!=',')
                    {
                       Max_Pages+=infile_char;
                    }
                } while(infile_ScanFile.get(infile_char));

                Max_Pages_int=ceilf(atoi(Max_Pages.c_str())/30.0); //The .0 is importend!
                results+=atoi(Max_Pages.c_str());

                outfile_ss << Category << " " << Max_Pages_int << '\n';
                cout << "[+] " << Max_Pages_int << " sites and " << results << " results" << endl;
                Max_Pages="";
                infile_ScanFile.close();
                return;
            }
        }
        //cout << "Critical Error! Outputfile will probably be corrupted and useless for you! Sorry." << endl; //Problem with corrupted Files fixed! :D
        cout << "No search result for " << path << endl;
        //cin.get();
        infile_ScanFile.close();
    }
}
