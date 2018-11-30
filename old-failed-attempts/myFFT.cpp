//reference from https://rosettacode.org/wiki/Fast_Fourier_transform#C.2B.2B

#include <cmath>
#include <complex>
#include <vector>
#include <armadillo>

using namespace std;
using namespace arma;

const double PI = 3.1415926535897932;


//cooley-tukey
// void recurseFFT(vector<complex<double>>& sig) {
//      if (sig.size() == 1) return;
//
//      vector<complex<double>> even, odd;
//      for(unsigned int i = 0; i < sig.size() / 2; i++)
//           if(i % 2 == 0)
//                even.push_back(sig[i]);
//           else
//                odd.push_back(sig[i]);
//
//      recurseFFT(even); recurseFFT (odd);
//
//      int N = sig.size();
//      for(unsigned int i = 0; i < sig.size() / 2; i++) {
//           complex<double> comp = polar(1.0, -2 * PI * i / N) * odd[i];
//           sig[i] = even[i] + comp;
//           sig[i + N/2] = even[i] - comp;
//      }
// }


////////////////////////////////////////////////////////////////////////////////
//
//
//
//
void recurseFFT(cx_vec& sig) {
     if (sig.size() == 1) return;

     cx_vec even(sig.size()), odd(sig.size());
     for(unsigned int i = 0; i < sig.size() / 2; i++)
          if(i % 2 == 0)
               even[i] = sig[i];
          else
               odd[i] = sig[i];

     recurseFFT(even); recurseFFT(odd);

     int N = sig.size();
     for(unsigned int i = 0; i < sig.size() / 2; i++) {
          complex<double> comp = polar(1.0, -2 * PI * i / N) * odd[i];
          sig[i] = even[i] + comp;
          sig[i + N/2] = even[i] - comp;
     }
}





////////////////////////////////////////////////////////////////////////////////
//
//
//
//
void optimizedFFT(cx_vec &x) {
	// DFT
	unsigned int N = x.size(), k = N, n;
	double thetaT = 3.14159265358979323846264338328L / N;
	cx_double phiT = cx_double(cos(thetaT), -sin(thetaT)), T;

     while (k > 1) {
		n = k;
		k >>= 1;
		phiT = phiT * phiT;
		T = 1.0L;
		for (unsigned int l = 0; l < k; l++)  {
			for (unsigned int a = l; a < N; a += n)   {
                    cout << a << "\r";
				unsigned int b = a + k;
				cx_double t = x[a] - x[b];
				x[a] += x[b];
				x[b] = t * T;
			}
			T *= phiT;
		}
	}
	// Decimate
	unsigned int m = (unsigned int)log2(N);
	for (unsigned int a = 0; a < N ; a++) {
		unsigned int b = a;
		// Reverse bits
		b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
		b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
		b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
		b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
		b = ((b >> 16) | (b << 16)) >> (32 - m);
		if (b > a) {
			cx_double t = x[a];
			x[a] = x[b];
			x[b] = t;
		}
	}
	//// Normalize (This section make it not working correctly)
	//cx_double f = 1.0 / sqrt(N);
	//for (unsigned int i = 0; i < N; i++)
	//	x[i] *= f;
}




////////////////////////////////////////////////////////////////////////////////
// inverse fft (in-place)
//
//
//
void ifft(cx_vec& s)
{
    // conjugate the complex numbers
    s = conj(s);

    // forward fft
    optimizedFFT( s );

    // conjugate the complex numbers again
    s = conj(s);

    // scale the numbers
    s /= s.size();
}