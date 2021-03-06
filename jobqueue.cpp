#include "jobqueue.h"

#include <stdexcept>
#include <memory>
#include <iostream>

namespace fs = std::experimental::filesystem;

JobQueue::JobQueue(const fs::path& dir)
    : m_dir(dir)
    {
    for(auto& entry : fs::directory_iterator(dir)) {
        if(entry.path().extension() == ".wav")
            m_waiting_files.push(entry);
        }

    if(pthread_mutex_init(&m_mutex, nullptr) != 0)
        throw std::runtime_error("Could not initialize job queue mutex");
    }

JobQueue::~JobQueue()
    {
    pthread_mutex_destroy(&m_mutex);
    }

EncodeJob* JobQueue::get_job()
    {
    if(pthread_mutex_lock(&m_mutex) != 0)
        throw std::runtime_error("Could not lock mutex");
    // Jury-rigged lock guard
    std::unique_ptr<pthread_mutex_t, int(*)(pthread_mutex_t*)> lock_guard(&m_mutex, pthread_mutex_unlock);

    if(m_waiting_files.empty())
        return nullptr;

    auto filename = m_waiting_files.front();
    auto out_filename = filename.stem();
    out_filename += ".mp3";
    m_waiting_files.pop();
    std::cout << "Working on encoding " << filename << " to " << m_dir / out_filename << std::endl;
    return new EncodeJob(filename, m_dir / out_filename);
    }
