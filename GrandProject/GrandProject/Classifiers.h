#ifndef CLASSIFIERS_H
#define CLASSIFIERS_H

#include "Features.h"
#include "Funcs.h"


void classifier_SVM_Train(HWND hwnd)
{

    //DialogBox(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_SVM_OPTION),hwnd, ClassDlgProcedure);

    string labelFilePath = selectTxtFile(hwnd);
    ifstream lbl(labelFilePath);
    Mat train;
    Mat trainingLabel;

    float lblVal;

    if(!lbl)
    {
        MessageBox(hwnd, "No training label file selected !", "Error", MB_ICONERROR);
        return ;
    }

    while(lbl >> lblVal)
    {
        trainingLabel.push_back(lblVal);
    }

    string xmlFileTrain = selectXmlFileTrain(hwnd);

    FileStorage trn(xmlFileTrain,FileStorage::READ);
    trn["Vocabulary"] >> train; // Make sure you change this according to whats in your xml file ! SIFT "Vocabulary" HOG "Descriptor"
    trn.release();


    //************ If using train_auto **************************************//
    //CvParamGrid CvParamGrid_C(pow(2.0,-5), pow(2.0,15), pow(2.0,2));
    //CvParamGrid CvParamGrid_gamma(pow(2.0,-15), pow(2.0,3), pow(2.0,2));

     // Set up SVM's parameters
    CvSVMParams params;
    params.svm_type = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    //params.gamma = 6; //2
    params.C = 1000;

    params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS,50000,1e-6);



    cout <<"Train label row: " << trainingLabel.rows << endl;
    cout << "Train mat row: " << train.rows << endl;

    if(train.rows != trainingLabel.rows)
    {
        MessageBox(hwnd, "Row size of training matrix not same with number of labels","Error", MB_ICONERROR);
        return;
    }

    // Train the SVM
    CvSVM SVM;
    //long int startTime = GetTickCount(); // Start time of the SVM training
    tic;
    //SVM.train_auto(train, trainingLabel, Mat(),Mat(),params,10,CvParamGrid_C,CvParamGrid_gamma);  //<------ experiments before 24/3/2015


    SVM.train(train, trainingLabel, Mat(),Mat(),params);
    toc;

     //************ If using train_auto **************************************//

    //params = SVM.get_params();

    //cout << "Gamma: " << params.gamma << endl;
    //cout << "C: " << params.C << endl;

    //long int endTime = GetTickCount();// End of the training time.

     TCHAR pth[MAX_PATH];
     brwsFol.hwndOwner = hwnd;
     brwsFol.lpszTitle = ("Save trained Svm...");
     brwsFol.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
     LPITEMIDLIST pidl = SHBrowseForFolder(&brwsFol);

     if(pidl == 0)
     {
        return;
     }

     SHGetPathFromIDList(pidl,pth);

     string path(pth);
     path = path + "\\" + "learnedSVM.svm";
     SVM.save(path.c_str());

     //cout << "Start Time: " << startTime << endl;
     //cout << "End Time: " << endTime << endl;
     //cout << "Training Time: " << (endTime - startTime)/1000 << endl;

     stringstream ss;
     //string trainTIme;
     //ss << "Training time: " << (endTime - startTime)/1000 << " s";
     //trainTIme = ss.str();

     MessageBox(hwnd, "Notification", "Training complete...", MB_OK);
     //MessageBox(hwnd, trainTIme.c_str(),"Training complete...", MB_OK);

}

void comput_Recognition_Rate(HWND hwnd, string path)
{

    Mat tr;
    int valLbl;
    int valRes;
    string p = path;

    vector<int> lbl;
    vector<int> res;

    p = p.substr(0, p.find_last_of("\\/"));
    p = p + "\\";
    ofstream fl(p + "Result.txt");



    FileStorage fs(path, FileStorage::READ);

    fs["result"] >> tr;

    for(size_t rows = 0; rows < tr.rows; rows++)
    {
        for(size_t cols = 0; cols < tr.cols; cols++)
        {
            fl << tr.at<float>(rows, cols) << endl;
        }
    }

    string labelFilePath = selectTxtFile(hwnd);
    ifstream FileLbl(labelFilePath);
    ifstream FileRes("Result.txt");

    if(!FileLbl)
    {
        MessageBox(hwnd, "No labels selected","Done", MB_ICONERROR);
        return;
    }

    if(!FileRes)
    {
        MessageBox(hwnd, "No result file opened","Done", MB_ICONERROR);
        return;
    }


    while(FileLbl >> valLbl)
    {
        lbl.push_back(valLbl);
    }

    while(FileRes >> valRes)
    {
        res.push_back(valRes);
    }

    if(lbl.size() != res.size())
    {
        cout << "size of label and result not same !" << endl;
        return;
    }

    /*float trueCount = 0.0;
    float count = 0.0;

    for(size_t i = 0; i < lbl.size(); i++)
    {
        if(res[i] == lbl[i])
        {
            trueCount++;
        }

        count++;
    }

    float recog = 0.0;

    recog = (trueCount/count)*100;*/

    ofstream logFile(p + "ResultLog.csv");
    stringstream ss;
    string RR;
    vector<int> classes;
    vector<int>::iterator it;

    classes = lbl;
    it = std::unique(classes.begin(), classes.end());
    classes.resize(std::distance(classes.begin(),it));



    typedef vector<double> Col;
    typedef vector<Col> Matrix;

    Matrix confMatrix(lbl.size(), Col(lbl.size()));

    // Plot the confusion matrix
    for(size_t i = 0; i < lbl.size(); i++)
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
            //logFile << confMatrix[row][col] << ",";
            logFile << (confMatrix[row][col]/numberOfClass[row])*100;
            //logFile << confMatrix[row][col] <<",";
        }
        //confMatrix[row][col]= roundf( (confMatrix[row][col]/numberOfClass[row])*100*100)/100;
        //cout << endl;
        logFile << endl;
    }

    // Compute the diagonal of the confusion matrix to find the overal recognition rate
    double val = 0;

    for(size_t row = 0; row < classes.size() ; row++)
    {
        for(size_t col = 0; col < classes.size(); col++)
        {
          if(row == col)
          {
              val = val + (confMatrix[row][col]/numberOfClass[row])*100; // sum all the diagonal
          }
        }
    }

    ss << "Recognition Rate: " << val/classes.size() << " %";
    RR = ss.str();

    logFile << RR << endl;

    MessageBox(hwnd, RR.c_str(),"Accuracy", MB_OK);

}


void classifier_SVM_Test(HWND hwnd)
{
    Mat test;
    Mat results;
    string svmFilePath = selectSvmFile(hwnd);


    CvSVM SVM;
    SVM.load(svmFilePath.c_str());

    string xmlFileTest = selectXmlFileTest(hwnd);

    FileStorage tst(xmlFileTest,FileStorage::READ);
    tst["Vocabulary"] >> test; // Make sure you change this according to whats in your xml file ! SIFT "Vocabulary" HOG
    tst.release();

    SVM.predict(test,results);

     TCHAR pth[MAX_PATH];
     brwsFol.hwndOwner = hwnd;
     brwsFol.lpszTitle = ("Save Results...");
     brwsFol.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
     LPITEMIDLIST pidl = SHBrowseForFolder(&brwsFol);

     if(pidl == 0)
     {
        return;
     }

     SHGetPathFromIDList(pidl,pth);

     string path(pth);
     path = path + "\\" + "Result.xml";

    FileStorage fs(path,FileStorage::WRITE);
    fs << "result" << results;
    fs.release();

    MessageBox(hwnd, "Testing complete...","Done", MB_OK);


    comput_Recognition_Rate(hwnd, path);

}



#endif // CLASSIFIERS_H
