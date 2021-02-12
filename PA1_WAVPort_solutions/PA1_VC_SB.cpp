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
 * build_X_data
 * first parameter is array to be filled
 * the array must be initialized to enough items before calling this operation
**/
void build_note_data(char wave_data[], double frequency, double sample_rate, unsigned int sample_count, unsigned int sample_offset = 0) {

    for(int i = 0; i < sample_count; i++){
        //Each sample is 16 bits = 2 bytes.
        double sample = cos(frequency * i * 3.142/sample_rate);
        // unsigned int sample_16 = (int)(sample * 32767);
        // ADDED THE MIN SO THAT VALUES THAT GO OVER THE TOP ARE FIXED TO 0xFFFF, rather than going around to 0.
        unsigned int sample_16 = std::min((int)(sample * 30000.0), 0xFFFF);
        wave_data[(sample_offset + i) * 2] = sample_16 & 0x000000FF;
        wave_data[(sample_offset + i) * 2 + 1] = (sample_16 & 0x0000FF00) >> 8;
    }
}

void build_silence_data(char wave_data[], unsigned int sample_count, unsigned int sample_offset = 0) {

    for(int i = 0; i < sample_count; i++){
        //Each sample is 16 bits = 2 bytes.
        wave_data[(sample_offset + i) * 2] = 0x00;
        wave_data[(sample_offset + i) * 2 + 1] = 0x00;
    }
}

void add_note_data(char wave_data[], double frequency, double sample_rate, unsigned int sample_count, unsigned int sample_offset = 0) {

    for(int i = 0; i < sample_count; i++){
        //Each sample is 16 bits = 2 bytes.
        double sample = cos(frequency * i * 3.142/sample_rate);
        unsigned int sample_16 = wave_data[(sample_offset + i) * 2] + (wave_data[(sample_offset + i) * 2 + 1] << 8);
        // sample_16 = std::min((int)sample_16 + std::min((int)(sample * 32767.0), 0xFFFF), 0xFFFF);
        sample_16 += std::min((int)(sample * 16000.0), 0x7FFF);
        wave_data[(sample_offset + i) * 2] = sample_16 & 0x000000FF;
        wave_data[(sample_offset + i) * 2 + 1] = (sample_16 & 0x0000FF00) >> 8;
    }
}

unsigned int calculate_frequency(char code, unsigned int octave){
    unsigned int frequency;
    switch(code){
        case 'a': frequency = 440; break;
        case 'A': frequency = 466; break;
        case 'b': frequency = 494; break;
        case 'c': case 'B': frequency = 523; break;
        case 'C': frequency = 554; break;
        case 'd': frequency = 587; break;
        case 'D': frequency = 622; break;
        case 'e': frequency = 659; break;
        case 'f': case 'E': frequency = 698; break;
        case 'F': frequency = 740; break;
        case 'g': frequency = 784; break;
        case 'G': frequency = 831; break;
    }
    if(octave == 0){
        frequency = frequency / 2;
    }
    else for(int i = 1; i < octave; i++){
        frequency = frequency * 2;
    }
    return frequency;
}

#define BUFFER_SIZE 524288 //enough to hold 5 seconds
//(actually 441.000 is enough, but I like whole powers of 2)
//262144
//131072

int main(int argc, char *argv[]){
    char input_file_name[48];
    input_file_name[0] = '\0';
    char input_file_name_2[48];
    input_file_name_2[0] = '\0';
    char wav_file_name[128];
    bool reading_two_files = false;

    if(argc >= 2){
        if(strcmp(argv[1], "-h") == 0){
            reading_two_files = true;
            if(argc >= 3){
                strcpy(input_file_name, argv[2]);
            }
            if(argc >= 4){
                strcpy(input_file_name_2, argv[3]);
            }
        }
        else{
            strcpy(input_file_name, argv[1]);
        }
    }
    if(input_file_name[0] == '\0') {
        std::cout << "Input file     : ";
        std::cin >> input_file_name;
    }
    else {
        std::cout << "Input file:       " << input_file_name << std::endl;
    }
    if(reading_two_files){
        if(input_file_name_2[0] == '\0'){
            std::cout << "Other input file: ";
            std::cin >> input_file_name_2;
        }
        else {
            std::cout << "Other input file: " << input_file_name_2 << std::endl;
        }
    }


    unsigned int sample_rate = 44100;
    unsigned int sample_size = 2;
    unsigned int total_sample_count = 0;

/***  INITIALIZE INFORMATION  ***/
    char wave_data[BUFFER_SIZE];  //initialize huge, but this is probably only enough for a few seconds
    unsigned int samples_per_bar;
    int frequency;
    double length;
    unsigned int sample_count;
    unsigned int sample_offset = 0;
    unsigned int numerator;
    unsigned int denominator;
    unsigned int octave;
    char code;

    std::ifstream fin;
    std::ifstream fin_2;
    fin.open(input_file_name);
    if(reading_two_files){
        fin_2.open(input_file_name_2);
    }

    fin >> wav_file_name;

    if(reading_two_files){
        char wav_file_name_2[48];
        fin_2 >> wav_file_name_2;
        strcat(strcat(wav_file_name, "_"), wav_file_name_2);
    }
    int name_length = strlen(wav_file_name);
    wav_file_name[name_length] = '.';
    wav_file_name[name_length + 1] = 'w';
    wav_file_name[name_length + 2] = 'a';
    wav_file_name[name_length + 3] = 'v';
    wav_file_name[name_length + 4] = '\0';

    std::cout << wav_file_name << std::endl;

    int beats_per_minute;
    fin >> beats_per_minute;
    std::cout << "BPM: " << beats_per_minute << std::endl;
    samples_per_bar = ((sample_rate * 60) / beats_per_minute) * 4;

/*** OPEN WRITE FILE ***/

    std::ofstream fout;
    fout.open(wav_file_name, std::ios::binary);

/*** WRITE HEADER ***/
    char header[44];
    build_header(header, sample_rate, sample_size);
    fout.write(header, 44);

/***  BUILD DATA  ***/
    if(reading_two_files){
        for(int i = 0; i < BUFFER_SIZE; i++){
            wave_data[i] = 0;
        }
        unsigned int other_total_sample_count = 0;
        bool both_files_finished = false;
        bool using_file_2 = false;
        bool other_file_is_finished = false;
        unsigned int tmp;

        unsigned int other_sample_offset = 0;
        fin_2 >> beats_per_minute;
        std::cout << "BPM (2): " << beats_per_minute << std::endl;
        unsigned int other_samples_per_bar = ((sample_rate * 60) / beats_per_minute) * 4;

        fin >> code;

        while(!both_files_finished){
            if(using_file_2){
                if(code != 's'){
                    fin_2 >> octave;
                    frequency = calculate_frequency(code, octave);
                }
                fin_2 >> numerator >> denominator;
            }
            else{
                if(code != 's'){
                    fin >> octave;
                    frequency = calculate_frequency(code, octave);
                }
                fin >> numerator >> denominator;
            }

            sample_count = (samples_per_bar * numerator) / denominator;
            if((sample_count + sample_offset) * sample_size > BUFFER_SIZE){
                std::cout << "Writing intermediate buffer." << std::endl;
                fout.write(wave_data, sample_offset * sample_size);
                if(sample_offset < other_sample_offset){
                    other_sample_offset -= sample_offset;
                    std::cout << "CHUNK DELTA: " << other_sample_offset << std::endl;
                    std::cout << "Using file 2: " << using_file_2 << std::endl;
                }
                else{
                    if(sample_offset == other_sample_offset){
                        std::cout << "EQUAL LENGTH CHUNKS!!!" << std::endl;
                        std::cout << "Using file 2: " << using_file_2 << std::endl;
                    }
                    else{
                        std::cout << "OTHER CHUNK SHORTER!!!" << std::endl;
                    }
                    other_sample_offset = 0;
                }
                for(int i = 0; i < (other_sample_offset * sample_size); i++){
                    wave_data[i] = wave_data[i + (sample_offset * sample_size)];
                }
                for(int i = (other_sample_offset * sample_size); i < BUFFER_SIZE; i++){
                    wave_data[i] = 0;
                }
                sample_offset = 0;
            }

            //different for notes and silence
            if(code != 's'){ // if 's' just do nothing, all the bytes have been initialized to 0
                add_note_data(wave_data, frequency, sample_rate, sample_count, sample_offset);
            }

            sample_offset += sample_count;
            total_sample_count += sample_count;

            if(!other_file_is_finished && sample_offset > other_sample_offset){ // Must switch - always read into smaller one.
                using_file_2 = !using_file_2;
            }
            if(using_file_2){
                fin_2 >> code;
                if(!other_file_is_finished && fin_2.eof()){
                    using_file_2 = false;
                    other_file_is_finished = true;
                    fin >> code;
                }
            }
            else{
                fin >> code;
                if(!other_file_is_finished && fin.eof()){
                    using_file_2 = true;
                    other_file_is_finished = true;
                    fin_2 >> code;
                }
            }
            if(!other_file_is_finished && sample_offset > other_sample_offset){
                tmp = sample_offset;
                sample_offset = other_sample_offset;
                other_sample_offset = tmp;
                tmp = samples_per_bar;
                samples_per_bar = other_samples_per_bar;
                other_samples_per_bar = tmp;
                tmp = total_sample_count;
                total_sample_count = other_total_sample_count;
                other_total_sample_count = tmp;
            }

            if(fin.eof() && fin_2.eof()){
                both_files_finished = true;
            }
        }
        total_sample_count = std::max(total_sample_count, other_total_sample_count);
        sample_offset = std::max(sample_offset, other_sample_offset);
    }
    else{
        fin >> code;

        while(!fin.eof()){
            if(code != 's'){ //any code except 's' - only notes
                fin >> octave;
                frequency = calculate_frequency(code, octave);
            }

            //This applies to both silence and notes, the size of the data has only to do with the length
            fin >> numerator >> denominator;
            sample_count = (samples_per_bar * numerator) / denominator;
            if((sample_count + sample_offset) * sample_size > BUFFER_SIZE){
                std::cout << "Writing intermediate buffer." << std::endl;
                fout.write(wave_data, sample_offset * sample_size);
                sample_offset = 0;
            }

            //different for notes and silence
            if(code == 's'){
                build_silence_data(wave_data, sample_count, sample_offset);
            }
            else{
                build_note_data(wave_data, frequency, sample_rate, sample_count, sample_offset);
            }

            sample_offset += sample_count;
            total_sample_count += sample_count;
            fin >> code;
        }
    }

    // WRITE FINAL BUFFER
    std::cout << "Writing final buffer." << std::endl;
    fout.write(wave_data, sample_offset * sample_size);

    fin.close();

/*** UPDATE HEADER ***/
    build_header(header, sample_rate, sample_size, total_sample_count);
    fout.seekp(0, std::ios::beg);
    fout.write(header, 44);

/*** CLOSE WRITE FILE ***/

    fout.close();

    return 0;
}
