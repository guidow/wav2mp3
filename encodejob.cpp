#include "encodejob.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <memory>

#include <lame/lame.h>

#include "wavefile.h"

void EncodeJob::run()
    {
    std::cout << "Working on encoding " << m_in_path << " to " << m_out_path << std::endl;
    WaveFile wave_file(m_in_path);

    if(wave_file.bits_per_sample() != 16)
        throw std::runtime_error("Unsupported bits per sample");
    if(wave_file.channels() != 1 && wave_file.channels() != 2)
        throw std::runtime_error("Unsupported number of channels");

    auto gfp = std::unique_ptr<lame_global_flags, int(*)(lame_global_flags*)>(
        lame_init(),
        lame_close);

    lame_set_num_channels(gfp.get(), wave_file.channels());
    lame_set_in_samplerate(gfp.get(), wave_file.sample_rate());
    lame_set_VBR(gfp.get(), vbr_mtrh);
    lame_set_VBR_q(gfp.get(), 2);
    lame_set_bWriteVbrTag(gfp.get(), 1);
    lame_set_quality(gfp.get(), 1);

    if(lame_init_params(gfp.get()) < 0)
        std::runtime_error("Error on initializing lame parameters");

    std::ofstream out_file(m_out_path, std::ios::out | std::ios::trunc | std::ios::binary);

    while(!wave_file.end_of_data()){
        auto in_buffer = wave_file.read_samples(1024);

        std::vector<unsigned char> out_buffer(7200 * 2);
        int out_bytes;
        if(wave_file.channels() == 2)
            out_bytes = lame_encode_buffer_interleaved(gfp.get(),
                                                       (short*)(&in_buffer[0]),
                                                       in_buffer.size() / wave_file.block_alignment(),
                                                       &out_buffer[0],
                                                       out_buffer.size());
        else
            out_bytes = lame_encode_buffer(gfp.get(),
                                           (short*)(&in_buffer[0]), (short*)(&in_buffer[0]), // For mono, use the same data for left and right channel
                                           in_buffer.size() / wave_file.block_alignment(),
                                           &out_buffer[0],
                                           out_buffer.size());
        out_file.write((char*)(&out_buffer[0]), out_bytes);
        }

    // Final encoding step, flushing remaining PCM buffers
    std::vector<unsigned char> out_buffer(7200 * 2);
    int out_bytes = lame_encode_flush(gfp.get(), &out_buffer[0], out_buffer.size());
    out_file.write((char*)(&out_buffer[0]), out_bytes);
    }
