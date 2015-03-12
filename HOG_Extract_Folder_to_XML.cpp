#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp> // This is needed for HOGDescriptor
#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h> // for directory manipulation
#include <cstring> // for strcmp(), memcpy()

/*
        Created: 12/3/2015 by Tarmizi Adam.
        Revised from file :HOG_Extract_Folder.cpp

        This small program loops trough a folder containing images and extract their corresponding HOG features one by one.

        Input: Image contained in a folder
        Output: 3780 HOG features for each image. HOG features are output to a ".xml" file. This can be easily used for any openCV recognition program

        Depending on the number of image (N) in your folder the ".csv" file will be a large "matrix" of N x 3780 feature matrix.

        Dependencies: OpenCV
*/

using namespace std;
using namespace cv;

void extractHog(Mat &img_raw,vector< vector<float> > &v_descriptorValues, vector< vector<Point> > &v_locations);

int main()
{
    FileStorage fs("HoG.xml", FileStorage::WRITE); //File to output the HOG vectors.

    DIR *pDir = nullptr;
    string imgDir("D:/WillowActions Backup/willowactions/sample/");// Directory of your images. (Change this according to where you images are)
    string imgPath;
    pDir = opendir(imgDir.c_str());
    string dirName;

    Mat img_raw; // Our loaded image.
    vector< vector<float> > v_descriptorValues; // Store are descriptor values (3780 HoG values per image)
    vector< vector<Point> > v_locations;

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
            extractHog(img_raw,v_descriptorValues,v_locations); // Extract HOG of image.
            count++;
        }
    }

    cout << "Number of files in folder: " << count << endl;
    cout << "Extracting. Please Wait..."
    cout << "Extraction complete.";
    cout << endl;
    cout << endl;

    int row = v_descriptorValues.size();
    int col = v_descriptorValues[0].size();

    cout << "Rows: " << row << endl;
    cout << "Columns: " << col << endl;

    Mat M(row, col, CV_32F); // OpenCV mat to store our HoG values

    // Copy our HoG values to the openCV mat.
    // 2D vector to openCV Mat conversion. Credit to: http://feelmare.blogspot.com/2014/04/example-source-code-of-extract-hog.html
    for(int i = 0; i < row; i++)
    {
        memcpy(&(M.data[col*i*sizeof(float)]),v_descriptorValues[i].data(),col*sizeof(float));
    }

    //Write to our .xml file !
    fs << "Descriptor" << M;

    fs.release();
    closedir(pDir);
    waitKey(0);

    return 0;
}

void extractHog(Mat &img_raw, vector< vector<float> > &v_descriptorValues, vector< vector<Point> > &v_locations)
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
