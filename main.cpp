#include <iostream>
#include <vector>
#include <thread>

#include <pthread.h>

#include "encodejob.h"
#include "jobqueue.h"

void* encoder_thread(void* _jobqueue)
    {
    JobQueue* jobqueue = (JobQueue*)(_jobqueue);

    std::unique_ptr<EncodeJob> current_job(jobqueue->get_job());
    while(current_job)  {
        try {
            current_job->run();
            }
        catch(std::runtime_error& error) {
            std::cerr << "Caught runtime_error running job: " << error.what() << std::endl;
            }
        current_job.reset(jobqueue->get_job());
        }

    return nullptr;
    }

int main(int argc, char **argv)
    {
    if(argc != 2) {
        std::cerr << "Usage: wav2mp3 <directory>" << std::endl;
        return EXIT_FAILURE;
        }

    JobQueue job_queue(argv[1]);

    const unsigned int num_cores = std::thread::hardware_concurrency();
    std::vector<pthread_t> threads;
    for(unsigned int i = 0 ; i < num_cores ; ++i) {
        threads.push_back(i);
        if(pthread_create(&threads[i], nullptr, encoder_thread, &job_queue) != 0)
            throw std::runtime_error("Could not start thread");
        }

    for(auto& thread : threads)
        pthread_join(thread, nullptr);

    return 0;
    }
