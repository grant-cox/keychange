/*
Grant Cox
grant.cox@uky.edu
CS315 Final Project
11/28/2017
*/

/*
1.window function (fairly trivial, e.g. Hanning)
2.FFT (FFTW would be a good choice but if licensing is an issue then go for Kiss FFT or similar)
3.calculate log magnitude of frequency domain components (trivial: log(sqrt(re * re + im * im))
*/

//frequency = 440 * 2^(n/12)
//so have user input n for +n or -n shift in pitch, pass 2^(n/12) to smbpitchshift

#include <iostream>
#include <string>
#include <armadillo>
#include "myFFT.cpp"
#include "./sigpack/sigpack/sigpack.h"
#include "smbPitchShift.cpp"
#include "AudioFile.h"
using namespace std;
using namespace arma;
using namespace sp;


int main(int argc, char* argv[]) {
     ///////////////////////////////////////////////////////////////////////////
     // open files
     //
     AudioFile<float> siginFloat;
     cout << "loading audio file(float)...";
     siginFloat.load("./fleetMono.wav");
     cout << "done." << endl;

     AudioFile<double> siginDouble;
     cout << "loading audio file(double)...";
     siginDouble.load("./fleetMono.wav");
     cout << "done." << endl;


     ///////////////////////////////////////////////////////////////////////////
     // print file information
     //
     cout << "\nSummary of input file: " << endl;
     siginFloat.printSummary();
     float pitchShift = stof(argv[1]);       cout << "\n\npitch shift factor:\t" << pitchShift <<"\n";
     int samples = siginFloat.getNumSamplesPerChannel(); cout << "samples:\t" << samples << "\n";
     long fftFrameSize = 2048;            cout << "fftFrameSize:\t" << fftFrameSize << "\n";
     long osamp = 32;                                    cout << "osamp\t" << osamp << "\n";
     float sampleRate = siginFloat.getSampleRate(); cout <<"sampleRate:\t" << sampleRate << "\n";



     ///////////////////////////////////////////////////////////////////////////
     // allocate data to float and double arrays
     //
     cout << "\nallocating memory and copying for pitch shifting...";
     float * dataFloat = new float[samples];
     for(int i = 0; i < samples; i++) dataFloat[i] = siginFloat.samples[0][i];

     int samplesDouble = siginDouble.getNumSamplesPerChannel();
     //double * dataDouble = new double[samplesDouble];
     vec dataDouble(samplesDouble);
     for(int i = 0; i < samplesDouble; i++) dataDouble[i] = siginDouble.samples[0][i];
     cout << "done." << endl;


     ///////////////////////////////////////////////////////////////////////////
     // do the pitch shifting
     //
     cout << "\npitch shifting...";
     //TODO keep track of time to compare algorithms
     //smbPitchShift(pitchShift, samples, fftFrameSize, osamp, sampleRate, dataFloat, dataFloat);


     //test sigpack transform
     cout << "\nsigpack transform..."<<endl;
     cx_vec dataDoubleSigFFT = fft(dataDouble);

     //test my transform
     cout << "\tcopying" << endl;
     cx_vec dataDoubleMyFFT(samplesDouble);
     for(int i = 0; i < samplesDouble; ++i) dataDoubleMyFFT[i] = dataDouble[i];
     cout << "Theres no way in hell im running my recursive transform" << endl;
     //recurseFFT(dataDoubleMyFFT);
     cout << "running optimized FFT" << endl;
     optimizedFFT(dataDoubleMyFFT);
     cout << "\n\n\n\n\ndone!" << endl;




     // for(int i = 0; i < samples; i++)
     //      siginFloat.samples[0][i] = dataFloat[i];
     //
     // cout << "writing to output file...";
     // siginFloat.save("./fleetMonoShift.wav");
     // cout << "done." << endl;
     //
     // cout << "deallocating and exiting..." << endl;
     //delete[] dataFloat;
     return 0;
}