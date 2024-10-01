// DEPENDENCIES
#include <omp.h>
#include "CStopWatch.h"


// LIBRARIES
#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <fstream>


// GLOBAL VARIABLES
std::queue<std::string> shared_queue;


// FUNCTION INITIALIZATION
void producer_consumer(int num_p, int num_c, std::queue<std::string> q);


// MAIN
int main() {
    // Initialize timer
    CStopWatch timer;
    // initialize file variable
    std::ofstream myFile;
    // Initialize queue to hold file names
    std::queue<std::string> producer_files;
    // Fill file queue with file names
    for(int num = 1; num < 13; num++) {
        // Create file name
        std::string file_name = std::to_string(num) + ".txt";
        // Push file name to queue
        producer_files.push(file_name);
    }
    // create cvs file
    myFile.open("results.cvs");
    // Number of producer threads
    for(int prod_t = 1; prod_t < 13; prod_t++) {
        // Number of consumer threads
        for(int cons_t = 1; cons_t < 13; cons_t++) {
            // Start timer
            timer.startTimer();
            // producer consumer function
            producer_consumer(prod_t, cons_t, producer_files);
            // Stop timer
            timer.stopTimer();

            myFile << prod_t << ", " << cons_t << ", " << prod_t + cons_t << ", " << timer.getElapsedTime() << std::endl;
        }
    }
    myFile.close();

    return 0;
}


// FUNCTIONS
void producer_consumer(int num_p, int num_c, std::queue<std::string> q) {
    // initialize string variable, stores file lines
    std::string file_line;
    // initialize integer vairable, stores thread number
    int threadID;
    // set number of threads to max-number of threads needed
    omp_set_num_threads(num_p + num_c);
    // parallelization
    # pragma omp parallel private(file_line, threadID) shared(shared_queue)
    {
        // fetch thread id number
        threadID = omp_get_thread_num();
        // if the thread is a producer
        if(threadID < num_p) {
            std::string file_name;

            while(!q.empty()) {
                #pragma omp critical(fetch_file)
                {
                    file_name = q.front();
                    q.pop();

                }
                std::ifstream file(file_name);
                while(getline(file, file_line)) {
                    #pragma omp critical(add_to_queue)
                    {
                        shared_queue.push(file_line);
                    }
                }
                file.close();
            }
        }
        else {
            #pragma omp critical(std_out)
            {
                std::string temp = shared_queue.front();
                shared_queue.pop();
                std::cout << temp << std::endl;
            }
        }
    }

    return;
}
