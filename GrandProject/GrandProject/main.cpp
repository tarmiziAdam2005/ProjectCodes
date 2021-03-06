// Created by Tarmizi Adam: 6/2/2015
// Program to do object recognition

#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#define tic      double tic_t = clock();
#define toc      std::cout << (clock() - tic_t)/CLOCKS_PER_SEC \
                           << " seconds" << std::endl;

#include <tchar.h>
#include <windows.h>

#include <string>


#include "WinProc.h"
#include "Resource.h"
#include "Funcs.h"
#include "Features.h"

using std::string;


/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ClassDlgProcedure(HWND, UINT, WPARAM, LPARAM);


// Our functions
void initialiseDialog(HWND hwnd);
void createClassLabelFile(HWND hwnd, string dirPath, int classLabel);
void createControls(HWND hwnd);
string browseFolder(HWND hwnd);

void renameImageFiles(HWND hwnd, string dirPath, int len);
void DIC_SIFT(HWND hwnd);
void DESCRIPTOR_SIFT(HWND hwnd);
void HOG_IO(HWND hwnd);
void extractHog(Mat &img_raw,vector< vector<float> > &v_descriptorValues, vector< vector<Point> > &v_locations);
void classifier_SVM_Train(HWND hwnd);
//void classifier_SVM_Train(HWND hwnd, DLGPROC ClassDlgProcedure);
void classifier_SVM_Test(HWND hwnd);
string selectXmlFileTrain(HWND hwnd);
string selectXmlFileTest(HWND hwnd);
string selectTxtFile(HWND hwnd);
string selectSvmFile(HWND hwnd);
void comput_Recognition_Rate(HWND hwnd, string path);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("Classification Project");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);


    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, MAKEINTRESOURCE(IDI_MYICON));
    wincl.hIconSm = (HICON)LoadImage(NULL, MAKEINTRESOURCE(IDI_MYICON),IMAGE_ICON,16,16,0);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = MAKEINTRESOURCE(hMenu);                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Classification Project"),       /* Title Text */
           (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZE |WS_MAXIMIZE), /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           800,                 /* The programs width */
           600,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

