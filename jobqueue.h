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

    EncodeJob* get_job();

    private:
    fs::path m_dir;
    std::queue<fs::path> m_waiting_files;
    pthread_mutex_t m_mutex;
    };
