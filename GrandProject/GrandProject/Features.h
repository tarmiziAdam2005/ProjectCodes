#ifndef FEATURES_H
#define FEATURES_H

// Winapi headers....
#include <windows.h>

// OpenCV headers
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/objdetect/objdetect.hpp> // This is needed for HOGDescriptor
#include <opencv2/imgproc/imgproc.hpp>

// Standard C/C++ headers...
#include <iostream>
#include <vector>

//created headers
#include "Funcs.h"

using namespace cv;
using std::cout;
using std::endl;
using std::vector;


// Create the dictionary
void DIC_SIFT(HWND hwnd)
{

    // Check if the file is already created, abort this function.

    FileStorage checkFile("Dictionary.xml", FileStorage::READ);

    if(checkFile.isOpened() ==  true)
    {
        cout << "Dictionary.xml already created" << endl;
        return;
    }
    else
    {
            // Step 1- Obtain the set of bags of features...

        float percent = 0;

        // to strore the current input image
        Mat input;
        // To store the keypoints that will be extracted by SIFT
        vector<KeyPoint> keypoints;
        //To store the SIFT descriptor of current image
        Mat descriptor;
        // To store all the descriptor that are extracted from all the images.
        Mat featuresUnclustered;
         // The SIFT feature extractor and descriptor
        SiftDescriptorExtractor detector;

        TCHAR pth[MAX_PATH];
        brwsFol.hwndOwner = hwnd;
        brwsFol.lpszTitle = ("Location of images to create dictionary...");
        brwsFol.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
        LPITEMIDLIST pidl = SHBrowseForFolder(&brwsFol);

        if(pidl != 0)
        {
            SHGetPathFromIDList(pidl,pth);
            //SetCurrentDirectory(path);
        }


        MessageBox(hwnd, pth, "test", MB_OK);

        DIR *pDir = nullptr;
        pDir = opendir(pth);
        struct dirent *pent = nullptr;
        int count = 0;
        string path(pth);
        path =  path + "\\";

        if(pDir == nullptr)
        {
            MessageBox(hwnd, "Features not extracted ! ", "Error", MB_OK | MB_ICONERROR);
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
                string imgName(pent->d_name);
                string fullPath = path + imgName;
                cout << fullPath << endl;
                // Open the image file
                input = imread(fullPath, CV_LOAD_IMAGE_GRAYSCALE);

                detector.detect(input,keypoints);

                //compute the descriptor for each keypoints
                detector.compute(input,keypoints,descriptor);

                // put all the feature descriptor in a single mat oject
                featuresUnclustered.push_back(descriptor);

                imgName = " ";
                count++;
            }
        }

        //Construct BOW k-means trainer
        // the number of bags
        int dictionarySize = 1024;

        //define term criteria
        TermCriteria tc(CV_TERMCRIT_ITER, 100, 0.001);

        // retry number
        int retries = 1;

        //necessary flags
        int flags = KMEANS_PP_CENTERS;

        //Create the BOW trainer
        BOWKMeansTrainer bowTrainer(dictionarySize, tc, retries, flags);

        //cluster the feature vectors
        Mat dictionary = bowTrainer.cluster(featuresUnclustered);

        //store the vocabulary
        FileStorage fs("Dictionary.xml", FileStorage::WRITE);
        fs << "Vocabulary" << dictionary;
        fs.release();

        cout << "Number of files: " << count << endl;

    }

    MessageBox(hwnd, "Done createing dictionary...","Complete",MB_OK);

}

// The dictionary is created, now we extract the features from the dictionary.

void DESCRIPTOR_SIFT(HWND hwnd)
{
    //Step 2 - Obtain the BoF descriptor for given image/video frame.

    //prepare BOW descriptor extractor from the dictionary
    Mat dictionary;
    FileStorage fs("Dictionary.xml", FileStorage::READ);
    fs["Vocabulary"] >> dictionary;
    fs.release();

    //Create a nearest neighbor matcher
    Ptr<DescriptorMatcher> matcher(new FlannBasedMatcher);

    //create SIFT feature point extractor
    Ptr<FeatureDetector> detector(new SiftFeatureDetector());

    // create SIFT descriptor extractor
    Ptr<DescriptorExtractor> extractor(new SiftDescriptorExtractor);

    //create BOF descriptor extractor
    BOWImgDescriptorExtractor bowDE(extractor, matcher);

    //Set the dictionary with the vocabulary we created in the first step
    bowDE.setVocabulary(dictionary);

    TCHAR pth[MAX_PATH];
    brwsFol.hwndOwner = hwnd;
    brwsFol.lpszTitle = ("Browse image to extract features...");
    brwsFol.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    LPITEMIDLIST pidl = SHBrowseForFolder(&brwsFol);

    if(pidl != 0)
    {
        SHGetPathFromIDList(pidl,pth);
        //SetCurrentDirectory(path);
    }

    DIR *pDir = nullptr;
    pDir = opendir(pth);
    struct dirent *pent = nullptr;
    int count = 0;
    string path(pth);
    path =  path + "\\";

    Mat bowTry;

    if(pDir == nullptr)
    {
        MessageBox(hwnd, "Features not extracted ! ", "Error", MB_OK | MB_ICONERROR);
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
            string imgName(pent->d_name);
            string fullPath = path + imgName;
            cout << fullPath << endl;
            // Open the image file
            Mat img = imread(fullPath, CV_LOAD_IMAGE_GRAYSCALE);

            //To store the keypoints that will be extracted by SIFT
            vector<KeyPoint> keypoints;

            //Detect SIFT keypoints (or feature points)
            detector->detect(img,keypoints);

            //To store BOF representation of the image
            Mat bowDescriptor;

            //extract BOF descriptor from given image
            bowDE.compute(img,keypoints,bowDescriptor);

            bowTry.push_back(bowDescriptor);

            bowDescriptor.release();

            imgName = " ";
            count++;
            cout << "Desc createion" << endl;
        }
    }

     //open the file to write the resultant descriptor
    FileStorage fs1("Features_test.xml", FileStorage::WRITE);

    //write the new BOF descriptor to the file
    fs1 << "Vocabulary" << bowTry;

    MessageBox(hwnd,"Done creating vocabulary...","Complete", MB_OK);

    fs1.release();

}

void extractHog(Mat &img_raw,vector< vector<float> > &v_descriptorValues, vector< vector<Point> > &v_locations)
{
    Mat img;
    cv::resize(img_raw,img_raw,Size(64,128));

    cvtColor(img_raw,img,CV_RGB2GRAY);

    HOGDescriptor d;

    vector<float> descriptorsValues;
    //Mat descriptorsValues;
    vector<Point> locations;
    d.compute(img,descriptorsValues,Size(0,0),Size(0,0),locations);

    v_descriptorValues.push_back(descriptorsValues);
    v_locations.push_back(locations);

}

void HOG_IO(HWND hwnd)
{

    int len = GetWindowTextLength(GetDlgItem(hwnd,IDT_FILE_NAME));
    char *buffer = (char*)GlobalAlloc(GPTR, len + 1);
    GetDlgItemText(hwnd, IDT_FILE_NAME, buffer, len + 1);

    string file(buffer);

    TCHAR pth[MAX_PATH];
    brwsFol.hwndOwner = hwnd;
    brwsFol.lpszTitle = ("Location of images to create dictionary...");
    brwsFol.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    LPITEMIDLIST pidl = SHBrowseForFolder(&brwsFol);

    if(pidl != 0)
    {
        SHGetPathFromIDList(pidl,pth);

    }

    FileStorage fs(file + ".xml", FileStorage::WRITE); //File to output the HOG vectors.

    DIR *pDir = nullptr; // Directory pointer

    string imgDir(pth);
    imgDir = imgDir + "\\";
    string imgPath;

    pDir = opendir(imgDir.c_str());

    string dirName;
    //cout << imgDir << endl;

    Mat img_raw; // Our loaded image.

    vector< vector<float> > v_descriptorValues; // Store are descriptor values (3780 HoG values per image)
    vector< vector<Point> > v_locations;

    struct dirent *pent = nullptr; // dirent structure for directory manipulation.

    if(pDir == nullptr)
    {
        MessageBox(hwnd,"Directory pointer could not be initialized correctly !", "Error", MB_OK | MB_ICONERROR);
        cout << "Directory pointer could not be initialized correctly ! " << endl; // Some checking.
        return;
    }

    int count = 0;

    cout << "Please WAIT..." << endl;

    while((pent = readdir(pDir)) != nullptr)
    {
        if(pent == nullptr)
        {
            MessageBox(hwnd,"Dirent struct could not be initialized correctly !", "Error", MB_OK | MB_ICONERROR);
            cout << "Dirent struct could not be initialized correctly !" << endl;
            return;
        }
        if(!strcmp(pent->d_name,".")||!strcmp(pent->d_name,".."))
        {

        }
        else
        {
            dirName = pent->d_name;
            imgPath = imgDir+dirName; // Overall image path... feed this to imread();
            img_raw = imread(imgPath,1);// read the image to extract HOG features
            extractHog(img_raw,v_descriptorValues,v_locations); // Extract HOG of image.
            count++;

        }


    }

    cout << "*****************Our HOG Statistics******************" << endl;
    cout << "Number of files in folder: " << count << endl;
    cout << "Extracting. Please Wait...";
    cout << "Extraction complete.";
    cout << endl;
    cout << endl;

    int row = v_descriptorValues.size();
    int col = v_descriptorValues[0].size();

    cout << "Rows: " << row << endl;
    cout << "Columns: " << col << endl;
    cout << "******************************************************" << endl;

    Mat M(row, col, CV_32F); // OpenCV mat to store our HoG values

    // Copy our HoG values to the openCV mat.
    // 2D vector to openCV Mat conversion. Credit to: http://feelmare.blogspot.com/2014/04/example-source-code-of-extract-hog.html
    for(int i = 0; i < row; i++)
    {
        memcpy(&(M.data[col*i*sizeof(float)]),v_descriptorValues[i].data(),col*sizeof(float));
    }

    //Write to our .xml file !
    fs << "Vocuabulary" << M;

    fs.release();
    closedir(pDir);

}



#endif // FEATURES_H
