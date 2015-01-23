//Created by Tarmizi Adam 22/1/2015
// This program is used to rename all files inside a folder.

// Instructions: Provide the path containing all the files you want to rename (imgDir variable)
//             : Change manually where the newly renamed files would be saved (savedRenamedPath variable)

// Caution: All the files inside the original path will be renamed and transfered to the folder that you specified.

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

#include <dirent.h> // for directory manipulation
#include <cstring> // for strcmp()
#include <cstdio>

using namespace std;

int main()
{

    DIR *pDir = nullptr;
    string imgDir("C:/Users/ASUS/Desktop/Towards PhD/Human Activity Recognition/Sports action CMU/pami09_preRelease/try/");// Directory of your images.

    string imgPath;
    string fileName;
    vector<string> images;

    pDir = opendir(imgDir.c_str());

    struct dirent *pent = nullptr; // dirent structure for directory manipulation.


    if(pDir == nullptr)
    {
        cout << "Directory pointer could not be initialized correctly ! " << endl; // Some checking.
        return 1;
    }

    int count = 0;

    cout << "Please WAIT..." << endl;

    while((pent = readdir(pDir)) != nullptr)
    {
        if(pent == nullptr)
        {
            cout << " Dirent struct could not be initialized correctly !" << endl;
            return 1;
        }

        if(!strcmp(pent->d_name,".")||!strcmp(pent->d_name,".."))
        {

        }
        else
        {
            // LOOP trough the directory to read image one by one and extract their HOG
            fileName = pent->d_name;
            imgPath = imgDir+fileName; // Overall image path...
            images.push_back(imgPath);
            count++;
        }
    }

    stringstream ss;
    string s;

    string a = "Action"; // This one to be entered by user in the edit box
    string newImgFileName;
    string image;

    for(size_t i =0; i < images.size(); i++)
    {
        string savedRenamedPath("C:/Users/ASUS/Desktop/Towards PhD/Human Activity Recognition/Sports action CMU/pami09_preRelease/SaveRename/"); // To be selected by user
        char *oldName = new char[images[i].size()+1];
        ss << i; // send integer into stringstring
        s = ss.str(); // ss into string.

        image = images[i];

        newImgFileName = a+s;
        newImgFileName = newImgFileName + ".png";
        savedRenamedPath = savedRenamedPath+newImgFileName;

        char *newName = new char[savedRenamedPath.size()+1];

        strcpy(oldName,image.c_str());
        strcpy(newName,savedRenamedPath.c_str());

        rename(oldName,newName);

        ss.str(" "); // clear stringstream
        savedRenamedPath = " "; // clear current renamed path
        delete[] oldName;
        delete[] newName;

    }

    closedir(pDir);
    return 0;
}


