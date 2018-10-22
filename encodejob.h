#pragma once

#include <string>
#include <utility>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

class EncodeJob
    {
    public:
    EncodeJob(fs::path in_path, fs::path out_path)
        : m_in_path(std::move(in_path)),
          m_out_path(std::move(out_path))
        {}

    void run();

    private:
    fs::path m_in_path;
    fs::path m_out_path;
    };
