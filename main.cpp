/*
Grant Cox
grant.cox@uky.edu
CS315 Final Project
11/28/2017
*/

/*
This program uses the Short Time Fourier Transform to shift a .wav input file 
up or down the number of semitones that the user requests. 

Usage: ./pitchshift filein fileout numkeys

where filein is an existing .wav file, fileout is the outputted .wav file, and 
numkeys is a positive or negative such that -12 <= numkeys <= 12. This is the 
number of keys to pitch shift the recording by.
*/

//frequency = 440 * 2^(n/12)

#include <iostream>
#include <string>
#include "smbPitchShift.cpp"
#include "AudioFile.h"
using namespace std;



int main(int argc, char* argv[]) {
     //input checking
     if(argc < 2 || argc > 4) {
          cerr << "Usage: ./pitchshift filein fileout numkeys" << endl;
          return 1;
     }
     
     ///////////////////////////////////////////////////////////////////////////
     // open files
     //
     //
     string fname = argv[1];
     AudioFile<float> siginFloat;
     cout << "loading audio file(float)...";
     siginFloat.load(fname);
     cout << "done." << endl;
     

     ///////////////////////////////////////////////////////////////////////////
     // get information for pitch shift, print file information
     //
     //
     cout << "\nSummary of input file: " << endl;
     siginFloat.printSummary();
     float pitchShift = stof(argv[3]);
     pitchShift = pow(2,pitchShift/12);
     cout << "shifting factor:\t" << pitchShift << endl;
     int samples = siginFloat.getNumSamplesPerChannel();
     long fftFrameSize = 2048;
     long oversampfac = 32;
     float sampleRate = siginFloat.getSampleRate();
     //TODO test the performance of the algorithm with different fftFrameSize,
     //TODO how does it perform with smaller or larger vals?


     ///////////////////////////////////////////////////////////////////////////
     // allocate data to float and double arrays
     //
     //
     cout << "\nallocating memory and copying for pitch shifting...";
     float * dataFloat = new float[samples];
     for(int i = 0; i < samples; i++) dataFloat[i] = siginFloat.samples[0][i];
     cout << "done." << endl;



     ///////////////////////////////////////////////////////////////////////////
     // do the pitch shifting, write output to input buffer
     //
     //
     cout << "\npitch shifting...";
     smbPitchShift(pitchShift, samples, fftFrameSize, oversampfac, sampleRate, dataFloat, dataFloat);
     cout << "done\n";


     ///////////////////////////////////////////////////////////////////////////
     // output to file
     //
     //
     for(int i = 0; i < samples; i++) siginFloat.samples[0][i] = dataFloat[i];

     string fnameout = argv[2];
     cout << "writing to output file...";
     siginFloat.save(fnameout);
     cout << "done." << endl;

     cout << "deallocating and exiting..." << endl;
     delete[] dataFloat;
     return 0;
}