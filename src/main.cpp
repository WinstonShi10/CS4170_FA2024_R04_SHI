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
void producer_consumer(int num_p, int num_c, std::queue<std::string>& q) {
    std::string file_line;
    int threadID;
    bool producers_finished = false;
    std::mutex queue_mutex;
    
    omp_set_num_threads(num_p + num_c);
    
    #pragma omp parallel private(file_line, threadID) shared(shared_queue, producers_finished, queue_mutex)
    {
        threadID = omp_get_thread_num();
        
        if (threadID < num_p) {
            // Producer
            std::string file_name;
            while (!q.empty()) {
                {
                    std::lock_guard<std::mutex> lock(queue_mutex);
                    if (!q.empty()) {
                        file_name = q.front();
                        q.pop();
                    }
                }
                if (!file_name.empty()) {
                    std::ifstream file(file_name);
                    if (!file.is_open()) {
                        std::cerr << "Failed to open file: " << file_name << std::endl;
                        continue;
                    }
                    while (getline(file, file_line)) {
                        std::lock_guard<std::mutex> lock(queue_mutex);
                        shared_queue.push(file_line);
                    }
                    file.close();
                }
            }
            
            #pragma omp critical(update_producers_finished)
            {
                producers_finished = true;
            }
        }
        else {
            // Consumer
            while (true) {
                std::string temp;
                bool has_item = false;
                
                {
                    std::lock_guard<std::mutex> lock(queue_mutex);
                    if (!shared_queue.empty()) {
                        temp = shared_queue.front();
                        shared_queue.pop();
                        has_item = true;
                    }
                }
                
                if (has_item) {
                    std::cout << temp << std::endl;
                }
                
                if (producers_finished && shared_queue.empty()) {
                    break;
                }
            }
        }
    }
}
