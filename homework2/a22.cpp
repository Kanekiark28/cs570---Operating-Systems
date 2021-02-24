#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <pthread.h>
#include <algorithm>
#include <iterator>
#include <vector>
//g++ -std=c++11 a22.cpp -lpthread

typedef struct { 
    //pointer to a long which represents current status of computation being tracked
    long * CurrentStatus;
    //starting value for the computation
    long InitialValue;
    //computation is complete
    long TerminationValue;
} PROGRESS_STATUS;


//monitors the progress of a task in separate thread
void * progress_monitor(void * file){
    double finish=100;
    double start=0;
    double current=50;
    double monitor=((current-start)/finish)*50;
    std::cout<<"monitor: "<<monitor<<std::endl;

    for(int i=1;i<=monitor;i++){
        if(i%10==0){
            std::cout<<"+";
        }
        else{
            std::cout<<"-";
        }
    }
    std::cout<<"\n";

    return NULL;
}

//spawn a progress_monitor thread
//given file and the file size
//open the file and read byte by byte
//counting words
//calculate percentage done from file_size
//update progress_bar
long wordcount(std::string filename, int file_size){
    long words;
    std::ifstream infile(filename);
    std::istream_iterator<std::string> in{ infile }, end;
    words = std::distance(in, end);
    return words;
}
long bytes_count(std::string filename){
    std::ifstream infile(filename);

    char data;
    long byte=0;
    //const char WHITE_SPACE(32);
    while (!infile.eof())
    {
        infile>>data;
        if (32==infile.peek()||10==infile.peek()){//if statement will count blank spaces
                            //32 is space in ascii 
                            //10 is ascii value for new line 
            byte++;
        }
        byte++;
        /* code */
    }
    
    

return byte;
}



//passes the command line filenames to wordcount 
int main(int argc, char **argv){
    if (argc < 2){
        std::cerr << "Usage: count_words <infile>\n";
        return EXIT_FAILURE;
    }

    std::string filename = argv[1];
    std::cout << "File name is " << filename<< std::endl;

    struct stat st;
    if (stat(filename.c_str(), &st) != 0){
        return 0;
    }
    int size = st.st_size;
    std::cout << "File size is " << size << std::endl;

    //initialize struct
    PROGRESS_STATUS *progress = new PROGRESS_STATUS;
    long currentVal = 0;
    progress->CurrentStatus = &currentVal;
    progress->InitialValue = 0;
    progress->TerminationValue = size;
    // std::cout << "progress init to " << progress << std::endl;
    // std::cout << "Termination " << progress->TerminationValue << std::endl;
    // std::cout << "Current Status " << progress->CurrentStatus << std::endl;

    //create the thread
    pthread_t thread;

    pthread_create(&thread,NULL,progress_monitor,progress);

    long file_bytes=bytes_count(filename);
    std::cout << "Total number of bytes are " << file_bytes << std::endl;

    //progress_monitor(&filename);

    long totalwords = wordcount(filename,size);
    std::cout << "Total number of words are " << totalwords << std::endl;
    return 0;
}

