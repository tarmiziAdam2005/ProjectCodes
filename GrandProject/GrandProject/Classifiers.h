#ifndef CLASSIFIERS_H
#define CLASSIFIERS_H

#include "Features.h"
#include "Funcs.h"


void classifier_SVM_Train(HWND hwnd)
{
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
    trn["Vocabulary"] >> train;
    trn.release();

     // Set up SVM's parameters
    CvSVMParams params;
    params.svm_type = CvSVM::C_SVC;
    params.kernel_type = CvSVM::RBF;
    params.gamma = 2;
    params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER,20000,1e-6);

    if(train.rows != trainingLabel.rows)
    {
        MessageBox(hwnd, "Row size of training matrix not same with number of labels","Error", MB_ICONERROR);
        return;
    }

    // Train the SVM
    CvSVM SVM;
    SVM.train_auto(train, trainingLabel, Mat(),Mat(),params,10);

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

     MessageBox(hwnd, "Training complete...","Done", MB_OK);

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
    tst["Vocabulary"] >> test;
    tst.release();

    SVM.predict(test,results);

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
     path = path + "\\" + "Result.xml";

    FileStorage fs(path,FileStorage::WRITE);
    fs << "result" << results;
    fs.release();

    MessageBox(hwnd, "Testing complete...","Done", MB_OK);

}



#endif // CLASSIFIERS_H
