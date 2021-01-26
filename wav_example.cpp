#include <iostream>

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

    int chunkSize = (4 + 8 + 16 + 8 + 0);
    header[7] = (unsigned char)(((unsigned int)chunkSize & 0xFF000000) >> 24);
    header[6] = (unsigned char)(((unsigned int)chunkSize & 0x00FF0000) >> 16);
    header[5] = (unsigned char)(((unsigned int)chunkSize & 0x0000FF00) >> 8);
    header[4] = (unsigned char)(((unsigned int)chunkSize & 0x000000FF));

    header[8] = 'W';
    header[9] = 'A';
    header[10] = 'V';
    header[11] = 'E';
    header[12] = 'f';
    header[13] = 'm';
    header[14] = ' ';
}

void addDataToArray()
{
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

    return 0;
}