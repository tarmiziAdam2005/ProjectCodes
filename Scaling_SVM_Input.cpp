//Created by Tarmizi Adam on 30/3/2015
// This Small program is used to scale/normalize the input to a Neural Network or SVM between -1 and 1.
// Scaling the input values are suggested before using SVM or Neural Network. User may want to refer to
// The following reference on why scaling is suggested before feeding to SVM or Neural Network
//      1) http://www.csie.ntu.edu.tw/~cjlin/papers/guide/guide.pdf
//      2) ftp://ftp.sas.com/pub/neural/FAQ2.html#A_binary (under topic: Subject: Why not code binary inputs as 0 and 1?)

// Input: random data matrix
// Output: normalized version of data matrix (saved as .xml file)

// Dependencies: OpenCV library

#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

int main()
{
    ifstream fl("h.txt"); // Read a test data... Random float values

    Mat dat; // Matrix to be normalized between -1 and 1;
    Mat temp(5,5,0.0); // temporary matrix
    float val;

    double min;
    double max;

    if(!fl)
    {
        cout << "Cannot open data file !" << endl;
        return 1;
    }

    while(fl >> val)
    {
        dat.push_back(val);
    }

    dat = dat.reshape(0,5); // reshape our matrix to be 5 by 5

    dat.copyTo(temp); // copy our data matrix to temp

    cout << "Original data matrix contains: " << endl;
    cout << dat << endl;
    cout << endl;
    cout << "Temp data matrix contains: " << endl;
    cout << temp << endl;
    cout << endl;

    //********** Do Normalization between -1 and 1 row wise ************//
    for(int i = 0; i < dat.rows; i++)
    {
           minMaxLoc(dat.row(i),&min, &max);
           dat.row(i) = ((dat.row(i) - min)/(max - min) -0.5)*2; // Normalize

           min = 0.0; // reset values
           max = 0.0; // reset values
    }

    cout << "Normalized data matrix is: " << endl;
    cout << dat << endl;
    cout << endl;

    Mat denorm; // Matrix to store denormalized

    dat.copyTo(denorm);

    //*********** Do De-Normalization to get original data matrix *************//
    for(int i = 0; i < denorm.rows; i++)
    {
          minMaxLoc(temp.row(i),&min, &max);
          denorm.row(i) = (denorm.row(i)/2+0.5)*(max - min) + min; // De-Normalization

          min = 0.0;
          max = 0.0;
    }

    cout << endl;
    cout << "Original data matrix after De-Normalization: " << endl;
    cout << denorm << endl;

    //*************** Output normalized Matrix to .xml file ****************//
    FileStorage fs("normalized.xml",FileStorage::WRITE);
    fs << "Normalized" << dat;


    return 0;
}
