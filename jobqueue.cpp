#include "jobqueue.h"

#include <stdexcept>
#include <memory>

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
    std::unique_ptr<pthread_mutex_t, int(*)(pthread_mutex_t*)> lock_guard(&m_mutex, pthread_mutex_unlock);

    if(m_waiting_files.empty())
        return nullptr;

    auto filename = m_waiting_files.front();
    auto out_filename = filename.stem();
    out_filename += ".mp3";
    m_waiting_files.pop();
    return new EncodeJob(filename, m_dir / out_filename);
    }
