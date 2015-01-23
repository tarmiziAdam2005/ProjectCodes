#ifndef FUNCS_H

#define FUNCS_H

// Winapi headers....
#include <windows.h>
#include <windowsx.h>
#include <shlobj.h>

//created lib
#include "Resource.h"

// Standard C/C++ headers...
#include <iostream>
#include <string>
#include <dirent.h> // directory manipulation....
#include <cstring>
#include <fstream>

using std::cout;
using std::endl;
using std::string;
using std::ofstream;

static OPENFILENAME ofn;

static BROWSEINFO brwsFol = {0};


char szFileName[500] = "\0";

//BROWSEINFO inf;
HWND hselectOK;
HWND hComboBox1;


void initialiseDialog(HWND hwnd)
{
    ZeroMemory(&ofn,sizeof(ofn));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.hInstance = NULL;
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter = 0;
    ofn.nFilterIndex = 0;
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = 500;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = MAX_PATH;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrFileTitle = NULL;
    ofn.Flags = 0;
    ofn.nFileOffset =0;
    ofn.nFileExtension = 0;
    ofn.lpstrDefExt = NULL;
    ofn.lCustData = 0L;
    ofn.lpfnHook = NULL;
    ofn.lpTemplateName = NULL;
}

string browseFolder(HWND hwnd)
{
    TCHAR path[MAX_PATH];
    brwsFol.hwndOwner = hwnd;
    brwsFol.lpszTitle = ("Select folder containing Image...");
    brwsFol.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    LPITEMIDLIST pidl = SHBrowseForFolder(&brwsFol);

    if(pidl != 0)
    {
        SHGetPathFromIDList(pidl,path);
        SetCurrentDirectory(path);
    }

    string test(path);
    return test;
}

void createClassLabelFile(HWND hwnd, string dirPath, int classLabel)
{
   string savePath;
   string slash("\\");
   char FilePath[MAX_PATH];

   GetModuleFileName(NULL, FilePath, MAX_PATH);
   string::size_type pos = string(FilePath).find_last_of("\\/");


   savePath = string(FilePath).substr(0, pos);
   savePath = savePath + slash;


   ofstream myFile(savePath + "label.txt",ofstream::app); // File to output the Class label file.

   DIR *pDir = nullptr;
   string imgPath;
   string dirName;
   int count = 0;

   pDir = opendir(dirPath.c_str());

   struct dirent *pent = nullptr;

   if(pDir == nullptr)
    {
        MessageBox(hwnd, "Label values not created ! Choose directory containing images ", "Error", MB_OK | MB_ICONERROR);
        //cout << "Directory pointer could not be initialized correctly ! " << endl; // Some checking.
    }

    while((pent = readdir(pDir)) != nullptr)
    {
        if(pent == nullptr)
        {
            MessageBox(hwnd, "Dirent struct could not be initialized correctly !", "Error", MB_OK | MB_ICONERROR);
            cout << "Dirent struct could not be initialized correctly !" << endl;
            //return 1;
        }

        if(!strcmp(pent->d_name,".")||!strcmp(pent->d_name,".."))
        {

        }
        else
        {
            count++;
        }
    }

    cout << "Number of files in directory: " << count << endl;

    for(int i = 1; i <= count; i++)
    {
        myFile << classLabel << endl;
    }

    closedir(pDir);

}


// Function that renames our images in a selected folder
void renameImageFiles(HWND hwnd, string dirPath)
{
        int len = GetWindowTextLength(GetDlgItem(hwnd,IDT_IMG_ONE));


            int count = 0;
            char *buffer = (char*)GlobalAlloc(GPTR, len + 1);
            GetDlgItemText(hwnd, IDT_IMG_ONE, buffer, len + 1);

            DIR *pDir = nullptr;

            pDir = opendir(dirPath.c_str());

            struct dirent *pent = nullptr; // dirent structure for directory manipulation.

            if(pDir == nullptr)
            {
                MessageBox(hwnd, "Images not renamed !", "Error", MB_OK | MB_ICONERROR);

            }

            while((pent = readdir(pDir)) != nullptr)
            {
                if(pent == nullptr)
                {
                    MessageBox(hwnd, "Dirent struct could not be initialized correctly !", "Error", MB_OK | MB_ICONERROR);

                }

                if(!strcmp(pent->d_name,".")||!strcmp(pent->d_name,".."))
                {

                }
                else
                {
                    count++;
                }
            }

            cout << buffer << endl;
            cout << "there are: " << count << " files";

            GlobalFree((HANDLE)buffer);





}

void createControls(HWND hwnd) // Create our listbox
{

    HWND hselectFeature;

    HINSTANCE hInstance = GetModuleHandle(NULL);

    // Create our List box
    hComboBox1 = CreateWindow(WC_COMBOBOX,"", CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD |WS_OVERLAPPED |WS_VISIBLE , 7, 20, 300, 100, hwnd, NULL, hInstance, NULL);
    SendMessage(hComboBox1, CB_ADDSTRING, 0,(LPARAM)"Histogram of Oriented Gradients (HOG)");
    SendMessage(hComboBox1, CB_ADDSTRING, 0,(LPARAM)"Scale Invariant Feature Transform (SIFT)");
    SendMessage(hComboBox1, CB_ADDSTRING, 0,(LPARAM)"Speeded Up Robust Features (SURF)");
    SendMessage(hComboBox1, CB_SETCURSEL, (WPARAM)0,(LPARAM)0); //CB_SETCURSEL

    // Create our push bottons
    hselectFeature = CreateWindow("STATIC", "Select Feature", SS_LEFT | WS_CHILD, 320,20, 100, 21,hwnd, (HMENU)1, hInstance, NULL);
    ShowWindow(hselectFeature,1);

    hselectOK = CreateWindow("BUTTON", "Ok", WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON | WS_DISABLED, 320,45, 100, 21,hwnd, (HMENU)ID_BUTTON_EXTRACTOK, hInstance, NULL);


}










#endif // FUNCS_H
