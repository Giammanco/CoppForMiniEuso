/* by Corrado Giammanco 30/04/2019*/
#ifndef _DEAD_PIXEL_H
#define _DEAD_PIXEL_H

#include <iostream>
#include <fstream>

/* to remove white space from matrix */
#include <string>
#include <cctype>
#include <algorithm>

#include <math.h> /*for fmod*/

#include <vector>
#define CONFIG_DIR_M "./" // "/home/corrado/MiniEuso/nursery/NUOVIMAT"
#define DIR_USB0  "/media/usb0"
#define DIR_USB1 "/media/usb1"
#define ZYNQ_IPH "192.168.7.10"


/**
 * It reads the file DeadPixelMask.txt which contains the pixels to be switched-off.
 * A list of the string command to send trought telnet connection i provided
 * in  .c2send. This vector has the attribute .line, .asic and, .pixel
 */
class DeadPixelMask{

    struct deadpixel { int BOARD; int ASIC; int Number; int ECU; } pixel;
    struct str2mask  {std::string line; std::string asic; std::string pixel;} cmaskline;

    public:
    std::vector <deadpixel> Dead;  /* vector containing the coordinate of the pixel to mask, just to check */

    std::vector <str2mask>  c2send; /* vector containing the command to send in order to mask pixels,
				      if empty it means that no file is provided or it is wrong */
    std::string directory = CONFIG_DIR_M;
    std::string direc_usb0 = DIR_USB0;
    std::string direc_usb1 = DIR_USB1;
    std::string readed_file;

    DeadPixelMask();

    private:
    void Pick_File();
    int ReadDead();
    void OrderPixel(int & , int & , int, int);
    void Send();

};

#endif // _DEAD_PIXEL_H


 int main(){

     DeadPixelMask mask;
     //mask.ReadDead();

     //cout<<mask.Dead[0].Number<<endl;
    // cout<<mask.c2send[0].pixel;
     //cout<<mask.directory;

//     int n_max=mask.c2send.size();
//     std::string cmd="echo \"slowctrl mask 1\" | nc 192.168.7.10 -23 -q 1";
//     //if ( n_max>0){
//     std::cout<<n_max<<std::endl;
//    for(int i=0;i<n_max;i++){
//         std::cout<<mask.c2send[i].line<<std::endl;
//         std::cout<<mask.c2send[i].asic<<std::endl;
//         std::cout<<mask.c2send[i].pixel<<std::endl;
//         std::cout<<cmd<<std::endl;
//
//        }
//
//     std::cout<<mask.readed_file<<std::endl;
//    // }
//     return 0;
 }




