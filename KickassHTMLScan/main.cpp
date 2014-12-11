#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <dirent.h>

using namespace std;
unsigned int Header_deaph=0;

stringstream magnet_ss;
unsigned int magnet_count;
unsigned int magnet_count_all;

stringstream torrent_ss;
unsigned int torrent_count;
unsigned int torrent_count_all;


//Header
void Get_all_Files(string dir, unsigned int Header_Basisphath_length);
void ScanFile(string path);
void WriteMagnetFile(void);
void WriteTorrentFile(void);




int main()
{
    remove("Magnet_Links.txt");
    remove("Torrent_Links_for_HTTrack.txt");
    string dir="./kickass/";
    Get_all_Files(dir, dir.length());
    WriteMagnetFile(); //Last Save
    WriteTorrentFile(); //Last Save
    return 0;
}




//Ermittlungsart File oder Ordner selbst Erunden! :D
//Im Internet findet man dazu abgesehen von boost nichts Platformunabhängiges!
void Get_all_Files(string dir, unsigned int Header_Basisphath_length)
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
                //cout << "Punkt" << endl;
                continue;
            }
            else
            {
                temp_ss.clear(); //important!
                temp_ss.str(string()); //= temp_ss.str(""); but maybe faster on bad compilers

                temp_ss << dir << "/" << entry->d_name;
                temp=temp_ss.str();

                ifstream myfile (temp, ios::binary|ios::ate);
                if (myfile.is_open())
                {
                    ScanFile(temp);
                } else {
                    myfile.close();
                    Header_deaph++;
                    Get_all_Files(temp, Header_Basisphath_length);
                }
                myfile.close();
            }
        }

    } while(entry);

    //headerfile.close();

    Header_deaph--;
    return;
}





void ScanFile(string path)
{
    string Find1="http://torcache.net/torrent/";
    string Find2="magnet:?xt=urn:btih:";
    string Torrent_URL;
    string Magnet_URL;
    int Find1_length=Find1.length();
    int Find2_length=Find2.length();
    int pos1=0;
    int pos2=0;
    char infile_char;

    ifstream infile (path, ios::in|ios::binary);
            if (infile.is_open()) {
                while (infile.get(infile_char))
                {
                    if(infile_char==Find1[pos1])
                    {
                        Torrent_URL+=infile_char;
                        pos1+=1;
                    }else{
                        pos1=0;
                        Torrent_URL="";
                    }

                    if(infile_char==Find2[pos2])
                    {
                        Magnet_URL+=infile_char;
                        pos2+=1;
                    }else{
                        pos2=0;
                        Magnet_URL="";
                    }


                    if(pos1==Find1_length)
                    {
                        while(infile.get(infile_char))
                        {
                            if(infile_char=='"' or infile_char==' ') break;
                            Torrent_URL+=infile_char;
                            pos1+=1;
                        }

                        pos1=0;
                        //cout << Torrent_URL << endl;
                        torrent_ss << Torrent_URL << endl;
                        torrent_count+=1;
                        if(torrent_count==1000) //=40 Files with 25 magnet Links (kickass)
                        {
                            torrent_count=0;
                            WriteTorrentFile();
                        }
                        Torrent_URL="";
                    }


                    if(pos2==Find2_length)
                    {
                        while(infile.get(infile_char))
                        {
                            if(infile_char=='"' or infile_char==' ') break;
                            Magnet_URL+=infile_char;
                            pos2+=1;
                        }

                        pos2=0;
                        //cout << Magnet_URL << endl;
                        magnet_ss << Magnet_URL << endl;
                        magnet_count+=1;
                        if(magnet_count==1000) //=40 Files with 25 magnet Links (kickass)
                        {
                            magnet_count=0;
                            WriteMagnetFile();
                        }
                        Magnet_URL="";
                    }

                }
            }
            infile.close();
}



void WriteMagnetFile(void)
{
    ofstream outfile ("Magnet_Links.txt", ios::out|ios::binary|ios::app);
    outfile << magnet_ss.str();
    outfile.close();
    magnet_count_all+=1000;
    cout << magnet_count_all << " Magnet links saved" << endl;
    magnet_ss.str("");
    magnet_ss.clear();
}


void WriteTorrentFile(void)
{
    ofstream outfile ("Torrent_Links_for_HTTrack.txt", ios::out|ios::binary|ios::app);
    outfile << torrent_ss.str();
    outfile.close();
    torrent_count_all+=1000;
    cout << torrent_count_all << " Torrent links saved" << endl << endl;
    torrent_ss.str("");
    torrent_ss.clear();
}
