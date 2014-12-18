#include <iostream>
#include <sstream>
#include <fstream>
#include <dirent.h>
#include <windows.h>
#include <shellapi.h>

using namespace std;


//Global Variables have to be Glabal!
unsigned long magnet_count=0;
unsigned long magnet_count_temp=0;
unsigned long magnet_clock;
unsigned int magnet_duplication_cont=0;
unsigned int magnet_duplication_cont_temp=0;


void Get_all_Files(string dir, unsigned int pack);

int main()
{
    mkdir("./Magnet");
    stringstream parm_ss;
    string parm;
    stringstream filename;
    ofstream outfile; //("IsoHunt.txt", ios::out|ios::binary|ios::trunc);
    for(int pack=405;pack<10000;pack++)
    {
        cout << pack << endl;
        filename << "oldpiratebay_Part" << pack << ".txt";
        outfile.open(filename.str(), ios::out|ios::binary|ios::trunc);
        for(int i=pack*1000;i<pack*1000+1000;i++)
        {

            outfile << "http://torrent.oldpiratebay.org/download.php?id=" << i << endl;
            //parm_ss << "--trust-server-names http://torrent.isohunt.to/download.php?id=" << i;
            //parm_ss << "--trust-server-names -P /wget http://isohunt.to/torrent_details/" << i <<"/";
            //parm=parm_ss.str();
            //parm_ss.str("");
            //parm_ss.clear();
        }
        outfile.close();
        //parm_ss << "-P ./wget/" << pack << "/ -O " << pack << ".torrent -i " << filename.str();
        parm_ss << "--content-disposition -P ./wget/" << pack << "/ -i " << filename.str();
        parm=parm_ss.str();
        cout << parm << endl;

        ShellExecute(0, "open", "wget64", parm.c_str(), 0, SW_HIDE);
        parm_ss.str("");
        parm_ss.clear();
        filename.str("");
        filename.clear();
        Sleep(25000);
        //cin.get();
    }


    return 0;
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
