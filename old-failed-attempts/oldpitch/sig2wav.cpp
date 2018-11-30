//https://gasstationwithoutpumps.wordpress.com/2011/10/08/making-wav-files-from-c-programs/


/* make_wav.c
 * Creates a WAV file from an array of ints.
 * Output is monophonic, signed 16-bit samples
 * copyright
 * Fri Jun 18 16:36:23 PDT 2010 Kevin Karplus
 * Creative Commons license Attribution-NonCommercial
 *  http://creativecommons.org/licenses/by-nc/3.0/
 */

#include <stdio.h>
#include <assert.h>
#include <string>
using namespace std;

inline void write_little_endian(unsigned int word, int num_bytes, FILE *wav_file) {
     unsigned buf;
     while(num_bytes>0) {
          buf = word & 0xff;
          fwrite(&buf, 1,1, wav_file);
          num_bytes--;
          word >>= 8;
     }
}

/* information about the WAV file format from
    http://ccrma.stanford.edu/courses/422/projects/WaveFormat/
 */

inline void sig2wav(string filename, unsigned long num_samples, short int * data, int s_rate) {
     FILE* wav_file;
     unsigned int sample_rate;
     unsigned int num_channels;
     unsigned int bytes_per_sample;
     unsigned int byte_rate;
     unsigned long i;    /* counter for samples */

     num_channels = 1;   /* monoaural */
     bytes_per_sample = 2;

     if (s_rate<=0) sample_rate = 44100;
     else sample_rate = (unsigned int) s_rate;

     byte_rate = sample_rate*num_channels*bytes_per_sample;

     wav_file = fopen(filename.c_str(), "w");
     assert(wav_file);   /* make sure it opened */

     /* write RIFF header */
     fwrite("RIFF", 1, 4, wav_file);
     write_little_endian(36 + bytes_per_sample* num_samples*num_channels, 4, wav_file);
     fwrite("WAVE", 1, 4, wav_file);

     /* write fmt  subchunk */
     fwrite("fmt ", 1, 4, wav_file);
     write_little_endian(16, 4, wav_file);   /* SubChunk1Size is 16 */
     write_little_endian(1, 2, wav_file);    /* PCM is format 1 */
     write_little_endian(num_channels, 2, wav_file);
     write_little_endian(sample_rate, 4, wav_file);
     write_little_endian(byte_rate, 4, wav_file);
     write_little_endian(num_channels*bytes_per_sample, 2, wav_file);  /* block align */
     write_little_endian(8*bytes_per_sample, 2, wav_file);  /* bits/sample */

     /* write data subchunk */
     fwrite("data", 1, 4, wav_file);
     write_little_endian(bytes_per_sample* num_samples*num_channels, 4, wav_file);
     for (i=0; i< num_samples; i++)
         write_little_endian((unsigned int)(data[i]),bytes_per_sample, wav_file);

     fclose(wav_file);
}









// #include <cmath>
// #include <fstream>
// #include <iostream>
// #include <stdio.h>
// #include "Wav2sig.h"
//
// using namespace std;

// //output for little endian formatting (16 bit PCM .wav type)
// inline void write_word(ostream&outs, unsigned int word, int num_bytes) {
//      unsigned buf; //buffer to hold integers to write
//      while (num_bytes>0) {    //while there are bytes remaining to write
//           buf = word & 0xff;  //bitwise and to keep 8 significant bits (1 byte)
//           //fwrite(&buf, 1,1, outs);  //write to file
//           outs.put(static_cast<char>(buf));
//           num_bytes--;                  //decrement byte count
//           word >>= 8;    //rshift word by eight bits
//      }
// }
// // template <typename Word>
// // std::ostream& write_word( std::ostream& outs, Word value, unsigned size = sizeof( Word ) ) {
// //      for (; size; --size, value >>= 8) //iterate until size = 0
// //           outs.put( static_cast <char> (value & 0xFF) ); //output the 1 byte of the word
// //      return outs;
// // }
//
// //output .wav file from sampled signals
// inline int sig2wav(float * dataOut, string ofname, wav_header header) {
//      if (ofname == "") {
//           cerr << "ERR: no filename given" << endl;
//           return 1;
//      }
//      if (dataOut == nullptr){
//           cerr << "ERR: null pointer for dataout" << endl;
//           return 1;
//      }
//
//      ofstream wave( ofname, ios::binary );
//
//      // Write the file headers
//      wave << "RIFF----WAVEfmt ";     // (chunk size to be filled in later)
//      write_word( wave,     header.ChunkSize , 4 );  // no extension data
//      wave << "WAVE";
//
//      wave << "fmt ";
//      write_word( wave,      header.Subchunk2Size, 4 );//subchunk size
//      write_word( wave,      1, 2 );  // PCM - integer samples
//      write_word( wave,      1, 2 );  // one channel
//      write_word( wave,  header.SamplesPerSec , 4 );  // samples per second (Hz)
//      write_word( wave,  header.bytesPerSec , 4 );  // bytes/s
//      write_word( wave,      4, 2 );  // data block size (size of two integer samples, one for each channel, in bytes)
//      write_word( wave,     16, 2 );  // number of bits per sample (use a multiple of 8)
//
//      // Write the data chunk header
//      wave << "data----";  // (chunk size to be filled in later)
//      write_word( wave,  header.Subchunk2Size, 4);
//
//      union floatint
//      {
//           float f;
//           int i;
//      } out;
//
//      //write sample data
//      for(unsigned int i = 0 ; i < header.numSamples; ++i) {
//           out.f = dataOut[i];
//           //write_word( wave, (unsigned int)dataOut[i], 2 );
//           write_word( wave, out.i, 2 );
//           cout << out.f << " " << out.i << " ";
//      }
//      wave.close();
//      cout << "done?" << endl;
//
//      return 0;
// }
//
//
// /*using namespace std;
//
// namespace little_endian_io
// {
//   template <typename Word>
//   std::ostream& write_word( std::ostream& outs, Word value, unsigned size = sizeof( Word ) )
//   {
//     for (; size; --size, value >>= 8)
//       outs.put( static_cast <char> (value & 0xFF) );
//     return outs;
//   }
// }
// using namespace little_endian_io;
//
// int main()
// {
//   ofstream f( "example.wav", ios::binary );
//
//   // Write the file headers
//   f << "RIFF----WAVEfmt ";     // (chunk size to be filled in later)
//   write_word( f,     16, 4 );  // no extension data
//   write_word( f,      1, 2 );  // PCM - integer samples
//   write_word( f,      2, 2 );  // two channels (stereo file)
//   write_word( f,  44100, 4 );  // samples per second (Hz)
//   write_word( f, 176400, 4 );  // (Sample Rate * BitsPerSample * Channels) / 8
//   write_word( f,      4, 2 );  // data block size (size of two integer samples, one for each channel, in bytes)
//   write_word( f,     16, 2 );  // number of bits per sample (use a multiple of 8)
//
//   // Write the data chunk header
//   size_t data_chunk_pos = f.tellp();
//   f << "data----";  // (chunk size to be filled in later)
//
//   // Write the audio samples
//   // (We'll generate a single C4 note with a sine wave, fading from left to right)
//   constexpr double two_pi = 6.283185307179586476925286766559;
//   constexpr double max_amplitude = 32760;  // "volume"
//
//   double hz        = 44100;    // samples per second
//   double frequency = 261.626;  // middle C
//   double seconds   = 2.5;      // time
//
//   int N = hz * seconds;  // total number of samples
//   for (int n = 0; n < N; n++)
//   {
//     double amplitude = (double)n / N * max_amplitude;
//     double value     = sin( (two_pi * n * frequency) / hz );
//     write_word( f, (int)(                 amplitude  * value), 2 );
//     write_word( f, (int)((max_amplitude - amplitude) * value), 2 );
//   }
//
//   // (We'll need the final file size to fix the chunk sizes above)
//   size_t file_length = f.tellp();
//
//   // Fix the data chunk header to contain the data size
//   f.seekp( data_chunk_pos + 4 );
//   write_word( f, file_length - data_chunk_pos + 8 );
//
//   // Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
//   f.seekp( 0 + 4 );
//   write_word( f, file_length - 8, 4 );
// }*/