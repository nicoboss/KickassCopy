#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#import <stdio.h>
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


struct torrent {
    char name[1023];
    unsigned long long size_bytes;
    char hash_char[41];
    unsigned int files_count;
    char author[255];
    unsigned short year;
    char category[32];
    char subcategory[32];
    unsigned int seeders;
    unsigned int leechers;
} torrent;

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
void make_filestructure(string dir, unsigned char make_filestructure_deph);





int main()
{
    /*
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    now->
    cout << (now->tm_year + 1900) << '-'
         << (now->tm_mon + 2) << '-'
         <<  now->tm_mday
         << endl;
    */

    Find.push_back("magnet:?xt=urn:btih:"); //hash
    Find.push_back("cellMainLink"); //name
    Find.push_back("Posted by"); //Author
    Find.push_back("in <span id="); //category & subcategory
    Find.push_back("<td class=\"nobr center\">"); // size
    Find.push_back("<td class=\"center\">"); // created_at & files count
    Find.push_back("\">Torrentz.eu</a>"); // Next File


    for(int i=0;i<7;i++)
    {
        Find_length.push_back(Find[i].length());
    }

    cout << "Preparing HTML Scan...\n"
         << "Creating temporary directories..." << endl;
    string dir;
    mkdir("./temp");
    cout << "Deleting old temp files..." << endl;


    make_filestructure("./temp", 1);
    chdir("..");


    remove("Kickass_DB_Dump.csv");
    dir="./kickass";
    cout << "Check continue.txt file..." << endl;
    ifstream infile ("continue.txt", ios::in|ios::binary);
    if(infile)
    {
        getline(infile, Get_all_Files_continue_string);
        Get_all_Files_continue=true;
    }
    infile.close();
    cout << "Preparing Clock..." << endl;
    clock_t start;
    magnet_clock = clock();
    torrent_clock = clock();
    if(Get_all_Files_continue==false)
    {
        cout << "Start scaning..." << endl;
    } else {
        cout << "Start searching check point..." << endl;
    }
    cout << "Please wait! This may take some time...\n\n" << endl;

    Get_all_Files(dir, dir.length(), 0);
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
    //cout << path << endl;

    memset(torrent.author,0,sizeof(torrent.author)); //Very importend because otherwise there are sometime random values in the RAM!

    //cout << path << endl;
    //cin.get();


    string line_read;
    stringstream line_write_ss;
    string line_write;
    string Quote_author;
    bool duplicate;
    unsigned long tempfile_size;


    vector<int> Pos(7);
    char number_char[7];
    float size_number=0.0;
    int i;
    int element;

    int i_max=1;

    char infile_char;

    infile_ScanFile.open(path, ios::in|ios::binary);
    if (infile_ScanFile.is_open()) {
        while (infile_ScanFile.get(infile_char))
            //cout << infile_char;
        {
            for(i=0;i<i_max;i++)
            {
                if(infile_char==Find[i][Pos[i]])
                {
                    Pos[i]++;
                } else {
                    Pos[i]=0;
                }
            }

            if(Pos[0]==Find_length[0])
            {
                i_max=7;
                //cout << "0" << endl;
                memset(torrent.hash_char,0,sizeof(torrent.hash_char));
                //Useless because the has is alwas 40 characters and overwrite the whole char arrey!
                for(i=0;i<40;i++)
                {
                    infile_ScanFile.get(torrent.hash_char[i]);
                }
                //cout << torrent.hash_char << endl;
            } else if (Pos[1]==Find_length[1]) {
                //cout << "1" << endl;
                memset(torrent.name,0,sizeof(torrent.name));
                infile_ScanFile.get();
                infile_ScanFile.get();
                for(i=0;i<1023;i++)
                {
                    continue_without_charset_1:;
                    infile_ScanFile.get(infile_char);
                    if(infile_char=='<')
                    {
                        do {
                            if(infile_char=='>')
                            {
                                //infile_ScanFile.get(infile_char); No thiss wont work in the special case: ...rip.<strong class="red">aaa</strong></a> because the ><!
                                //i++; No i++ thats the trick. So the last '>' will be overwrited :D
                                goto continue_without_charset_1;
                            }
                            if(infile_char=='/')
                            {
                                infile_ScanFile.get(infile_char);
                                if(infile_char=='a')
                                {
                                    infile_ScanFile.get(infile_char);
                                    if(infile_char=='>') goto multiloop_break_1; //Ugly but necessary goto statment.
                                }
                            }
                        } while (infile_ScanFile.get(infile_char));
                    }
                    torrent.name[i]=infile_char;
                }
                multiloop_break_1:;
                //cout << torrent.name << endl;
            } else if (Pos[2]==Find_length[2]) {
                //cout << "2" << endl;
                memset(torrent.author,0,sizeof(torrent.author));
                for(i=0;i<128;i++) //32=defult error value
                {
                    infile_ScanFile.get(infile_char);
                    if(infile_char=='/')
                    {
                        infile_ScanFile.get(infile_char);
                        if(infile_char=='\"')
                        {
                            infile_ScanFile.get(infile_char);
                            if(infile_char=='>') break;
                        }
                    }
                }

                for(i=0;i<255;i++)
                {
                    continue_without_charset_2:;
                    infile_ScanFile.get(infile_char);
                    if(infile_char=='<')
                    {
                        do {
                            if(infile_char=='>')
                            {
                                //infile_ScanFile.get(infile_char); No thiss wont work in the special case: ...rip.<strong class="red">aaa</strong></a> because the ><!
                                //i++; No i++ thats the trick. So the last '>' will be overwrited :D
                                goto continue_without_charset_2;
                            }
                            if(infile_char=='/')
                            {
                                infile_ScanFile.get(infile_char);
                                if(infile_char=='a')
                                {
                                    infile_ScanFile.get(infile_char);
                                    if(infile_char=='>') goto multiloop_break_2; //Ugly but necessary goto statment.
                                }
                            }
                        } while (infile_ScanFile.get(infile_char));
                    }
                torrent.author[i]=infile_char;
                }
                multiloop_break_2:;
                //if(i<3) memset(torrent.author,0,sizeof(torrent.author));
            } else if (Pos[3]==Find_length[3]) {
                //cout << "3" << endl;

                memset(torrent.category,0,sizeof(torrent.category));
                memset(torrent.subcategory,0,sizeof(torrent.subcategory));
                for(element=0;element<2;element++)
                {
                    for(i=0;i<128;i++) //32=defult error value
                    {
                        infile_ScanFile.get(infile_char);
                        if(infile_char=='/')
                        {
                            infile_ScanFile.get(infile_char);
                            if(infile_char=='\"')
                            {
                                infile_ScanFile.get(infile_char);
                                if(infile_char=='>') break;
                            }
                        } else if (infile_char=='<') {
                            infile_ScanFile.get(infile_char);
                            if(infile_char=='/')
                            {
                                infile_ScanFile.get(infile_char);
                                if(infile_char=='d') //</div>
                                {
                                    if(element==0)
                                    {
                                        element=1; //to brake for loop
                                        memset(torrent.category,0,sizeof(torrent.category));
                                    }
                                    memset(torrent.subcategory,0,sizeof(torrent.subcategory)); //always
                                    goto multiloop_break_3;
                                }
                            }
                        }


                        //cout << infile_char;
                    }

                    for(i=0;i<32;i++) //128=defult error value
                    {
                        infile_ScanFile.get(infile_char);
                        if(infile_char=='<')
                        {
                            do {
                                if(infile_char=='>')
                                {
                                    infile_ScanFile.get(infile_char);
                                    break;
                                }
                                if(infile_char=='/')
                                {
                                    infile_ScanFile.get(infile_char);
                                    if(infile_char=='a')
                                    {
                                        infile_ScanFile.get(infile_char);
                                        if(infile_char=='>') goto multiloop_break_3; //Ugly but necessary goto statment.
                                    }
                                }
                            } while (infile_ScanFile.get(infile_char));
                        }

                        if(element==0)
                        {
                            torrent.category[i]=infile_char;
                            //cout << infile_char;
                        } else {
                            torrent.subcategory[i]=infile_char;
                            //cout << infile_char;
                        }
                    }
                    multiloop_break_3:;
                }
                //cout << torrent.category << endl;
                //cout << torrent.subcategory << endl;
            } else if (Pos[4]==Find_length[4]) {
                //cout << "4" << endl;
                memset(number_char,0,sizeof(number_char));
                for(i=0;i<8;i++)
                {
                    infile_ScanFile.get(infile_char);
                    if(infile_char==' ') break;
                    number_char[i]=infile_char;
                }
                //cout << number_char << endl;
                size_number=atof(number_char);

                for(i=0;i<6;i++)
                {
                    infile_ScanFile.get();
                }


                infile_ScanFile.get(infile_char);
                switch(infile_char)
                {
                    case 'M':
                        torrent.size_bytes=size_number*1000000;
                        break;
                    case 'G':
                        torrent.size_bytes=size_number*1000000000;
                        break;
                    case 'T':
                        torrent.size_bytes=size_number*1000000000000;
                        break;
                    case 'P':
                        torrent.size_bytes=size_number*1000000000000000;
                        break;
                    case 'k':
                        torrent.size_bytes=size_number*1000;
                        break;
                    case 'K':
                        torrent.size_bytes=size_number*1000;
                        break;
                    case 'b':
                        torrent.size_bytes=0;
                        break;
                    case 'B':
                        torrent.size_bytes=0;
                        break;
                    case '<':
                        torrent.size_bytes=0;
                        cout << "Missing file size!" << endl;
                        cout << torrent.name << endl;
                        cout << path << '/n' << endl;
                        break;
                    default:
                        torrent.size_bytes=0;
                        cout << "File Size Error!\nUnknown unit " << infile_char << " (+B)" << endl;
                        cout << torrent.name << endl;
                        cout << path << endl;
                        system("pause");
                }
                //cout << torrent.size_bytes << endl;
            } else if (Pos[5]==Find_length[5]) {
                //cout << "5" << endl;
                memset(number_char,0,sizeof(number_char));
                for(i=0;i<8;i++)
                {
                    infile_ScanFile.get(infile_char);
                    if(infile_char=='<') break;
                    number_char[i]=infile_char;
                }
                torrent.files_count=atoi(number_char);

                for(i=0;i<64;i++)
                {
                    infile_ScanFile.get(infile_char);
                    if(infile_char=='\"')
                    {
                        infile_ScanFile.get(infile_char);
                        if(infile_char=='>') break;
                    }
                }

                for(i=0;i<3;i++)
                {
                    infile_ScanFile.get(infile_char);
                    if(infile_char=='&') break;
                    number_char[i]=infile_char;
                }

                for(i=0;i<4;i++)
                {
                    infile_ScanFile.get();
                }

                infile_ScanFile.get(infile_char);
                if(infile_char=='y') {
                    torrent.year=2014-atoi(number_char);
                } else {
                    torrent.year=2014;
                }

                for(i=0;i<64;i++)
                {
                    infile_ScanFile.get(infile_char);
                    if(infile_char=='\"')
                    {
                        infile_ScanFile.get(infile_char);
                        if(infile_char=='>') break;
                    }
                }

                memset(number_char,0,sizeof(number_char));
                for(i=0;i<8;i++)
                {
                    infile_ScanFile.get(infile_char);
                    if(infile_char=='<') break;
                    number_char[i]=infile_char;
                }
                torrent.seeders=atoi(number_char);

                for(i=0;i<64;i++)
                {
                    infile_ScanFile.get(infile_char);
                    if(infile_char=='\"')
                    {
                        infile_ScanFile.get(infile_char);
                        if(infile_char=='>') break;
                    }
                }

                memset(number_char,0,sizeof(number_char));
                for(i=0;i<8;i++)
                {
                    infile_ScanFile.get(infile_char);
                    if(infile_char=='<') break;
                    number_char[i]=infile_char;
                }
                torrent.leechers=atoi(number_char);

                outfile_ss << "./temp/" << torrent.hash_char[0] << "/" << torrent.hash_char[1] << torrent.hash_char[2] << torrent.hash_char[3];
                //cout << outfile_ss.str() << endl;
                line_write_ss.clear(); //important!
                line_write_ss.str(string()); //= temp_ss.str(""); but maybe faster on bad compilers


                if(torrent.author[0]==0)
                {
                    torrent.author[0]='\\';
                    torrent.author[1]='N';
                    Quote_author="";
                } else {
                    Quote_author='"';
                }


                if(torrent.category[0]==0)
                {
                    torrent.category[0]='\\';
                    torrent.category[1]='N';
                }


                if(torrent.subcategory[0]==0)
                {
                    torrent.subcategory[0]='\\';
                    torrent.subcategory[1]='N';
                }

                /*
                //Errorlinefinder
                if(torrent.hash_char[0]=='5' and torrent.hash_char[1]=='f' and torrent.hash_char[2]=='2')
                {
                    cout << path << endl;
                    system("Pause");
                }
                */

                //memset(torrent.author,0,sizeof(torrent.author));

                line_write_ss << '"' << torrent.name << "\"|"
                              << torrent.size_bytes << "|"
                              << torrent.hash_char << "|"
                              << torrent.files_count << "|"
                              << Quote_author << torrent.author << Quote_author << "|"
                              << torrent.year << "|"
                              << torrent.category << "|"
                              << torrent.subcategory << "|"
                              << torrent.seeders << "|"
                              << torrent.leechers;

                line_write=line_write_ss.str();

                //cout << line_write << endl;

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
                        if(line_write==line_read)
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
                    //cout << line_write << endl;
                    tempfile << line_write << endl;
                }

                tempfile.close();

                outfile_ss.str("");
                outfile_ss.clear();

                memset(torrent.author,0,sizeof(torrent.author));


            } else if (Pos[6]==Find_length[6]) {
                //cout << "6" << endl;
                break; //Next File!
            }



        }
    }
    infile_ScanFile.close();
}


void make_filestructure(string dir, unsigned char make_filestructure_deph)
{
    string c="  ";
    //cout << (int)make_filestructure_deph << endl;
    //system("cd");
    //system("pause");

    chdir(dir.c_str());
    for(c.at(0)=48;c.at(0)<58;c.at(0)++)
    {
        mkdir(c.c_str());

        if(make_filestructure_deph<1)
        {
            make_filestructure_deph++;
            make_filestructure(c, make_filestructure_deph);
            make_filestructure_deph--;
            chdir("..");
        }
    }


    for(c.at(0)=97;c.at(0)<123;c.at(0)++)
    {
        mkdir(c.c_str());
        if(make_filestructure_deph<1)
        {
            make_filestructure_deph++;
            make_filestructure(c, make_filestructure_deph);
            make_filestructure_deph--;
            chdir("..");
        }
    }
    //make_filestructure_deph--;
    //chdir("..");
}
