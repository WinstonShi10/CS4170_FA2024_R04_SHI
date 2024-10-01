// DEPENDENCIES
#include <omp.h>
#include "CStopWatch.h"


// LIBRARIES
#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <mutex>
#include <atomic>


// GLOBAL VARIABLES
std::queue<std::string> shared_queue;


// FUNCTION INITIALIZATION
void producer_consumer(int num_p, int num_c, std::queue<std::string>& q);


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
    myFile.open("data.csv");
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
            // input data to csv file
            myFile << prod_t << ", " << cons_t << ", " << prod_t + cons_t << ", " << timer.getElapsedTime() << std::endl;
        }
    }
    // close file
    myFile.close();
    // end program
    return 0;
}


// FUNCTIONS
void producer_consumer(int num_p, int num_c, std::queue<std::string>& q) {
    // string to get file line
    std::string file_line;
    // get thread num
    int threadID;
    // check to see if producer is finished
    bool producers_finished = false;
    // use mutex to protect synchronization
    std::mutex queue_mutex;
    // set number of threads
    omp_set_num_threads(num_p + num_c);
    // parallelize section
    #pragma omp parallel private(file_line, threadID) shared(shared_queue, producers_finished, queue_mutex)
    {
        // get thread num
        threadID = omp_get_thread_num();
        // check to see if thread is producer
        if (threadID < num_p) {
            // get file name
            std::string file_name;
            // while file name queue is not empty
            while (!q.empty()) {
                {
                    // lock queue
                    std::lock_guard<std::mutex> lock(queue_mutex);
                    // get file name
                    if (!q.empty()) {
                        file_name = q.front();
                        q.pop();
                    }
                }
                if (!file_name.empty()) {
                    // open file
                    std::ifstream file(file_name);
                    // push line to shared queue
                    while (getline(file, file_line)) {
                        std::lock_guard<std::mutex> lock(queue_mutex);
                        shared_queue.push(file_line);
                    }
                    // close file
                    file.close();
                }
            }
            // producers has finished
            #pragma omp critical(update_producers_finished)
            {
                producers_finished = true;
            }
        }
        else {
            while (true) {
                // string to get line
                std::string temp;
                // check if line is gotten
                bool has_item = false;
                // wait for lock
                std::lock_guard<std::mutex> lock(queue_mutex);
                // go through shared queue
                if (!shared_queue.empty()) {
                    temp = shared_queue.front();
                    shared_queue.pop();
                    has_item = true;
                }
                // output line if line gotten
                if (has_item) {
                    std::cout << temp << std::endl;
                }
                // if the producer and shared queue are finished, end function
                if (producers_finished && shared_queue.empty()) {
                    break;
                }
            }
        }
    }
}
