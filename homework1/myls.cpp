#include <iostream>
#include <dirent.h>
#include <getopt.h>
#include <map>
#include <string>

void printFiles(std::string directory,int hFlag){
    struct dirent *d0;
    DIR *dr;
    dr = opendir(directory.c_str());
    std::cout<< directory <<std::endl;
    if(dr!=NULL){
        for(d0=readdir(dr); d0!= NULL; d0=readdir(dr)){
            // if hFlag == 1 and d0[0] == '.' -> show hidden file
            // if hFlag != 1 and d0[0] == '.' -> do not current show hidden file
            if (hFlag == 1 && (d0 ->d_name[0] == '.' || d0 -> d_name[0] != '.')){
                std::cout<<d0->d_name<<std::endl;
            } else if (hFlag != 1 && d0 -> d_name[0] != '.'){
                std::cout<<d0->d_name<<std::endl;
            }
        }
        closedir(dr);
    }else{
        std::cout<<"\n Error Occured!";
    }
}


int main(int argc, char* argv[]){

    //check for flag -h
    int option;
    int hFlag;
    while((option = getopt(argc, argv,"h")) != -1){
        switch (option)
        {
        case 'h':
            hFlag = 1;
            break;
        
        default:
            hFlag = 0;
            break;
        }
    }
    
    //open current directory
    struct dirent *d;
    DIR *dr;
    dr = opendir(".");

    //list of files, map for faster lookup
    std::map<std::string,int> files;
    std::map<std::string,int>::iterator lookup;

    //if dr exists
    if(dr!=NULL){
        //read the program execution directory
        int count = 0;
        for(d=readdir(dr); d!= NULL; d=readdir(dr)){
            //if argc == 1 and hflag, print out contents of current directory
            count ++;
            if (argc == 2 && hFlag == 1 && count >= 1 || argc == 1){
                std::cout << d->d_name << std::endl;
            }
            files[d->d_name] = 1;
        }
        closedir(dr);
    }else{
        std::cout<<"\n Error Occured!";
    }

    //iterate through arguments
    for(int i = 1; i < argc; i++){
        //if iteration argument 1 and it is an -h, no need for lookup
        if(i == 1 && hFlag == 1){
            continue;
        }else{
            //lookup passed argument
            lookup = files.find(argv[i]);
            //if argument doesnt exist
            if(lookup == files.end()){
                std::cout<<"Cannot access "<<argv[i]<<std::endl;
            }else{
                //print out files
                printFiles(lookup->first,hFlag);
                std::cout<<'\n';
            }
        }
    }
    return 0;
}