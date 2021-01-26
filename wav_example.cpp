#include <iostream>
#include <math.h> /* cos */
#include <fstream>

using namespace std;

// Initialize global variables for header array and data array
char header[44];
char data[44100];

/* Add two functions here */
void makeHeader(int sampleRate, int noChannels, int bitsSample)
{
    int byteRate = sampleRate * noChannels * bitsSample / 8;
    int blockAlign = noChannels * bitsSample / 8;

    int int_field = 0;

    // Set ChunkID at first 4 bytes
    header[0] = 'R';
    header[1] = 'I';
    header[2] = 'F';
    header[3] = 'F';

    // Chunk size
    int chunkSize = (4 + 8 + 16 + 8 + 0);

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
    int subChunk1Size = (4 + 8 + 16 + 8 + 0);
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

    header[36] = 'd';
    header[37] = 'a';
    header[38] = 't';
    header[39] = 'a';

    // Byte rate
    header[40] = (unsigned char)(((unsigned int)byteRate & 0x000000FF));
    header[41] = (unsigned char)(((unsigned int)byteRate & 0x0000FF00) >> 8);
    header[42] = (unsigned char)(((unsigned int)byteRate & 0x00FF0000) >> 16);
    header[43] = (unsigned char)(((unsigned int)byteRate & 0xFF000000) >> 24);

    ofstream fout;
    fout.open("example2.wav", ios::binary);

    // fout << characters << endl;
    fout.write(header, 43);

    fout.close();
}

void addDataToArray(float sample)
{
    int sample_16 = (int)(sample * 32767);
}

int main()
{
    int sampleRate = 44100; // Sample rate in HZ
    int freq = 440;         // A above middle C
    double duration = 0.5;  // Length of tone - seconds
    int noChannels = 1;     // Mono

    int noSamples;
    noSamples = duration * sampleRate; // Total number of samples for file

    // Create the header for the wave file
    makeHeader(sampleRate, noChannels, 16);
    cout << header;

    for (int i = 0; i < noSamples; i++)
    {
        float sample = cos(freq * i * 3.142 / sampleRate);
        addDataToArray(sample);
    }

    return 0;
}
