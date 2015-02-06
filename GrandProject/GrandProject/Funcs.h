#ifndef FUNCS_H

#define FUNCS_H

// Winapi headers....
#include <windows.h>
#include <windowsx.h>
#include <shlobj.h>
#include "Shlwapi.h"

//created lib
#include "Resource.h"


// Standard C/C++ headers...
#include <iostream>
#include <string>
#include <dirent.h> // directory manipulation....
#include <cstring>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>

using std::cout;
using std::endl;
using std::string;
using std::ofstream;
using std::vector;
using std::stringstream;
using std::ostringstream;
using std::ifstream;


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

string selectXmlFileTrain(HWND hwnd)
{
    char pth[300] = "\0";
    string filePath;

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hInstance = GetModuleHandle(NULL);
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter = 0;
    ofn.nFilterIndex = 0;
    ofn.lpstrTitle = "Select training file";

    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = pth;
    ofn.Flags = OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;
    ofn.lpstrFilter = TEXT("Xml file (*.xml)\0*.xml\0\0");

    GetOpenFileName(&ofn);

    filePath = pth;
    return filePath;

}

string selectXmlFileTest(HWND hwnd)
{
    char pth[300] = "\0";
    string filePath;

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hInstance = GetModuleHandle(NULL);
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter = 0;
    ofn.nFilterIndex = 0;
    ofn.lpstrTitle = "Select testing file";

    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = pth;
    ofn.Flags = OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;
    ofn.lpstrFilter = TEXT("Xml file (*.xml)\0*.xml\0\0");

    GetOpenFileName(&ofn);

    filePath = pth;
    return filePath;

}

string selectTxtFile(HWND hwnd)
{
    char pth[300] = "\0";
    string filePath;

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hInstance = GetModuleHandle(NULL);
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter = 0;
    ofn.nFilterIndex = 0;
    ofn.lpstrTitle = "Select label file";

    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = pth;
    ofn.Flags = OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;
    ofn.lpstrFilter = TEXT("Txt file (*.txt)\0*.txt\0\0");

    GetOpenFileName(&ofn);

    filePath = pth;
    return filePath;

}

string selectSvmFile(HWND hwnd)
{
    char pth[300] = "\0";
    string filePath;

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hInstance = GetModuleHandle(NULL);
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter = 0;
    ofn.nFilterIndex = 0;
    ofn.lpstrTitle = "Select svm file";

    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = pth;
    ofn.Flags = OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;
    ofn.lpstrFilter = TEXT("Svm file (*.svm)\0*.svm\0\0");

    GetOpenFileName(&ofn);

    filePath = pth;
    return filePath;

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
        return;
    }

    while((pent = readdir(pDir)) != nullptr)
    {
        if(pent == nullptr)
        {
            MessageBox(hwnd, "Dirent struct could not be initialized correctly !", "Error", MB_OK | MB_ICONERROR);
            cout << "Dirent struct could not be initialized correctly !" << endl;
            return;
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

    string imgPath;
    string fileName;
    vector<string> images;

    int count = 0;
    char *buffer = (char*)GlobalAlloc(GPTR, len + 1);
    GetDlgItemText(hwnd, IDT_IMG_ONE, buffer, len + 1);

    DIR *pDir = nullptr;

    pDir = opendir(dirPath.c_str());

    struct dirent *pent = nullptr; // dirent structure for directory manipulation.

    if(pDir == nullptr)
    {
        MessageBox(hwnd, "Images not renamed !", "Error", MB_OK | MB_ICONERROR);
        return;

    }

    while((pent = readdir(pDir)) != nullptr)
    {
        if(pent == nullptr)
        {
            MessageBox(hwnd, "Dirent struct could not be initialized correctly !", "Error", MB_OK | MB_ICONERROR);
            return;
        }

        if(!strcmp(pent->d_name,".")||!strcmp(pent->d_name,".."))
        {

        }
        else
        {
            fileName = pent->d_name;
            imgPath = dirPath + "\\" + fileName;
            images.push_back(imgPath);

            count++;

            imgPath = " ";
        }
    }

    cout << "there are: " << count << " files";

    TCHAR path[MAX_PATH];
    brwsFol.hwndOwner = hwnd;
    brwsFol.lpszTitle = ("Where to save the renamed image ?");
    brwsFol.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    LPITEMIDLIST pidl = SHBrowseForFolder(&brwsFol);

    if(pidl != 0)
    {
        SHGetPathFromIDList(pidl,path);
        //SetCurrentDirectory(path);
    }

    string pth(path);
    pth = pth + "\\";
    string s;
    string image;
    stringstream ss;


    size_t iter = 1;

    for(size_t i = 0; i < images.size(); i++)
    {
        string newImgName(buffer);
        ss << newImgName << std::setfill('0') << std::setw(10 - newImgName.length()) << iter << ".png";

        string savePath(pth);

        char *oldName = new char[images[i].size()+1];

        image = images[i];

        s = ss.str();
        savePath = savePath + s;
        cout << savePath << endl;

        char *newName = new char[savePath.size()+1];

        strcpy(oldName,image.c_str());
        strcpy(newName,savePath.c_str());

        rename(oldName,newName);

        ss.str(" "); // clear stringstream
        savePath = " "; // clear current renamed path
        newImgName = " ";
        iter ++;

       delete[] oldName;
       delete[] newName;

    }

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
