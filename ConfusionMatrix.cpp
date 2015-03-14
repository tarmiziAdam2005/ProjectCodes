// Created by Tarmizi Adam on 12/2/2015
// Updated: 14/3/2015
// This program Compute the confusion matrix and recognition rate between two vectors.
// The first vector is the expected labels. the second label is the output from a classifier i.e. SVM or Neural
// Network.

// Input: Two vectors, expected labels, and result from a classfier
// Output: Recognition rate and confusion matrix (.csv file)

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <utility>

using namespace std;

int main()
{
    ifstream inF("label_test.txt"); // labels: expected output. Change this two files to yours.
    ifstream inX("Result_will.txt");//
    ofstream out ("R.csv");

    int valLbl;
    int valRes;

    vector<int> lbl;
    vector<int> res;
    vector<int> classes;

    vector<int>::iterator it;

    while(inF >> valLbl)
    {
        lbl.push_back(valLbl);
    }

    while(inX >> valRes)
    {
        res.push_back(valRes);
    }

    //std::sort(lbl.begin(), lbl.end());
    classes =  lbl;
    it = std::unique(classes.begin(), classes.end());
    classes.resize(std::distance(classes.begin(),it));

    if(lbl.size() != res.size())
    {
        cout << "size not same !" << endl;
        return 1;
    }

   // vector<int> classCount;
    typedef vector<double> Col;
    typedef vector<Col> Matrix;

    Matrix confMatrix(lbl.size(), Col(lbl.size()));

    // Plot the confusion matrix
    for(size_t i = 0; i < confMatrix.size(); i++)
    {
        confMatrix[ lbl[i] - 1 ][ res[i] - 1 ]++;
    }

    double valCount = 0.0;
    vector<int> numberOfClass;

    for(size_t row = 0; row < classes.size() ; row++)
    {
        for(size_t col = 0; col < classes.size(); col++)
        {
          valCount = valCount + confMatrix[row][col]; // compute the number of patters for each class
        }

        numberOfClass.push_back(valCount);
        valCount = 0.0;
    }

    for(size_t row = 0; row < classes.size() ; row++)
    {
        for(size_t col = 0; col < classes.size(); col++)
        {
          //out << confMatrix[row][col] << ","; // Number of classification counts. Not in percentage %
          out << (confMatrix[row][col]/numberOfClass[row])*100 << ","; // Recog rate in %
        }
        out << endl;
    }

    // Compute the diagonal of the confusion matrix to find the overal recognition rate
    double val = 0;

    for(size_t row = 0; row < classes.size() ; row++)
    {
        for(size_t col = 0; col < classes.size(); col++)
        {
          if(row == col)
          {
              val = val + (confMatrix[row][col]/numberOfClass[row])*100;
          }
        }
    }

    cout << val << endl;
    cout << classes.size();
    cout << "Recognition: " << val/classes.size() << "%" << endl;
    out << "Recognition: " << val/classes.size() << "%" << endl;
    cout << endl;

    for(size_t col = 0; col < classes.size(); col++)
        {
          cout << numberOfClass[col] << ",";
        }

    return 0;
}
