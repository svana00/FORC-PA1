#include <fstream>
#include <math.h>
//#include <algorithm>
//#include <iostream>

/**
 * build_data
 * first parameter is array to be filled
 * the array must be initialized to at least 44 items before calling this operation
**/
void build_header(char header[], unsigned int sample_rate, unsigned int sample_size, unsigned int sample_count = 0) {

    //byte_rate = sample_rate * channels(1) * sample_size(2 bytes = 16 bits)
    unsigned int byte_rate = sample_rate * sample_size;
    unsigned int sub_chunk_2_size = sample_count * 2;
    unsigned int sub_chunk_1_size = sub_chunk_2_size + 36;

    int index = 0;

    //RIFF header

    //"RIFF"
    header[index++] = 0x52;
    header[index++] = 0x49;
    header[index++] = 0x46;
    header[index++] = 0x46;

    //Chunk size must be updated when all samples have been added
    header[index++] = sub_chunk_1_size & 0x000000FF;
    header[index++] = (sub_chunk_1_size & 0x0000FF00) >> 8;
    header[index++] = (sub_chunk_1_size & 0x00FF0000) >> 16;
    header[index++] = (sub_chunk_1_size & 0xFF000000) >> 24;

    //"WAVE"
    header[index++] = 0x57;
    header[index++] = 0x41;
    header[index++] = 0x56;
    header[index++] = 0x45;

    //fmt sub-chunk

    //"fmt "
    header[index++] = 0x66;
    header[index++] = 0x6D;
    header[index++] = 0x74;
    header[index++] = 0x20;

    //Sub chunk 1 size
    header[index++] = 16;
    header[index++] = 0;
    header[index++] = 0;
    header[index++] = 0;

    //Audio format (PCM)
    header[index++] = 1;
    header[index++] = 0;
    
    //Number of channels
    header[index++] = 1;
    header[index++] = 0;

    //Sample rate
    header[index++] = sample_rate & 0x000000FF;
    header[index++] = (sample_rate & 0x0000FF00) >> 8;
    header[index++] = (sample_rate & 0x00FF0000) >> 16;
    header[index++] = (sample_rate & 0xFF000000) >> 24;

    //Byte rate
    header[index++] = byte_rate & 0x000000FF;
    header[index++] = (byte_rate & 0x0000FF00) >> 8;
    header[index++] = (byte_rate & 0x00FF0000) >> 16;
    header[index++] = (byte_rate & 0xFF000000) >> 24;

    //Block align (16 bits * 1 (mono) = 2 bytes)
    header[index++] = 2;
    header[index++] = 0;

    //Bits per sample
    header[index++] = 16;
    header[index++] = 0;

    //data sub-chunk

    //"data"
    header[index++] = 0x64;
    header[index++] = 0x61;
    header[index++] = 0x74;
    header[index++] = 0x61;

    //Sub-chunk 2 size must be updated when all samples have been added
    header[index++] = sub_chunk_2_size & 0x000000FF;
    header[index++] = (sub_chunk_2_size & 0x0000FF00) >> 8;
    header[index++] = (sub_chunk_2_size & 0x00FF0000) >> 16;
    header[index++] = (sub_chunk_2_size & 0xFF000000) >> 24;
}

/**
 * build_data
 * parameter is array to be filled
 * the array must be initialized to enough items before calling this operation
 * returns the number of samples generated in this data
**/
int build_data(char wave_data[]) {

    double frequency = 440.0;
    unsigned int sample_count = 0;

    //srand(628443);
    for(int i = 0; i < 22050; i++){
        //Each sample is 16 bits = 2 bytes.
        double sample = cos(frequency * i * 3.142/44100.0);
        // unsigned int sample_16 = (int)(sample * 32767);
        // ADDED THE MIN SO THAT VALUES THE GO OVER THE TOP ARE FIXED TO 0xFFFF, rather than going around to 0.
        unsigned int sample_16 = std::min((int)(sample * 32767.0), 0xFFFF);
        wave_data[i*2] = sample_16 & 0x000000FF;
        wave_data[i*2 + 1] = (sample_16 & 0x0000FF00) >> 8;

        // RANDOM WHITE NOISE
        // wave_data[i*2] = (char)(rand() % 255);
        // wave_data[i*2 + 1] = (char)(rand() % 255);

        ++sample_count;
    }
    return sample_count;
}

int main(){

/***  DATA  ***/
    char wave_data[88200];
    unsigned int sample_count = build_data(wave_data);

/*** HEADER ***/
    char header[44];
    build_header(header, 44100, 2, sample_count);

/*** WRITE FILE ***/

    std::ofstream fout;
    fout.open("cpp_example.wav", std::ios::binary);
    fout.write(header, 44);
    fout.write(wave_data, 88200);
    fout.close();



    return 0;
}