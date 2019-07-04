/* by Corrado Giammanco 30/04/2019*/
#ifndef _SetMatrixDac10_H
#define _SetMatrixDac10_H

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
 * It reads the file dac10.txt which contains the dac values for each pmt.
 * A list of the string command to send trought nc connection is provided
 * in  .c2send.
 */
class SetMatrixDac10{


    public:

    vector<int> snake; /*vector containing the matrix

    std::vector <str2mask>  c2send; /* vector containing the command to send in order to mask pixels,
				      if empty it means that no file is provided or it is wrong */
    string directory = CONFIG_DIR_M;
    string direc_usb0 = DIR_USB0;
    string direc_usb1 = DIR_USB1;
    string file;

    SetMatrixDac10();


    vector <string>  c2send;
    void Send();
    void Write();

    private:
    void Pick_File();
    int ReadMatrix();
    void GenerateCmd();



};

#endif // _DEAD_PIXEL_H






