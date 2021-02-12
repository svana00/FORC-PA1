#include <fstream>
#include <math.h>
//#include <algorithm>
#include <iostream>
#include <string.h>

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
 * first parameter is array to be filled
 * the array must be initialized to enough items before calling this operation
**/
void build_data(char wave_data[], double frequency, double sample_rate, unsigned int sample_count) {

    //srand(628443);
    for(int i = 0; i < sample_count; i++){
        //Each sample is 16 bits = 2 bytes.
        double sample = cos(frequency * i * 3.142/sample_rate);
        // unsigned int sample_16 = (int)(sample * 32767);
        // ADDED THE MIN SO THAT VALUES THE GO OVER THE TOP ARE FIXED TO 0xFFFF, rather than going around to 0.
        unsigned int sample_16 = std::min((int)(sample * 32767.0), 0xFFFF);
        wave_data[i*2] = sample_16 & 0x000000FF;
        wave_data[i*2 + 1] = (sample_16 & 0x0000FF00) >> 8;
    }
}


void build_harmony(char wave_data[], double freq1, double freq2, double sample_rate, unsigned int sample_count) {

    //srand(628443);
    for(int i = 0; i < sample_count; i++){
        //Each sample is 16 bits = 2 bytes.
        double sample = 0.5 * (cos(freq1 * i * 3.142/sample_rate) + cos(freq2 * i * 3.142/sample_rate));
        // unsigned int sample_16 = (int)(sample * 32767);
        // ADDED THE MIN SO THAT VALUES THE GO OVER THE TOP ARE FIXED TO 0xFFFF, rather than going around to 0.
        unsigned int sample_16 = std::min((int)(sample * 32767.0), 0xFFFF);
        wave_data[i*2] = sample_16 & 0x000000FF;
        wave_data[i*2 + 1] = (sample_16 & 0x0000FF00) >> 8;
    }
}

int main(int argc, char *argv[]){
    char file_name[48];
    int frequency;
    int frequency2 = 0;
    double length;

    if(argc >= 6 && strcmp(argv[1], "-h") == 0){
        strcpy(file_name, argv[2]);
        frequency = atoi(argv[3]);
        frequency2 = atoi(argv[4]);
        length = atof(argv[5]);
    }
    else if(argc >= 4){
        strcpy(file_name, argv[1]);
        frequency = atoi(argv[2]);
        length = atof(argv[3]);
    }
    else {
        std::cout << "Output file name: ";
        std::cin >> file_name;

        std::cout << "Note frequency:   ";
        std::cin >> frequency;

        std::cout << "Note length:      ";
        std::cin >> length;
    }
    int name_length = strlen(file_name);
    file_name[name_length] = '.';
    file_name[name_length + 1] = 'w';
    file_name[name_length + 2] = 'a';
    file_name[name_length + 3] = 'v';
    file_name[name_length + 4] = '\0';

    std::cout << file_name << " " << frequency << " " << length;


    unsigned int sample_rate = 44100;
    unsigned int sample_size = 2;

/***  DATA  ***/
    unsigned int sample_count = (unsigned int)(sample_rate * length);
    char wave_data[500000];  //initialize huge, but this is probably only enough for a few seconds
    if(frequency2 == 0){
        build_data(wave_data, frequency, sample_rate, sample_count);
    }
    else{
        build_harmony(wave_data, frequency, frequency2, sample_rate, sample_count);
    }

/*** HEADER ***/
    char header[44];
    build_header(header, sample_rate, sample_size, sample_count);

/*** WRITE FILE ***/

    std::ofstream fout;
    fout.open(file_name, std::ios::binary);
    fout.write(header, 44);
    fout.write(wave_data, sample_count * sample_size);
    fout.close();

    return 0;
}