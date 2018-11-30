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

//#include "Wav2sig.h"
#include <iostream>
#include <string>
//#include <armadillo>
//#include <bitset>
//#include "./sigpack/sigpack/sigpack.h"
#include "smbPitchShift.cpp"
//#include "sig2wav.cpp"
#include "AudioFile.h"

//using namespace arma; //linear algebra
using namespace std;

int main(int argc, char* argv[]) {
     AudioFile<float> sigin;
     cout << "loading audio file...";
     sigin.load("./fleetMono.wav");
     cout << "done." << endl;



     cout << "\nSummary of input file: " << endl;
     sigin.printSummary();
     float pitchShift = 1.01;       cout << "\n\npitch shift factor:\t" << pitchShift <<"\n";
     int samples = sigin.getNumSamplesPerChannel(); cout << "samples:\t" << samples << "\n";
     long fftFrameSize = 2048;            cout << "fftFrameSize:\t" << fftFrameSize << "\n";
     long osamp = 32;                                    cout << "osamp\t" << osamp << "\n";
     float sampleRate = sigin.getSampleRate(); cout <<"sampleRate:\t" << sampleRate << "\n";



     cout << "\nallocating memory and copying for pitch shifting...";
     float * data = new float[samples];
     for(int i = 0; i < samples; i++)
          data[i] = sigin.samples[0][i];
     cout << "done." << endl;


     cout << "\npitch shifting...";
     smbPitchShift(pitchShift, samples, fftFrameSize, osamp, sampleRate, data, data);
     cout << "done." << endl;



     for(int i = 0; i < samples; i++)
          sigin.samples[0][i] = data[i];

     cout << "writing to output file...";
     sigin.save("./fleetMonoShift.wav");
     cout << "done." << endl;

     cout << "deallocating and exiting..." << endl;
     delete[] data;
     return 0;
}

// int main(int argc, char* argv[]) {
//      cout << "Spectrogram program starting..." << endl;
//      vector<string> fname = {"./fleetMono.wav"};
//      Wav2sig audio(fname);    //get samples from .wav file
//
//
//      cout << "\nGetting header data" << endl;
//      wav_header headerData = audio.getHeader();
//
//
//      uint64_t num_samples = headerData.numSamples;
//
//
//      //float * inData = new float[num_samples];
//      double * inData = new double[num_samples];
//      cout << "made array" << endl;
//
//
//      //copy to float array used in smbpitchshift
//      for(unsigned int i = 1; i < num_samples; ++i)    inData[i] = audio.data[i];
//
//
//      long fftFrameSize = 2048.0; //frame size for fft
//      long osamp = 32; //best quality
//      float sampleRate = headerData.SamplesPerSec; //fs
//
//      //use smbPitchShift to do pitch shifting
//      cout << "performing pitch shift...";
//      smbPitchShift(1.2, num_samples, fftFrameSize, osamp, sampleRate, inData, inData );
//      cout << "done." << endl;
//
//
//      for(int i = 110; i < 130; i++) {
//           cout << "orig: " << audio.data[i] << "\tpitchShift: " << inData[i] << endl;
//      }
//
//
//
//      //format output file name
//      string output_f_name = fname[0];
//      int dot_index;
//      for (int i = output_f_name.size() - 1; i >= 0 ; i--) {
//           if(output_f_name.at(i) == '.') {
//                     dot_index = i;
//                     break;
//           }
//      }
//      output_f_name = output_f_name.substr(0,dot_index-1) + "Shifted" +
//                          output_f_name.substr(dot_index,output_f_name.size());
//      cout << "outputting to " << output_f_name << endl;
//
//
//      //int sig2wav(float * dataOut, string ofname, wav_header header)
//      //sig2wav(outData, "./fleetMonoShift.wav", headerData);
//      // short int * intdata = new short int[num_samples];
//      // for (unsigned int i = 0; i < num_samples; i++ ) {
//      //      intdata[i] = *reinterpret_cast<short int*>(&inData[i]);
//      // }
//      // sig2wav("./fleetMonoShift.wav", headerData.numSamples, intdata, headerData.SamplesPerSec);
//
//
//      cout << "\nrunning exit procedures..." << endl;
//      delete[] inData;
//      //delete[] intdata;
//      //delete[] outData;
//      cout << "...done." << endl;
//      return 0;
// }