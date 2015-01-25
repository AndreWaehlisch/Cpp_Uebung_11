#include <iostream>
#include <fstream>
#include <cmath>

#include "fft.h"

using namespace std;

const int N_input = 3;
const double input[N_input] = { 0.05, 0.2, 0.6 };

int
heaviside (double x)
{
  if (x < 0)
    return 0;
  else
    return 1;
}

int
h (double x)
{
  return heaviside (x - 45) - heaviside (x - 55);
}

int
main ()
{
  const int N = 256;		
  const double x_min = 0;	
  const double x_max = 1000;	
  const double dx = (x_max - x_min) / (N - 1);	

  ofstream inputfile ("aufg2_input.txt", ios::trunc);
  ofstream file ("aufg2.txt", ios::trunc);

  double data[2 * N];
  for (int i = 0; i < 2 * N; i += 2)
    {
      data[i] = h (x_min + (i / 2) * dx);
      data[i + 1] = 0;
      inputfile << data[i] << '\t' << data[i + 1] << endl;
    }

  VecDoub forward (2 * N, data);
  fft (forward, 1);	
  VecDoub backward (forward);
  fft (backward, -1);	

  // File-Output:
  //      1 Re der FFT
  //      2 Im der FFT
  //      3 Betrag der FFT
  //      4 Re der Inversen der FFT
  //      5 Im der Inversen der FFT
  //      6 Abweichung der Beträge zwischen ursprünglichen Input-f(x)-Werten und Inverser FFT

  for (int i = 0; i < 2 * N; i += 2)
    {
      backward[i] /= N;
      backward[i + 1] /= N;
      const double betrag = sqrt (forward[i] * forward[i] + forward[i + 1] * forward[i + 1]);
      const double diff = abs (sqrt (data[i] * data[i]) - sqrt (backward[i] * backward[i] + backward[i + 1] * backward[i + 1]));
      file << forward[i] << '\t' << forward[i + 1] << '\t' << betrag << '\t' << backward[i] << '\t' << backward[i + 1] << '\t' << diff << endl;
    }

  file.close ();
  inputfile.close ();

  cout << "Done." << endl;
}
