#include <iostream>
#include <sstream>
#include <fstream>
#include <dirent.h>
#include <windows.h>
#include <math.h>
#include <shellapi.h>

using namespace std;


//Global Variables have to be Glabal!
unsigned long magnet_count=0;
unsigned long magnet_count_temp=0;
unsigned long magnet_clock;
unsigned int magnet_duplication_cont=0;
unsigned int magnet_duplication_cont_temp=0;

ifstream infile_ScanFile;
ofstream outfile;


void brute5(void);
bool next(string& x);
unsigned short ScanFile(string path, string searchstring);
void make_filestructure(string dir, unsigned char max_deph, unsigned char make_filestructure_deph);





int main()
{
    stringstream parm_ss;
    string parm;
    stringstream filename;

    //mkdir("./torrenz");
    //make_filestructure("./torrenz", 3, 1); //The 3. Parameter always have to be 1!
    //chdir("..");
    //system("dir");

    brute5();
    system("pause");

    /*
    ofstream outfile; //("IsoHunt.txt", ios::out|ios::binary|ios::trunc);
    for(int pack=1571;pack<10000;pack++)
    {
        cout << pack << endl;
        filename << "IsoHunt_Part" << pack << ".txt";
        outfile.open(filename.str(), ios::out|ios::binary|ios::trunc);
        for(int i=pack*1000;i<pack*1000+1000;i++)
        {

            outfile << "https://torrentz.eu/search?f=aaa&p=" << i << endl;
            //parm_ss << "--trust-server-names http://torrent.isohunt.to/download.php?id=" << i;
            //parm_ss << "--trust-server-names -P /wget http://isohunt.to/torrent_details/" << i <<"/";
            //parm=parm_ss.str();
            //parm_ss.str("");
            //parm_ss.clear();
        }
        outfile.close();
        parm_ss << "-q --trust-server-names -P ./wget/" << pack << "/ -i " << filename.str();
        parm=parm_ss.str();
        cout << parm << endl;

        ShellExecute(0, "open", "wget64", parm.c_str(), 0, SW_HIDE);
        parm_ss.str("");
        parm_ss.clear();
        filename.str("");
        filename.clear();
        filename << "./wget/" << int(pack-60);
        Get_all_Files(filename.str(), pack-60);
        filename.str("");
        filename.clear();
        Sleep(15000);
        //cin.get();
    }
*/
    return 0;
}





void brute5(void)
{
    stringstream subdirpath_ss;
    string subdirpath;
    stringstream parm_ss;
    string parm;
    stringstream p_outputfile_ss;
    string p_outputfile;
    stringstream searchstring_ss;
    string searchstring_string;

    unsigned long Pos=0;
    int i;
    unsigned short Max_Pages; //Big First lethers to don't mit up this with the max_pages from the page_scan function!
    fstream fs;
    fs.open ("kickass_for_HTtrack.txt", fstream::out);
    string brute="https://torrentz.eu/search?q=000";//('a', 48);
    outfile.open("torrentz_1.txt", ios::out|ios::binary|ios::trunc);
    do {
        //subdirpath_ss << brute.at(0) << "/" << brute.at(1) << "/" << brute.at(2);

        /*
        subdirpath=subdirpath_ss.str();
        subdirpath_ss.str("");
        subdirpath_ss.clear();

        parm_ss << "-P torrenz/" << subdirpath << "/ "
                << "/ -nv -nc --no-check-certificate --header=\"Accept: text/html\" "
                << "--user-agent=\"Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:21.0) Gecko/20100101 Firefox/21.0\" "
                //<< "--referer connect.wso2.com http://dist.wso2.org/products/carbon/4.2.0/wso2carbon-4.2.0.zip " //Download refer every time so useless future in my case!
                << "\"https://torrentz.eu/search?f=" << brute <<"&p=0\"";

                //-w 0 --random-wait

        parm=parm_ss.str();
        parm_ss.str("");
        parm_ss.clear();

        */

        //for(i=0;i<=Max_Pages;i++)
        //{
            outfile << brute << endl;
        //}
        //outfile.close();

        /*
        //ShellExecute(0, "open", "wget64", parm.c_str(), 0, SW_NORMAL);
        Sleep(6000);


        p_outputfile_ss << "torrenz/" << subdirpath << "/search@f=" << brute <<"&p=0";
        p_outputfile=p_outputfile_ss.str(); //I know it's a bit useless but otherwise more difficult to understand.
        p_outputfile_ss.str("");
        p_outputfile_ss.clear();

        searchstring_ss << "<a href=\"/search?f=" << brute <<"&amp;p=";
        searchstring_string=searchstring_ss.str(); //I know it's a bit useless but otherwise more difficult to understand.
        searchstring_ss.str("");
        searchstring_ss.clear();

        Max_Pages=ScanFile(p_outputfile, searchstring_string);
        cout << Max_Pages << endl;

        outfile.open("torrentz.txt", ios::out|ios::binary|ios::app);
        for(i=0;i<=Max_Pages;i++)
        {
            outfile << "https://torrentz.eu/search?f=" << brute <<"&p=" << i << endl;
        }
        outfile.close();

        /*

        //system("pause");

        parm_ss << "-P torrenz/" << subdirpath
                << "/ -nv -nc --no-check-certificate --header=\"Accept: text/html\" "
                << "--user-agent=\"Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:21.0) Gecko/20100101 Firefox/21.0\" "
                //<< "--referer connect.wso2.com http://dist.wso2.org/products/carbon/4.2.0/wso2carbon-4.2.0.zip " //Download refer every time so useless future in my case!
                << "-i " << brute;

        parm=parm_ss.str();
        cout << parm << endl;
        parm_ss.str("");
        parm_ss.clear();
        //ShellExecute(0, "open", "wget64", parm.c_str(), 0, SW_SHOWNORMAL);


        //parm="--no-check-certificate \"https://torrentz.eu/search?f=aaa&p=5\"";
        //ShellExecute(0, "open", "wget64", parm.c_str(), 0, SW_SHOWNORMAL);
        //Sleep((i*14090)+4);
        //Sleep(4000);
        //system("pause");

*/

        //if(brute[brute.length()-1]<'a' or brute[brute.length()-2]<'a' or brute[brute.length()-3]<'a') //Nubers in conbination only mod


        cout << brute << endl;

        if(brute=="https://torrentz.eu/search?q=zzz")
        {
            fs.close();
            return;
        }

        //if (hash(brute) == desired_hash)
        //return make_pair(true, brute);
    }
    while(next(brute));
    outfile.close();
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
        (x[len] == 'z')?(overflow = true, x[len] = '0'):x[len] += 1;
        if(x[len]==':')
            x[len] += 39;
    }
    return true;
}




unsigned short ScanFile(string path, string searchstring)
{
    unsigned short max_pages_temp=0;
    unsigned short max_pages=0;
    int Find1_length=searchstring.length();
    int pos1=0;
    int i;
    char number_char[4];
    char infile_char;

    cout << path << endl;
    cout << searchstring << endl;

    infile_ScanFile.open(path, ios::in|ios::binary);
    if (infile_ScanFile.is_open()) {
        while (infile_ScanFile.get(infile_char))
        {
            if(infile_char==searchstring[pos1])
            {
                pos1++;
            }else{
                pos1=0;
            }

            if(pos1==Find1_length)
            {
                //cout << infile_char << endl;
                //system("pause");

                memset(number_char,0,sizeof(number_char));
                for(i=0;i<4;i++)
                {
                    infile_ScanFile.get(infile_char);
                    if(infile_char=='"') break;
                    number_char[i]=infile_char;
                }
                max_pages_temp=atof(number_char);

                if(max_pages<max_pages_temp)
                {
                    max_pages=max_pages_temp;
                }

            }
        }
        //cout << "Critical Error! Outputfile will probably be corrupted and useless for you! Sorry." << endl; //Problem with corrupted Files fixed! :D
        //cout << "Error path: " << path << endl;
        //cin.get();
        infile_ScanFile.close();
    } else {
        cout << "ERROR downloaded temp file not found!" << endl;
        //return 0; //return max_pages; returns also 0 if nothing is set. 0 means that only 1 page, no result or missing/defect temp file.
    }
    return max_pages;
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









































/*
Shellexecute;
If lpFile specifies an executable file, nShowCmd specifies how the application is to be shown when it is opened. This parameter can be one of the following values:
SW_HIDE
Hides the window and activates another window.
SW_MAXIMIZE
Maximizes the specified window.
SW_MINIMIZE
Minimizes the specified window and activates the next top-level window in the Z order.
SW_RESTORE
Activates and displays the window. If the window is minimized or maximized, Windows restores it to its original size and position. An application should specify this flag when restoring a minimized window.
SW_SHOW
Activates the window and displays it in its current size and position.
SW_SHOWDEFAULT
Sets the show state based on the SW_ flag specified in the STARTUPINFO structure passed to the CreateProcess function by the program that started the application. An application should call ShowWindow with this flag to set the initial show state of its main window.
SW_SHOWMAXIMIZED
Activates the window and displays it as a maximized window.
SW_SHOWMINIMIZED
Activates the window and displays it as a minimized window.
SW_SHOWMINNOACTIVE
Displays the window as a minimized window. The active window remains active.
SW_SHOWNA
Displays the window in its current state. The active window remains active.
SW_SHOWNOACTIVATE
Displays a window in its most recent size and position. The active window remains active.
SW_SHOWNORMAL
Activates and displays a window. If the window is minimized or maximized, Windows restores it to its original size and position. An application should specify this flag when displaying the window for the first time.

If lpFile specifies a document file, nShowCmd should be zero.
*/
