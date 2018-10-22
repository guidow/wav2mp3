#include "wavefile.h"

#include "riffheader.h"

namespace fs = std::experimental::filesystem;

WaveFile::WaveFile(const fs::path& pathname)
    {
    m_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    m_file.open(pathname, std::ios::in | std::ios::binary);

    RiffHeader header;
    m_file.read(header.bytes, sizeof(RiffHeader::bytes));
    if(std::string(header.data.name, 4) != "RIFF" || std::string(header.data.riff_type, 4) != "WAVE")
        throw NotAWaveFile("File is not a WAVE file");

    std::ios::pos_type riff_chunks_offset = m_file.tellg(); // Where the inner RIFF chunks begin

    // Find the format chunk with basic metadata
    bool format_chunk_found = false;
    while(!format_chunk_found) {
        RiffChunkHeader chunk_header;
        m_file.read(chunk_header.bytes, sizeof(RiffChunkHeader::bytes));
        if(std::string(chunk_header.data.name, 4) == "fmt ")
            format_chunk_found = true;
        else
            m_file.seekg(chunk_header.data.chunksize, std::ios::cur);
        }

    // Read basic metadata
    m_file.read(m_format.bytes, sizeof(RiffFormatChunk::bytes));
    if(m_format.data.format_tag != format_tag::PCM)
        throw UnsupportedData("Wave file has unsupported data format (only PCM supported)");

    // Find the data chunk
    // Rewind the stream, because it's possible the data chunk comes before the format chunk
    m_file.seekg(riff_chunks_offset);
    bool data_chunk_found = false;
    while(!data_chunk_found) {
        RiffChunkHeader chunk_header;
        m_file.read(chunk_header.bytes, sizeof(RiffChunkHeader::bytes));
        if(std::string(chunk_header.data.name, 4) == "data") {
            data_chunk_found = true;
            m_remaining_data_bytes = chunk_header.data.chunksize;
            }
        else
            m_file.seekg(chunk_header.data.chunksize, std::ios::cur);
        }

    // Just leave the file at this position, so we can start reading samples
    }

std::vector<char> WaveFile::read_samples(std::size_t num_samples)
    {
    if(num_samples == 0)
        return std::vector<char>{};
    std::size_t bytes_to_read = std::min(uint32_t(num_samples * m_format.data.block_alignment),
                                         m_remaining_data_bytes);
    std::vector<char> out(bytes_to_read);
    m_file.read(&out[0], bytes_to_read);

    // Resize the array in case fewer bytes than expected were read
    std::size_t bytes_read = m_file.gcount();
    out.resize(bytes_read);
    m_remaining_data_bytes -= bytes_read;

    return out;
    }
