#include <iostream>

using namespace std;

// Initialize global variables for header array and data array
char header[44];
char data[44100];

/* Add two functions here */
void makeHeader(int sampleRate, int noChannels, int bitsSample)
{
    int byteRate = sampleRate * noChannels * bitsSample/8;
    int blockAlign = noChannels * bitsSample/8;

    int int_field = 0;

    // Set ChunkID at first 4 bytes
    header[0] = 'R';
    header[1] = 'I';
    header[2] = 'F';
    header[3] = 'F';
}

void addDataToArray()
{

}

int main()
{
    int sampleRate = 44100;
    int freq = 440;
    double duration = 0.5;
    int noChannels = 1;

    int noSamples;
    noSamples = duration * sampleRate;
    return 0;
}