#ifndef FEATURES_H
#define FEATURES_H

// Winapi headers....
#include <windows.h>

// OpenCV headers
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/ml/ml.hpp>

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

    FileStorage checkFile("train_CMU.xml", FileStorage::READ);

    if(checkFile.isOpened() ==  true)
    {
        cout << "Train_CMU.xml already created" << endl;
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
        brwsFol.lpszTitle = ("Browse for location of the images...");
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
        int dictionarySize = 200;

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
        FileStorage fs("train_CMU.xml", FileStorage::WRITE);
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
    FileStorage fs("train_CMU.xml", FileStorage::READ);
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
    brwsFol.lpszTitle = ("Browse image to test...");
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
    FileStorage fs1("test.xml", FileStorage::WRITE);

    //write the new BOF descriptor to the file
    fs1 << "Vocabulary" << bowTry;

    MessageBox(hwnd,"Done creating vocabulary...","Complete", MB_OK);

    fs1.release();

}

#endif // FEATURES_H
