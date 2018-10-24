#pragma once

#include <string>
#include <queue>
#include <experimental/filesystem>

#include <pthread.h>

#include "encodejob.h"

namespace fs = std::experimental::filesystem;

class JobQueue
    {
    public:
    JobQueue(const fs::path& dir);
    ~JobQueue();

    // Returns a pointer to an EncodeJob or nullptr if no work is left
    // Ownership of the pointer is with the caller
    EncodeJob* get_job();

    private:
    fs::path m_dir;
    std::queue<fs::path> m_waiting_files;
    pthread_mutex_t m_mutex;
    };
