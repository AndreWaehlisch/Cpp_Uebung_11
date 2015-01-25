#include <iostream>
#include <fstream>
#include <cmath>

#include "fft.h"

using namespace std;

const int N_input = 3;
const double input[N_input] = { 0.05, 0.2, 0.6 };

double
f (const double x)
{
  double result = 0;

  for (int i = 0; i < N_input; i++)
    {
      result += sin (input[i] * x);
    }

  return result;
}

int
main ()
{
  const int N = 256;		// Anzahl der "Schritte"
  const double x_min = 0;
  const double x_max = 1000;
  const double dx = (x_max - x_min) / (N - 1);	// Schrittweite für gefordertes N
  double data[2 * N];		// Array für [Re(f(x)) ,Im(f(x)] und später die FFT davon
  //f_min = -f_max = -1/(2*dx) = -1/(2*1000/255) = -.1275

  ofstream inputfile ("aufg1_input.txt", ios::trunc);	// Werte von f(x)
  ofstream file ("aufg1.txt", ios::trunc);	// Ergebnisse
  

  for (int i = 0; i < 2 * N; i += 2)
    {
      data[i] = f (x_min + (i / 2) * dx);	// Re(f(x))
      data[i + 1] = 0;		// f(x) ist immer reell: Im(f(x)) ist 0.
      inputfile << data[i] << '\t' << data[i + 1] << endl;
    }

  VecDoub forward (2 * N, data);	// Koeffizienten in Vektor-Objekt schreiben
  fft (forward, 1);		// Schnelle Fourier-Transformation durchführen
  VecDoub backward (forward);	// Koeffizienten der inversen FFT (zur Überprüfung des Ergebnisses)
  fft (backward, -1);		// Inverse FFT durchführen
  
  // File-Output:
  //      1 Re der FFT
  //      2 Im der FFT
  //      3 Betrag der FFT
  //      4 Re der Inversen der FFT
  //      5 Im der Inversen der FFT
  //      6 Abweichung der Beträge zwischen ursprünglichen Input-f(x)-Werten und Inverser FFT
  for (int i = 0; i < 2 * N; i += 2)
    {
      backward[i] /= N; //FIX: Fehlt diese Normierung evt. in der Bibliothek? Ohne
      backward[i + 1] /= N;
      const double betrag = sqrt (forward[i] * forward[i] + forward[i + 1] * forward[i + 1]);
      const double diff = abs (sqrt (data[i] * data[i]) - sqrt (backward[i] * backward[i] + backward[i + 1] * backward[i + 1]));
      file << forward[i] << '\t' << forward[i + 1] << '\t' << betrag << '\t' << backward[i] << '\t' << backward[i + 1] << '\t' << diff << endl;
    }

  //Die Rücktransformation entspricht bis auf numerische Abweichungen der ursprünglichen Funktion (vgl. aufg1_diff.png) 
  // Datein schließen
  file.close ();
  inputfile.close ();

  cout << "Done." << endl;	// fertig!
}
