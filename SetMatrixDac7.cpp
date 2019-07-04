#include "SetMatrixDac7.h"

SetMatrixDac7::SetMatrixDac7(){


     Pick_File();
     ReadMatrix();
     GenerateCmd();

     }



void SetMatrixDac7::Pick_File(){



    string filename=this->direc_usb1+"/dac7.txt";

    ifstream test_usb1(filename);


    if(!test_usb1.is_open()){

        filename=this->direc_usb0+"/dac7.txt";
        ifstream test_usb0 (filename) ;


        if(!test_usb0.is_open()){

            filename=this->directory+"/dac7.txt";


        }

    }


    this->file=filename;

}


int SetMatrixDac7::ReadMatrix(){

    //create an input file stream
    ifstream matrix(this->file,ios::in);
    int number;


    //vector to store the entire matrix

    while(matrix>>number){

        this->snake.push_back(number);
        //cout<<number<<endl;
    }


    if(snake.size()!=2304){
        cout<<"ERROR: file \""<<file<<"\" is out of format.\nCheck number of elements."<<endl;
        return 1;

    }
    matrix.close();
    return 0;

}
/******************************
Dac7 is given in Human frendly way
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
void SetMatrixDac7::OrderPixel(int &x, int &y, int b, int a){

int x2;
int y2;


if((b==1||b==3 || b==5) && (a==0||a==2 || a==4)){

    x2=x;
    y2=y;
       }

if((b==1 || b==3 || b==5) && (a==1 || a==3 || a==5)){

    x2=abs(y-7);
    y2=x;

        }

if((b==0 || b==2 || b==4) && (a==0 || a==2 || a==4)){

    x2=y;
    y2=abs(x-7);

}

if((b==0 || b==2 || b==4 ) && (a==1 || a==3 || a==5)){

    x2=abs(x-7);
    y2=abs(y-7);

}

    x=x2;
    y=y2;


}


void SetMatrixDac7::GenerateCmd(){

    int x,y,line, val;
    unsigned int index, TX, TY;
    string cmd;

    for(int asic=0; asic<6; asic++){

        cmd="echo \"slowctrl asic "+  to_string(asic)+"\" | nc "+ ZYNQ_IPH+" 23 -q 1";
        c2send.push_back(cmd);


        for(int board=0; board<6; board++){

            line=(5-board);

            cmd="echo \"slowctrl line "+  to_string(line)+"\" | nc "+ ZYNQ_IPH+" 23 -q 1";
            c2send.push_back(cmd);

            for(int pixel=0; pixel<64; pixel++){

                x=pixel/8;
                y=pixel-x*8;
                OrderPixel(x,y,line,asic);
                TX=asic*8+x;
                TY=board*8+y;
                index=TX*48+TY;

                cmd="echo \"slowctrl pixel "+to_string(pixel)+"\" | nc "+ ZYNQ_IPH+" 23 -q 1";
                c2send.push_back(cmd);

                val=this->snake[index];
                cmd="echo \"slowctrl dac7 "+to_string(val)+"\" | nc "+ ZYNQ_IPH+" 23 -q 1";
                c2send.push_back(cmd);
                //cout<<asic<<" "<<line<<" "<<pixel<<" value:"<<this->snake[index]<<endl;
            }


        }


    }
    cmd="echo \"slowctrl apply\" | nc ";
    cmd+= ZYNQ_IPH;
    cmd+=" 23 -q 1";
    c2send.push_back(cmd);
}


void SetMatrixDac7::Write(){

int nel=this->c2send.size();

    for(int i=0; i<nel; i++){

    cout<<c2send[i]<<endl;
    }


}

void SetMatrixDac7::Send(){

int nel=this->c2send.size();

    for(int i=0; i<nel; i++){


    system(this->c2send[i].c_str());

    }


}



int main(int argc, char *argv[]){

SetMatrixDac7 dac7;


if(argc==2){

    stringstream ss;
    ss<<argv[1];
    string key=ss.str();

    if (key=="write"){

       dac7.Write();}else{

       cout<<"\nuse: SetMatrixDac7 to send the commands \n or: SetMatrixDac7 write , to check them. \n \n";

       }

}else{dac7.Send();}



return 0;
    }
