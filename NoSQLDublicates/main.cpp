#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <dirent.h>

//#include <iomanip>      // std::put_time
//#include <time.h>
#include <ctime>        // std::time_t, struct std::tm, std::localtime
//#include <chrono>       // std::chrono::system_clock

using namespace std;
unsigned int Header_deaph=0;

const long double sysTime = time(0);
const long double sysTimeMS = sysTime*1000;

//Global variables have to be Glabal!
unsigned long magnet_count=0;
unsigned long magnet_count_temp=0;
unsigned long magnet_clock;
unsigned int magnet_duplication_cont=0;
unsigned int magnet_duplication_cont_temp=0;

unsigned long torrent_count=0;
unsigned long torrent_count_temp=0;
unsigned long torrent_clock;
unsigned int torrent_duplication_cont=0;
unsigned int torrent_duplication_cont_temp=0;

bool Get_all_Files_continue=false;
unsigned long Get_all_Files_continue_count=0;
unsigned long Get_all_Files_continue_count_temp=0;
string Get_all_Files_continue_string="";
unsigned long Get_all_Files_count;
unsigned long Get_all_Files_count_temp;

char hash_char[41];


vector<string> Find;
vector<unsigned char> Find_length;

stringstream outfile_ss;

fstream tempfile;
ifstream infile_ScanFile;
ifstream myfile_Get_all_Files;

double clock_duration;


//Header
void Get_all_Files(string dir, unsigned int Header_Basisphath_length, unsigned int Option);
void ScanFile(string path);
void make_filestructure(string dir, unsigned char max_deph, unsigned char make_filestructure_deph);





int main()
{
    setlocale(LC_ALL, "");
    Find.push_back(") VALUES\n("); //hash
    Find.push_back("', '"); //name
    Find.push_back(");\n"); //Author

    for(int i=0;i<3
    ;i++)
    {
        Find_length.push_back(Find[i].length());
    }

    cout << "Preparing HTML Scan...\n"
         << "Creating temporary directories..." << endl;
    string dir;
    mkdir("./temp");
    make_filestructure("./temp", 1, 1); //The 3. Parameter always have to be 1!
    chdir("..");
    cout << "Deleting old temp files..." << endl;
    remove("Kickass_DB_Dump.csv");
    dir="./torrenz";
    cout << "Preparing Clock..." << endl;
    clock_t start;
    magnet_clock = clock();
    torrent_clock = clock();
    system("pause");
    if(Get_all_Files_continue==false)
    {
        cout << "Start scaning..." << endl;
    } else {
        cout << "Start searching check point..." << endl;
    }
    cout << "Please wait! This may take some time...\n\n" << endl;
    ScanFile("OPB+Kickass_with_dublicates.sql");
    //Get_all_Files(dir, dir.length(), 0);
    cout << "Saving..." << endl;
    ofstream outfile ("Kickass_DB_Dump.csv", ios::out|ios::binary|ios::app);
    outfile << "name|size[Bytes]|hash|files_count|author|year|category|subcategory|seeders|leechers" << endl;
    outfile.close();
    dir="./temp";
    Get_all_Files(dir, dir.length(), 1);
    cout << "Deleting old temp files...\n\n\n" << endl;
    Get_all_Files(dir, dir.length(), 2);
    remove("continue.txt");
    cout << "Done!" << endl;
    cout << "Press ENTER to close the program" << endl;
    cin.get();
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
                        Get_all_Files_count_temp++;
                        //cout << Get_all_Files_count_temp << endl;
                        if(Get_all_Files_count_temp==1000 and Get_all_Files_continue==false)
                        {
                            Get_all_Files_count+=Get_all_Files_count_temp;
                            Get_all_Files_count_temp=0;
                            ofstream outfile ("continue.txt", ios::out|ios::binary|ios::trunc);
                            outfile << temp << endl;
                            outfile.close();
                            cout << "Saved check point!" << endl;
                        }

                        if(Get_all_Files_continue==false)
                        {
                           ScanFile(temp);
                        } else {
                            //cout << Get_all_Files_continue_string << endl;
                            //cout << temp << endl;
                            if(Get_all_Files_continue_string==temp)
                            {
                                Get_all_Files_count_temp=0;
                                Get_all_Files_continue_count+=Get_all_Files_continue_count_temp;
                                cout << Get_all_Files_continue_count
                                     << " files Scanned for finding the latest check point" << endl;
                                Get_all_Files_continue_count_temp=0;
                                Get_all_Files_continue=false;
                                magnet_clock = clock();
                                torrent_clock = clock();
                                cout << "\n\n\nStart scaning..." << endl;
                                cout << "Please wait! This may take some time...\n\n" << endl;
                            } else {
                                Get_all_Files_continue_count_temp++;
                                if(Get_all_Files_continue_count_temp==1000)
                                {
                                    Get_all_Files_continue_count+=Get_all_Files_continue_count_temp;
                                    cout << Get_all_Files_continue_count
                                         << " files Scanned for finding the latest check point" << endl;
                                    Get_all_Files_continue_count_temp=0;
                                }
                            }
                        }

                    } else if(Option==1) {
                        ofstream outfile ("Kickass_DB_Dump.csv", ios::out|ios::binary|ios::app);
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
    cout << path << endl;
    //system("pause");
    //cout << path << endl;
    //cin.get();

    string line_read;
    stringstream line_write_ss;
    string line_write;
    string Quote_author;
    bool duplicate;
    unsigned long tempfile_size;

    string line;
    string feld;
    unsigned int line_pos=0;
    unsigned short feld_pos=0;

    vector<int> Pos(7);
    int i;

    char infile_char;

    infile_ScanFile.open(path, ios::in|ios::binary);
    if (infile_ScanFile.is_open()) {

        unsigned int aaa=0;
        unsigned int bbb=0;

        /*
        while (infile_ScanFile.get(infile_char))
        {
            aaa++;
            if(aaa==1)
            {
                aaa=0;
                bbb++;
                cout << infile_char;
            }
        }
        system("pause");
        */

        while (infile_ScanFile.get(infile_char))
        {
            if(infile_char==Find[0][Pos[0]])
            {
                Pos[0]++;
            } else {
                Pos[0]=0;
            }
            if(Pos[0]==Find_length[0]) break;
        }

            //
        //}

        while(1)
        {
            //cout << line << endl;
            //memset(line,0,sizeof(line));
            feld_pos=0;
            line.clear();
            feld.clear();
            //infile_ScanFile.get(infile_char);
            while(infile_ScanFile.get(infile_char))
            {
                //cout << aaa++ << endl;
                //continue;
                //cout << "#";
                //cout << infile_char;
                line.push_back(infile_char);
                if(infile_char=='\n') break;
                //new_feld:;
                if(infile_char==' ')
                {
                    infile_ScanFile.get(infile_char);
                    line.push_back(infile_char);
                    if(infile_char=='\'')
                    {
                        infile_ScanFile.get(infile_char);
                        line.push_back(infile_char);
                        if(infile_char=='\'') continue;

                        //memset(hash_char,0,sizeof(hash_char));
                        if(feld_pos==1)
                        {
                            while(1)
                        {
                            //cout << infile_char;
                            //cout << "#";
                            if(infile_char=='\'')
                            {
                                infile_char_finchar_1:;
                                infile_ScanFile.get(infile_char);
                                line.push_back(infile_char);
                                if(infile_char==',')
                                {
                                    //infile_ScanFile.get(infile_char);
                                    line.push_back(infile_char);
                                    break;
                                } else if (infile_char=='\'') {
                                    goto infile_char_finchar_1; //for the rare but possible ''', case!
                                } else {
                                    //break;
                                    feld.push_back('\'');
                                    i++;
                                    feld.push_back(infile_char);
                                }


                            }
                            feld.push_back(infile_char);
                            infile_ScanFile.get(infile_char);
                            line.push_back(infile_char);
                        }
                        }

                        //cout << line << endl;
                        feld_pos++;

                        /*
                        if(feld_pos==2)
                        {
                            aaa++;
                            if(aaa==10000)
                            {
                                aaa=0;
                                bbb++;
                                for(i=0;i<40;i++)
                                {
                                    cout << feld[i];
                                }
                                cout << endl;
                            //system("pause");
                            }
                        }
                        */

                    } else {
                        if(infile_char=='\n') break;
                    }
                } else {
                    if(infile_char=='\n') break;
                }
            }

            //cout << endl;// << endl << endl;
            //system("pause");

            //cout << line << endl;
            /*
            for(i=0;i<2000;i++)
            {
                cout << line[i];
            }
            system("pause");
            */
            /*
                                        aaa++;
                            if(aaa==10000)
                            {
                                aaa=0;
                                bbb++;
                                for(i=0;i<40;i++)
                                {
                                    cout << feld[i];
                                }
                                cout << endl;
                            //system("pause");
                            }

            continue;
            */
            cout << feld << endl;

            if(feld.length()<40) continue;

    for(c.at(0)=48;c.at(0)<58;c.at(0)++)
    for(c.at(0)=97;c.at(0)<123;c.at(0)++)

            outfile_ss << "./temp/" << feld[0] << "/" << feld[1] << feld[2] << feld[3];

            //cout << feld << endl;

            tempfile.open(outfile_ss.str(), ios::in|ios::out|ios::binary|ios::app);
            duplicate=false;
            tempfile.seekg(0, tempfile.end);
            //cout << tempfile.tellg() << endl;
            tempfile_size=tempfile.tellg();
            if(tempfile_size>0)
            {
                tempfile.seekg(0, tempfile.beg);
                while(tempfile_size>tempfile.tellg()+7)
                {
                    getline(tempfile, line_read);
                    //cout << line << endl;
                    if(line==line_read)
                    {
                        duplicate=true;
                        torrent_duplication_cont_temp++;
                        //cout << "Duplicate!" << endl;
                        break;
                    }
                }
            }

            torrent_count_temp++;
            if(torrent_count_temp==1000)
            {
                torrent_count+=torrent_count_temp;
                torrent_count_temp=0;
                torrent_duplication_cont+=torrent_duplication_cont_temp;

                        clock_duration = (clock()-torrent_clock) / (double) CLOCKS_PER_SEC;
                        cout << torrent_count << " Torrents in " << clock()/(double)CLOCKS_PER_SEC
                             << "s and " << clock_duration << "s/part avg="
                             << (clock()/(double)CLOCKS_PER_SEC/torrent_count*1000) << "s/l D/p="
                             << torrent_duplication_cont_temp << " Dubl=" << torrent_duplication_cont
                             << "\nFile:" << path << '\n' << endl;
                        torrent_duplication_cont_temp=0;
                        torrent_clock = clock();
            }
            if(duplicate==false)
            {
                //cout << line << endl;
                tempfile << line;
            }

            tempfile.close();

            outfile_ss.str("");
            outfile_ss.clear();
            //memset(line,0,sizeof(line));
            //system("pause");

        }
    }
    infile_ScanFile.close();

}









void make_filestructure(string dir, unsigned char max_deph, unsigned char make_filestructure_deph)
{
    string c="  ";
    chdir(dir.c_str());
    //cout << (int)make_filestructure_deph << endl;
    //system("cd");
    //system("pause");


    for(c.at(0)=48;c.at(0)<58;c.at(0)++)
    {
        mkdir(c.c_str());

        if(make_filestructure_deph<max_deph)
        {
            make_filestructure_deph++;
            make_filestructure(c, max_deph, make_filestructure_deph);
            chdir("..");
            make_filestructure_deph--;
        }
    }


    for(c.at(0)=97;c.at(0)<123;c.at(0)++)
    {
        mkdir(c.c_str());
        if(make_filestructure_deph<max_deph)
        {
            make_filestructure_deph++;
            make_filestructure(c, max_deph, make_filestructure_deph);
            chdir("..");
            make_filestructure_deph--;
        }
    }
    //make_filestructure_deph--;
    //chdir("..");
}

