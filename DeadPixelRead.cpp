/* by Corrado Giammanco 30/04/2019*/

#include "DeadPixelRead.h"


DeadPixelMask::DeadPixelMask(){


     Pick_File();
     ReadDead();
     Send();

     }

/******************************
DeadPixelMask is given in Human frendly way
this function transforms the coordinate
taking into account that the Ec units have the following
orientation


0   7     56   0

56  63    63   7


7   63    63   56

0   56    7    0

Also we must take into account that the immage sowed by the viewer

is oriented in board and asic as follow:

B5 B4 B3 B2 B1 B0/ A0
                   A1
                   A2
                   A3
                   A4
                   A5


******************************/


void DeadPixelMask::OrderPixel(int &x, int &y, int b, int a){

int x2;
int y2;
//int x0=x0;
//int y0=y0;


if((b==0||b==2 || b==4) && (a==0||a==2 || a==4)){

    x2=x;
    y2=y;
       }

if((b==0 || b==2 || b==4) && (a==1 || a==3 || a==5)){

    x2=abs(y-7);
    y2=x;

        }

if((b==1 || b==3 || b==5) && (a==0 || a==2 || a==4)){

    x2=y;
    y2=abs(x-7);

}

if((b==1 || b==3 || b==5 ) && (a==1 || a==3 || a==5)){

    x2=abs(x-7);
    y2=abs(y-7);

}

    x=x2;
    y=y2;
    pixel.ASIC=b;
    pixel.BOARD=abs(a-5);

}

/*Check if the file is in usb1, usb0, local*/
void DeadPixelMask::Pick_File(){



    std::string filename=this->direc_usb1+"/DeadPixelMask.txt";

    std::ifstream test_usb1(filename);


    if(!test_usb1.is_open()){

        filename=this->direc_usb0+"/DeadPixelMask.txt";
        std::ifstream test_usb0 (filename) ;


        if(!test_usb0.is_open()){

            filename=this->directory+"/DeadPixelMask.txt";


        }

    }


    this->readed_file=filename;

}


int DeadPixelMask::ReadDead(){


    std::string line;

    int flag=0;

    int nline=0;

    //int BOARD;
    //int ASIC;
    //int ECU;
    int X; //x and y indices inside of a chip to be converted in pixel number
    int Y;
    //int Npixel;




   std::string filename=this->readed_file;

    std::ifstream ifile (filename) ;

	if (ifile.is_open()) {


		while(getline (ifile,line)){


			/*set a flag=0 to end the reading map*/
			if(line[0]=='^' && flag==1){
				flag=0;
				getline (ifile,line);
			}

			/*set a flag=1 to read only the map*/
			if(line[0]=='^' && flag==0){
				flag=1;
				getline (ifile,line);

			}


			if(flag==1)
			{
				/*remove white space and or tab from the matrix*/
				line.erase(remove_if(line.begin(),line.end(),::isspace),line.end());

				/*check the position of 1 in the line */

				if(line[0]!='\0'){

					int col; /*iteration variable to be remeber for check*/
					for(col=0; line[col]!='\0'; col++){

						/*line[col]=49 it's the character 1*/

						if(line[col]==49){

							/*having nline col coorinate of a dead pixel calculate  the BOARD ASIC and ECU and the number of pixel inside  of a chip*/
							pixel.BOARD=nline/8;
							pixel.ASIC=col/8;


							pixel.ECU=3*(nline/16)+col/16;
							X=fmod(nline,8);
							Y=fmod(col,8);
							//*It changes the number pixel and board and asic coordinate in order to correspond whith immages.
                            OrderPixel( X , Y, pixel.BOARD, pixel.ASIC );
							pixel.Number=X*8+Y;

							cmaskline.line="echo \"slowctrl line "+  std::to_string(pixel.BOARD)+"\" | nc "+ ZYNQ_IPH+" 23 -q 1";
							cmaskline.asic="echo \"slowctrl asic "+  std::to_string(pixel.ASIC)+"\" | nc "+ ZYNQ_IPH+" 23 -q 1";
							cmaskline.pixel="echo \"slowctrl pixel "+std::to_string(pixel.Number)+"\" | nc "+ ZYNQ_IPH+" 23 -q 1";


                            Dead.push_back(pixel);
                            c2send.push_back(cmaskline);



			   // std::cout<<'('<<pixel.BOARD<<';'<<pixel.ASIC<<')'<<'('<<nline<<';'<<col<<')'<<pixel.ECU<<",("<<X<<","<<Y<<"),"<<pixel.Number<<std::endl;



						}



					}


					//cout<<col<<endl;
					if(col!=48){

                        Dead.clear();
                        c2send.clear();
                        return 0;

					}
					nline++;
				}

			}




			    }
		ifile.close();

		//cout<<nline;
		if(nline!=48) {

            Dead.clear();
            c2send.clear();
            return 0; /*its a format error*/


        }


			  }

	else {
        std::cout << "Unable to open "<<filename<<std::endl;
        Dead.clear();
        c2send.clear();
        return 0;
        }

    return 1;



		}


    void DeadPixelMask::Send(){


     int n_max=this->c2send.size();

     std::string cmd="echo \"slowctrl mask 1\" | nc ";//ZYNQ_IPH" 23 -q 1";
     cmd+=ZYNQ_IPH;
     cmd+=" 23 -q 1";

     if ( n_max>0){

        std::cout<<"Number of masked pixels: "<<n_max<<std::endl;
        for(int i=0;i<n_max;i++){
            system(this->c2send[i].line.c_str());
            system(this->c2send[i].asic.c_str());
            system(this->c2send[i].pixel.c_str());
            system(cmd.c_str());
//
//            std::cout<<this->c2send[i].line<<std::endl;
//            std::cout<<this->c2send[i].asic<<std::endl;
//            std::cout<<this->c2send[i].pixel<<std::endl;
//            std::cout<<cmd<<std::endl;


        }

        std::cout<<"Mask in: "<<this->readed_file<<std::endl;
      }


		}

