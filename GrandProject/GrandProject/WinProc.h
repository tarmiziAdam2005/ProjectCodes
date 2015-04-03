#ifndef WINPROC_H

#define WINPROC_H

// Winapi Headers
#include <CommDlg.h>
#include <winuser.h>



// Standard C/C++ headers
#include <iostream>
#include <string>

// Created headers;
#include "Funcs.h"
#include "Resource.h"
#include "Features.h"
#include "Classifiers.h"


using std::cout;
using std::endl;
using std::string;

//using namespace cv;

int classNumber;
string dirPath;


BOOL CALLBACK ClassDlgProcedure(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{


    switch(Message)
    {
        case WM_INITDIALOG:

            SetDlgItemInt(hwnd, IDT_CLASS_ONE, 1, FALSE);
            SetDlgItemText(hwnd, IDT_IMG_ONE,NULL);

        break;

        case WM_COMMAND:

            switch(LOWORD(wParam))
            {

                case ID_OK:

                EndDialog(hwnd,ID_OK);
                classNumber = GetDlgItemInt(hwnd, IDT_CLASS_ONE, 0, FALSE);

                break;

                case ID_IMG_OK:
                    renameImageFiles(hwnd, dirPath);
                    EndDialog(hwnd, ID_IMG_OK);
                    break;

                case ID_FILE_NAME_OK:
                    HOG_IO(hwnd);
                    EndDialog(hwnd,ID_FILE_NAME_OK);
                    break;



            }

            break;

        case WM_CLOSE:
            EndDialog(hwnd, ID_IMG_OK);
        break;

        default:
            return FALSE;

    }

    return TRUE;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{


    int comboIndex;

    //Mat img;
    //HWND hwnd2;
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
            initialiseDialog(hwnd);
            createControls(hwnd);

            /*img = imread("cameraman.bmp",0);
            cvNamedWindow("Test", WINDOW_AUTOSIZE);
            hwnd2 = (HWND)cvGetWindowHandle("Test");
            hwnd = GetParent(hwnd2);
            SetParent(hwnd2,hwnd);
            ShowWindow(hwnd,SW_HIDE);*/

            break;

        case WM_COMMAND:
            {

                switch(HIWORD(wParam))
                    {
                        case  CBN_SELCHANGE:
                            {
                                //char listName[200];
                                //comboIndex = SendMessage((HWND) lParam, (UINT) CB_GETCURSEL, 0, 0);
                                //SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT, (WPARAM)comboIndex, (LPARAM)listName);
                                EnableWindow(hselectOK, TRUE);
                            }

                            break;

                            case BN_CLICKED: // When user has chosen a list, the OK button is used to process the feature extraction here.
                            {

                               if(LOWORD(wParam) == ID_BUTTON_EXTRACTOK)
                                  {
                                        char listName[200];
                                        comboIndex = SendMessage(hComboBox1, (UINT) CB_GETCURSEL, 0, 0);
                                        SendMessage(hComboBox1, (UINT)CB_GETLBTEXT, (WPARAM)comboIndex, (LPARAM)listName);

                                        if(comboIndex == 0)
                                        {
                                           MessageBox(hwnd,listName, "You chose", MB_OK);
                                           DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_FILE_NAME),hwnd,ClassDlgProcedure);

                                        }

                                        if(comboIndex == 1)
                                        {
                                           MessageBox(hwnd,listName, "You chose", MB_OK);
                                           DIC_SIFT(hwnd);
                                           DESCRIPTOR_SIFT(hwnd);
                                        }

                                        if(comboIndex == 2)
                                        {
                                            MessageBox(hwnd,listName, "You chose", MB_OK);

                                        }

                                  }

                            }

                            break;
                     }


                switch LOWORD(wParam)
                    {
                        case IDM_IMG_PATH:
                            {
                               dirPath = browseFolder(hwnd);

                               DialogBox(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_CLASS),hwnd, ClassDlgProcedure);
                               createClassLabelFile(hwnd, dirPath, classNumber);

                               return 0;
                            }

                        case IDM_EXIT:
                            {
                               PostMessage(hwnd, WM_CLOSE,0 , 0);
                            }
                        break;

                        case IDM_RENAME_IMG:
                                dirPath = browseFolder(hwnd);
                                DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_IMG_NAME),hwnd,ClassDlgProcedure);
                        break;

                        case IDM_SVM_TRAIN:
                            classifier_SVM_Train(hwnd);
                        break;

                        case IDM_SVM_TEST:
                            classifier_SVM_Test(hwnd);
                        break;

                    }
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}




#endif // WINPROC_H
