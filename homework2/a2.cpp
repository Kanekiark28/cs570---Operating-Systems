#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <sys/stat.h>
#include <pthread.h>



typedef struct { 
    //pointer to a long which represents current status of computation being tracked
    long * CurrentStatus;
    //starting value for the computation
    long InitialValue;
    //computation is complete
    long TerminationValue;
} PROGRESS_STATUS;

void display_prog(int curr_prog){
    std::cout<<"\r";
    for(int i = 1; i<=curr_prog;i++){
        if(i%10==0){
            std::cout<<"+";
        }else{
            std::cout<<"-";

        }
    }
    //std::cout<<"\n";
}
//monitors the progress of a task in separate thread
void * progress_monitor(void * arg){
    PROGRESS_STATUS* prog_stat = (PROGRESS_STATUS*)arg;
    while(prog_stat->TerminationValue > *(prog_stat->CurrentStatus)){
        float curr_prog = ((*(prog_stat->CurrentStatus))/(float)(prog_stat->TerminationValue)) * (50);
        curr_prog = round(curr_prog);
        display_prog(curr_prog);
    }
    std::cout<<"\n"<<std::endl;
    pthread_exit(NULL);
}

//spawn a progress_monitor thread
//given file and the file size
//open the file and read byte by byte
//counting words
//calculate percentage done from file_size
//update progress_bar
long wordcount(char* path){
    FILE *file;
    file = fopen(path,"r");
    if(!file){
        std::cerr<<"Invalid File \n";
        return 0;
    }
    //stat for counting filesize
    struct stat s;
    stat(path,&s);
    int space_between = 0;
    long chars = 0;
    char c;
    long num_words = 0;

    
    //Initialize Struct resource
    PROGRESS_STATUS prog_stat;
    prog_stat.InitialValue = 0;
    prog_stat.TerminationValue = s.st_size;
    prog_stat.CurrentStatus = &chars;
    //create thread
    pthread_t thrd;
    pthread_create(&thrd,NULL,progress_monitor,(void*)&prog_stat);
    
    while((c = fgetc(file))!=EOF){
        chars++;
        if(isspace(c)){
            if(space_between){
                space_between = 0;
                num_words++;
            }
        }else{
            space_between = 1;
        }
    } pthread_join(thrd,NULL);
    fclose(file);
    return num_words;

}


//passes the command line filenames to wordcount 
int main(int argc, char **argv){
    if (argc < 2){
        std::cerr << "Please provide file name. \n";
        return EXIT_FAILURE;
    }
    char* path = argv[1];
    long total_words = wordcount(path);
    std::cout << "Total number of words are " << total_words << std::endl;
    return 0;
}

