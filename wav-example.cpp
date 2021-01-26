#include <iostream>

using namespace std;

char header[44];
char data[44100];

/* Add two functions here */
void makeHeader()
{
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
    return 0;
}