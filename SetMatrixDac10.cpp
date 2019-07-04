#include "SetMatrixDac10.h"

SetMatrixDac10::SetMatrixDac10(){


     Pick_File();
     ReadMatrix();
     GenerateCmd();

     }



void SetMatrixDac10::Pick_File(){



    string filename=this->direc_usb1+"/dac10.txt";

    ifstream test_usb1(filename);


    if(!test_usb1.is_open()){

        filename=this->direc_usb0+"/dac10.txt";
        ifstream test_usb0 (filename) ;


        if(!test_usb0.is_open()){

            filename=this->directory+"/dac10.txt";


        }

    }


    this->file=filename;

}


int SetMatrixDac10::ReadMatrix(){

    //create an input file stream
    ifstream matrix(this->file,ios::in);
    int number;


    //vector to store the entire matrix

    while(matrix>>number){

        this->snake.push_back(number);
        //cout<<number<<endl;
    }


    if(snake.size()!=36){
        cout<<"ERROR: file \""<<file<<"\" is out of format.\nCheck number of elements."<<endl;
        return 1;

    }
    matrix.close();
    return 0;

}

void SetMatrixDac10::GenerateCmd(){

    int line, val, pixel=0;
    unsigned int index;
    string cmd;

    for(int asic=0; asic<6; asic++){

        cmd="echo \"slowctrl asic "+  to_string(asic)+"\" | nc "+ ZYNQ_IPH+" 23 -q 1";
        c2send.push_back(cmd);


        for(int board=0; board<6; board++){

            line=(5-board);

            cmd="echo \"slowctrl line "+  to_string(line)+"\" | nc "+ ZYNQ_IPH+" 23 -q 1";
            c2send.push_back(cmd);


            index=asic*6+board;

            cmd="echo \"slowctrl pixel "+to_string(pixel)+"\" | nc "+ ZYNQ_IPH+" 23 -q 1";
            c2send.push_back(cmd);

                val=this->snake[index];
                cmd="echo \"slowctrl dac10 "+to_string(val)+"\" | nc "+ ZYNQ_IPH+" 23 -q 1";
                c2send.push_back(cmd);
                //cout<<asic<<" "<<line<<" "<<pixel<<" value:"<<this->snake[index]<<endl;



        }


    }
    cmd="echo \"slowctrl apply\" | nc ";
    cmd+= ZYNQ_IPH;
    cmd+=" 23 -q 1";
    c2send.push_back(cmd);
}


void SetMatrixDac10::Write(){

int nel=this->c2send.size();

    for(int i=0; i<nel; i++){

    cout<<c2send[i]<<endl;
    }


}

void SetMatrixDac10::Send(){

int nel=this->c2send.size();

    for(int i=0; i<nel; i++){


    system(this->c2send[i].c_str());

    }


}



int main(int argc, char *argv[]){

SetMatrixDac10 dac10;


if(argc==2){

    stringstream ss;
    ss<<argv[1];
    string key=ss.str();

    if (key=="write"){

       dac10.Write();}else{

       cout<<"\nuse: SetMatrixDac10 to send the commands \n or: SetMatrixDac10 write , to check them. \n \n";

       }

}else{dac10.Send();}



return 0;
    }
