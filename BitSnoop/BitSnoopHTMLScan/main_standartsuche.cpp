#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <ctime>

using namespace std;
unsigned int Header_deaph=0;

const long double sysTime = time(0);
const long double sysTimeMS = sysTime*1000;

//Global Variables have to be Glabal!
stringstream magnet_ss;
unsigned long magnet_count=0;
unsigned long magnet_count_temp=0;
unsigned long magnet_clock;
unsigned int magnet_duplication_cont=0;
unsigned int magnet_duplication_cont_temp=0;

stringstream torrent_ss;
unsigned long torrent_count=0;
unsigned long torrent_count_temp=0;
unsigned long torrent_clock;
unsigned int torrent_duplication_cont=0;
unsigned int torrent_duplication_cont_temp=0;

stringstream outfile_ss;

fstream tempfile;
ifstream myfile_Get_all_Files;
ifstream infile_ScanFile;
stringstream infile_ScanFile_ss;
string infile_ScanFile_string;

double clock_duration;


//Header
void Get_all_Files(string dir, unsigned int Header_Basisphath_length, unsigned int Option);
void ScanFile(string path);




int main()
{
    cout << "Preparing HTML Scan...\n"
         << "Creating temporary directories..." << endl;
    string dir;
    mkdir("./temp");
    mkdir("./temp/Torrent");
    mkdir("./temp/Magnet");
    cout << "Deleting old temp files..." << endl;
    remove("Magnet_Links.txt");
    remove("Torrent_Links_for_HTTrack.txt");
    dir="./kickass/";
    cout << "Preparing Clock..." << endl;
    clock_t start;
    magnet_clock = clock();
    cout << "Start scaning...\n\n" << endl;
    Get_all_Files(dir, dir.length(), 0);
    cout << "Saving..." << endl;
    dir="./temp/Magnet";
    Get_all_Files(dir, dir.length(), 1);
    dir="./temp/Torrent";
    Get_all_Files(dir, dir.length(), 2);
    dir="./temp";
    cout << "Deleting old temp files...\n\n\n" << endl;
    Get_all_Files(dir, dir.length(), 3);
    cout << "Done!" << endl;
    return 0;
}




//Ermittlungsart File oder Ordner selbst Erunden! :D
//Im Internet findet man dazu abgesehen von boost nichts Platformunabhängiges!
void Get_all_Files(string dir, unsigned int Header_Basisphath_length, unsigned int Option)
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

                myfile_Get_all_Files.open(temp, ios::in|ios::binary);
                if (myfile_Get_all_Files.is_open())
                {
                    if(Option==0)
                    {
                        myfile_Get_all_Files.close();
                        ScanFile(temp);
                    } else if(Option==1) {
                        ofstream outfile ("Magnet_Links.txt", ios::out|ios::binary|ios::app);
                        outfile << myfile_Get_all_Files.rdbuf();
                        outfile.close();
                        myfile_Get_all_Files.close();
                    } else if(Option==2) {
                        ofstream outfile ("Torrent_Links_for_HTTrack.txt", ios::out|ios::binary|ios::app);
                        outfile << myfile_Get_all_Files.rdbuf();
                        outfile.close();
                        myfile_Get_all_Files.close();
                    } else {
                        myfile_Get_all_Files.close();
                        remove(temp.c_str());
                        //cout << "Deleted " << temp << endl;
                    }
                } else {
                myfile_Get_all_Files.close();
                //Header_deaph++;
                Get_all_Files(temp, Header_Basisphath_length, Option);
            }
        }
    }

    } while(entry);

    //headerfile.close();

    //Header_deaph--;
    return;
}





void ScanFile(string path)
{
    string Find1="http://torcache.net/torrent/";
    int Find1_Pos=0;
    string Find2="magnet:?xt=urn:btih:";
    string Torrent_URL;
    string Magnet_URL;
    string line;
    bool duplicate;
    unsigned long tempfile_size;
    int Find1_length=Find1.length();
    int Find2_length=Find2.length();
    int pos1=0;
    int pos2=0;
    char infile_char;

    infile_ScanFile.open(path, ios::in|ios::binary);
    if (infile_ScanFile.is_open()) {
        infile_ScanFile_ss << infile_ScanFile.rdbuf();
        infile_ScanFile_string=infile_ScanFile_ss.str();
        infile_ScanFile.close();
        while (Find1_Pos=infile_ScanFile_string.find_first_of(Find1, Find1_Pos+1))
        {
            if(Find1_Pos==string::npos) break;
            /*
            if(infile_char==Find1[pos1])
            {
                Torrent_URL+=infile_char;
                pos1++;
            }else{
                pos1=0;
                Torrent_URL="";
            }

            if(infile_char==Find2[pos2])
            {
                Magnet_URL+=infile_char;
                pos2++;
            }else{
                pos2=0;
                Magnet_URL="";
            }
            */


            //if(infile_ScanFile_string.find_first_of(Find1))
            {
                //Find1_Pos-infile_ScanFile_string.find(" ", Find1_Pos)
                Torrent_URL=infile_ScanFile_string.substr(Find1_Pos, infile_ScanFile_string.find_first_of('"', Find1_Pos)-Find1_Pos);
                //cout << Torrent_URL << endl;
                /*
                while(infile_ScanFile_string[infile_ScanFile_string])
                {
                    if(infile_char=='"' or infile_char==' ') break;
                    Torrent_URL+=infile_char;
                    pos1++;
                }
                */

                pos1=0;
                //cout << Torrent_URL << endl;
                torrent_ss << Torrent_URL << endl;
                outfile_ss << "./temp/Torrent/" << Torrent_URL[28] << Torrent_URL[29] << ".tmp";

                tempfile.open(outfile_ss.str(), ios::in|ios::out|ios::binary|ios_base::app);
                duplicate=false;
                tempfile.seekg(0, tempfile.end);
                //cout << tempfile.tellg() << endl;
                tempfile_size=tempfile.tellg();
                if(tempfile_size>0)
                {
                    tempfile.seekg(0, tempfile.beg);
                    while(tempfile_size>tempfile.tellg()+7)
                    {
                        getline(tempfile, line);
                        //cout << line << endl;
                        if(Torrent_URL==line)
                        {
                            duplicate=true;
                            torrent_duplication_cont_temp++;
                            //cout << "Duplicate!" << endl;
                            break;
                        }
                    }
                }

                if(duplicate==false)
                {
                    torrent_count_temp++;
                    if(torrent_count_temp==1000)
                    {
                        torrent_count+=torrent_count_temp;
                        torrent_count_temp=0;
                        torrent_duplication_cont+=torrent_duplication_cont_temp;

                        clock_duration = (clock()-torrent_clock) / (double) CLOCKS_PER_SEC;
                        cout << torrent_count << " Torrent links in " << clock()/(double)CLOCKS_PER_SEC
                             << "s and " << clock_duration << "s/part avg="
                             << (clock()/(double)CLOCKS_PER_SEC/torrent_count*1000) << "ms/l D/p="
                             << torrent_duplication_cont_temp << " Dubl=" << torrent_duplication_cont
                             << '\n' << endl;
                        torrent_duplication_cont_temp=0;
                        torrent_clock = clock();
                    }
                    tempfile << Torrent_URL << endl;
                }

                tempfile.close();

                outfile_ss.str("");
                outfile_ss.clear();
                Torrent_URL="";
            }


            if(pos2==Find2_length)
            {
                while(infile_ScanFile.get(infile_char))
                {
                    if(infile_char=='"' or infile_char==' ') break;
                    Magnet_URL+=infile_char;
                    pos2++;
                }

                pos2=0;
                //cout << Magnet_URL << endl;
                magnet_ss << Magnet_URL << endl;
                outfile_ss << "./temp/Magnet/" << Magnet_URL[20] << Magnet_URL[21] << ".tmp";

                tempfile.open(outfile_ss.str(), ios::in|ios::out|ios::binary|ios_base::app);
                duplicate=false;
                tempfile.seekg(0, tempfile.end);
                //cout << tempfile.tellg() << endl;
                tempfile_size=tempfile.tellg();
                if(tempfile_size>0)
                {
                    tempfile.seekg(0, tempfile.beg);
                    while(tempfile_size>tempfile.tellg()+7)
                    {
                        getline(tempfile, line);
                        //cout << line << endl;
                        if(Magnet_URL==line)
                        {
                            duplicate=true;
                            magnet_duplication_cont_temp++;
                            //cout << "Duplicate!" << endl;
                            break;
                        }
                    }
                }

                if(duplicate==false)
                {
                    magnet_count_temp++;
                    if(magnet_count_temp==1000)
                    {
                        magnet_count+=magnet_count_temp;
                        magnet_count_temp=0;
                        magnet_duplication_cont+=magnet_duplication_cont_temp;

                        clock_duration = (clock()-magnet_clock) / (double) CLOCKS_PER_SEC;
                        cout << magnet_count << " Magnet links in " << clock()/(double)CLOCKS_PER_SEC
                             << "s and " << clock_duration << "s/part avg="
                             << (clock()/(double)CLOCKS_PER_SEC/magnet_count*1000) << "ms/l D/p="
                             << magnet_duplication_cont_temp << " Dubl=" << magnet_duplication_cont
                             << endl;
                        magnet_duplication_cont_temp=0;
                        magnet_clock = clock();
                    }
                    tempfile << Magnet_URL << endl;
                }

                tempfile.close();

                outfile_ss.str("");
                outfile_ss.clear();
                Magnet_URL="";
            }

        }
    }
    infile_ScanFile.close();
}
