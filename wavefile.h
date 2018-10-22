#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <experimental/filesystem>

#include "riffformatchunk.h"

namespace fs = std::experimental::filesystem;

class WaveFile
    {
    public:
    WaveFile(const fs::path& pathname);

    uint16_t channels() const {return m_format.data.channels;}
    uint32_t sample_rate() const {return m_format.data.samples_per_second;}
    uint16_t bits_per_sample() const {return m_format.data.bits_per_sample;}
    uint16_t block_alignment() const {return m_format.data.block_alignment;}

    std::vector<char> read_samples(std::size_t num_samples);
    bool end_of_data() const {return m_remaining_data_bytes == 0 || m_file.eof();}

    class NotAWaveFile : public std::runtime_error
        {public: NotAWaveFile(const std::string& msg) : std::runtime_error(msg){}};

    class UnsupportedData : public std::runtime_error
        {public: UnsupportedData(const std::string& msg) : std::runtime_error(msg){}};

    private:
    std::ifstream m_file;
    RiffFormatChunk m_format;
    uint32_t m_remaining_data_bytes;
    };
