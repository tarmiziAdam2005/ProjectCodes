#include<iostream>
#include<vector>
#include<dirent.h>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>

/*
        Created: 27/4/2015 by Tarmizi Adam.
        This small program loops trough a folder containing images and extract their corresponding dense SIFT features one by one.
        Input: Image contained in a folder
        Output: Dense SIFT features for each image. Dense SIFT features are output to a ".xml" file. This can be easily used for any openCV recognition program
        Depending on the number of image (N) in your folder the ".csv" file will be a large "matrix" of N x M feature matrix. M is number of descriptor extracted
        from the image. M depends on grid used for dense sampling.
        Dependencies: OpenCV
*/

using namespace std;
using namespace cv;

void dense_SIFT(Mat img_raw, Mat &featuresUnclustered);

int main()
{

    initModule_nonfree();

     // Store detected image key points.

    Mat featuresUnclustered;

    DIR *pDir = nullptr;
    string imgDir("D:/WillowActions Backup/Willow 300_200/Train/");// Directory of your images. (Change this according to where you images are)
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
            // LOOP trough the directory to read image one by one and extract their dense SIFT
            dirName = pent->d_name;
            imgPath = imgDir+dirName; // Overall image path... feed this to imread();
            img_raw = imread(imgPath,1);// read the image to extract features
            dense_SIFT(img_raw, featuresUnclustered);
            count++;
        }
    }

   cout << "Number of files in folder: " << count << endl;
   cout << featuresUnclustered.size() << endl;



    //store the vocabulary
    FileStorage fs("D:/WillowActions Backup/Willow 300_200/DenseSIFT_Train.xml", FileStorage::WRITE);
    fs << "Vocabulary" << featuresUnclustered;
    fs.release();


   waitKey(0);
   return 0;

}

void dense_SIFT(Mat img_raw,Mat &featuresUnclustered)
{

    Mat descriptors; // Store our dense SIFT descriptors.
    vector<KeyPoint> keypoints;

    DenseFeatureDetector detector(12.f, 1, 0.1f, 10);

    cv::resize(img_raw,img_raw,Size(64,128));

    detector.detect(img_raw,keypoints);

    Ptr<DescriptorExtractor> descriptorExtractor = DescriptorExtractor::create("SIFT");

    descriptorExtractor->compute(img_raw,keypoints,descriptors);

    descriptors.setTo(0, descriptors < 0);
    descriptors = descriptors.reshape(0,1);

    featuresUnclustered.push_back(descriptors);


}
