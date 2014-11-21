#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp> // This is needed for HOGDescriptor
#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h> // for directory manipulation
#include <cstring> // for strcmp()

/*
        Created 21/11/2014 by Tarmizi Adam.
        This small program loops trough a folder containing images and extract their corresponding HOG features one by one.

        Input: Image contained in a folder
        Output: 3780 HOG features for each image. HOG features are output to a ".csv" file.

        Depending on the number of image (N) in your folder the ".csv" file will be a large "matrix" of N x 3780 feature matrix.

        Dependencies: OpenCV
*/



using namespace std;
using namespace cv;

void extractHog(Mat &img_raw, ofstream &myFile);


int main()
{
    ofstream myFile("h.csv",ofstream::app); // File to output the HOG vectors.

    DIR *pDir = nullptr;
    string imgDir("C:/Users/ASUS/Desktop/Towards PhD/Human Activity Recognition/willowactions/JPEGImages/");// Directory of your images.
    string imgPath;
    pDir = opendir(imgDir.c_str());
    string dirName;

    Mat img_raw; // Our loaded image.

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
            dirName = pent->d_name;
            imgPath = imgDir+dirName; // Overall image path... feed this to imread();
            img_raw = imread(imgPath,1);// read the image to extract HOG features
            extractHog(img_raw,myFile); // Extract HOG of image.
            count++;
        }
    }

    cout << "Number of files in folder: " << count << endl;
    cout << "Extraction complete....";
    cout << endl;
    cout << endl;

    closedir(pDir);
    waitKey(0);
    return 0;
}

void extractHog(Mat &img_raw, ofstream &myFile)
{
    Mat img;
    cv::resize(img_raw,img_raw,Size(64,128));

    cvtColor(img_raw,img,CV_RGB2GRAY);

    HOGDescriptor d;

    vector<float> descriptorsValues;
    vector<Point> locations;
    d.compute(img,descriptorsValues,Size(0,0),Size(0,0),locations);
    //cout << "HOG Descriptor size is:" << d.getDescriptorSize() << endl;

    // Output the extracted HOG features to our ".csv" file
    for(size_t i = 0; i < descriptorsValues.size(); i++)
    {
        myFile << descriptorsValues[i] <<",";
    }

    myFile << endl; // skipt to the next row in the ".csv" file
    descriptorsValues.clear(); // clear our HOG vector.
}
