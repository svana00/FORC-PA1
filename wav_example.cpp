#include <iostream>
#include <math.h> /* cos */
#include <fstream>

using namespace std;

// Initialize global variables for header array and data array
char header[44];

void makeHeader(int sampleRate, int noChannels, int bitsSample, int subChunk2Size, int chunkSize)
{
    int byteRate = sampleRate * noChannels * bitsSample / 8;
    int blockAlign = noChannels * bitsSample / 8;

    // Set ChunkID at first 4 bytes
    header[0] = 'R';
    header[1] = 'I';
    header[2] = 'F';
    header[3] = 'F';

    // Chunk size
    header[4] = (unsigned char)(((unsigned int)chunkSize & 0x000000FF));
    header[5] = (unsigned char)(((unsigned int)chunkSize & 0x0000FF00) >> 8);
    header[6] = (unsigned char)(((unsigned int)chunkSize & 0x00FF0000) >> 16);
    header[7] = (unsigned char)(((unsigned int)chunkSize & 0xFF000000) >> 24);

    // Format
    header[8] = 'W';
    header[9] = 'A';
    header[10] = 'V';
    header[11] = 'E';
    header[12] = 'f';
    header[13] = 'm';
    header[14] = 't';
    header[15] = ' ';

    // Subchunk 1 size
    int subChunk1Size = 16;
    header[16] = (unsigned char)(((unsigned int)subChunk1Size & 0x000000FF));
    header[17] = (unsigned char)(((unsigned int)subChunk1Size & 0x0000FF00) >> 8);
    header[18] = (unsigned char)(((unsigned int)subChunk1Size & 0x00FF0000) >> 16);
    header[19] = (unsigned char)(((unsigned int)subChunk1Size & 0xFF000000) >> 24);

    // Audio format
    header[20] = (unsigned char)(0x01);
    header[21] = (unsigned char)(0x00);

    // Number of channels
    header[22] = (unsigned char)(0x01);
    header[23] = (unsigned char)(0x00);

    // Sample rate
    header[24] = (unsigned char)(((unsigned int)sampleRate & 0x000000FF));
    header[25] = (unsigned char)(((unsigned int)sampleRate & 0x0000FF00) >> 8);
    header[26] = (unsigned char)(((unsigned int)sampleRate & 0x00FF0000) >> 16);
    header[27] = (unsigned char)(((unsigned int)sampleRate & 0xFF000000) >> 24);

    // Byte rate
    header[28] = (unsigned char)(((unsigned int)byteRate & 0x000000FF));
    header[29] = (unsigned char)(((unsigned int)byteRate & 0x0000FF00) >> 8);
    header[30] = (unsigned char)(((unsigned int)byteRate & 0x00FF0000) >> 16);
    header[31] = (unsigned char)(((unsigned int)byteRate & 0xFF000000) >> 24);

    // Block align
    header[32] = (unsigned char)(((unsigned int)blockAlign & 0x000000FF));
    header[33] = (unsigned char)(((unsigned int)blockAlign & 0x0000FF00) >> 8);

    // Bits per sample
    header[34] = (unsigned char)(((unsigned int)bitsSample & 0x000000FF));
    header[35] = (unsigned char)(((unsigned int)bitsSample & 0x0000FF00) >> 8);

    // Subchunk 2 id
    header[36] = 'd';
    header[37] = 'a';
    header[38] = 't';
    header[39] = 'a';

    // Byte rate
    header[40] = (unsigned char)(((unsigned int)subChunk2Size & 0x000000FF));
    header[41] = (unsigned char)(((unsigned int)subChunk2Size & 0x0000FF00) >> 8);
    header[42] = (unsigned char)(((unsigned int)subChunk2Size & 0x00FF0000) >> 16);
    header[43] = (unsigned char)(((unsigned int)subChunk2Size & 0xFF000000) >> 24);
}

int main(int argc, char *argv[])
{
    int freq;
    double duration;

    freq = atoi(argv[2]);
    duration = atof(argv[3]);

    int sampleRate = 44100; // Sample rate in HZ
    int noChannels = 1;     // Mono

    // Size of data array = sample rate * duration * 2 bytes per sample
    int dataSize = sampleRate * duration * 2;
    int noSamples;
    noSamples = duration * sampleRate; // Total number of samples for file

    // Calculate variables for header
    int subChunk2Size = (int)(duration * sampleRate * noChannels * 16 / 8);
    int chunkSize = subChunk2Size + 4 + 8 + 16 + 8;

    // Create the header for the wave file
    makeHeader(sampleRate, noChannels, 16, subChunk2Size, chunkSize);

    // Insert the data into the data array
    char dataArray[dataSize];
    for (int i = 0; i < noSamples; i++)
    {
        float sample = cos(freq * i * 3.142 / sampleRate);

        int sample_16 = (int)(sample * 32767);

        dataArray[2 * i] = (unsigned char)(((unsigned int)sample_16 & 0x000000FF));
        dataArray[(2 * i) + 1] = (unsigned char)(((unsigned int)sample_16 & 0x0000FF00) >> 8);
    }

    // Write header and data to wav file
    ifstream fin;
    ofstream fout;

    string fileName;

    fileName = argv[1];

    fout.open(fileName + ".wav", ios::binary);

    fout.write(header, 44);
    fout.write(dataArray, noSamples);

    fout.close();

    return 0;
}
