/* by Corrado Giammanco 30/04/2019*/
#ifndef _SetMatrixDac7_H
#define _SetMatrixDac7_H

#include <iostream>
#include <fstream>

/* to remove white space from matrix */
#include <string>
#include <cctype>
#include <algorithm>

#include <math.h> /*for fmod*/

#include <vector>
#include <sstream>

using namespace std;

#define CONFIG_DIR_M "./" //
#define DIR_USB0  "/media/usb0"
#define DIR_USB1 "/media/usb1"
#define ZYNQ_IPH "192.168.7.10"


/**
 * It reads the file DeadPixelMask.txt which contains the pixels to be switched-off.
 * A list of the string command to send trought telnet connection i provided
 * in  .c2send. This vector has the attribute .line, .asic and, .pixel
 */
class SetMatrixDac7{


    public:

    vector<int> snake; /*vector containing the matrix

    std::vector <str2mask>  c2send; /* vector containing the command to send in order to mask pixels,
				      if empty it means that no file is provided or it is wrong */
    string directory = CONFIG_DIR_M;
    string direc_usb0 = DIR_USB0;
    string direc_usb1 = DIR_USB1;
    string file;

    SetMatrixDac7();


    vector <string>  c2send;
    void Send();
    void Write();

    private:
    void Pick_File();
    int ReadMatrix();
    void GenerateCmd();
    void OrderPixel(int & , int & , int, int);


};

#endif // _DEAD_PIXEL_H






