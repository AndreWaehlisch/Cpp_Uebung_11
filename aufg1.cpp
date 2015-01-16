#include <iostream>
#include <fstream>
#include <cmath>

#include "fft.h"

using namespace std;

const int N_input = 3;
const double input[N_input] = { 0.05, 0.2, 0.6 };

double f(const double x)
{
	double result = 0;

	for(int i=0; i < N_input; i++)
	{
		result += sin(input[i] * x);
	}

	return result;
}

int main()
{
	const int N = 256;	// Anzahl der "Schritte"
	const double x_min = 0;	// f_min=-2.52786 z.B. bei x=-91.191
	const double x_max = 1000;	// f_max=2.5789 z.B bei x=31.4728
	const double dx = (x_max - x_min) / (N-1);	// Schrittweite für gefordertes N

	ofstream inputfile("aufg1_input.txt", ios::trunc); // Werte von f(x)

	double coeff[2*N];	// Array für [Re(f(x)) ,Im(f(x)] und später die FFT davon

	for(int i=0; i < 2*N; i+=2)
	{
		coeff[i] = f( x_min + (i/2)*dx );	// Re(f(x))
		coeff[i+1] = 0;	// f(x) ist immer reell: Im(f(x)) ist 0. Könnte hier auch auskommentiert werden, da bei der Erzeugung des Arrays bereits alle Elemente zu 0 gesetzt werden.

		inputfile << coeff[i] << '\t' << coeff[i+1] << endl;
	}

	VecDoub forward(2*N, coeff);	// Koeffizienten in Vektor-Objekt schreiben

	fft(forward, 1);	// Schnelle Fourier-Transformation durchführen

	VecDoub backward(forward); // Koeffizienten der inversen FFT (zur Überprüfung des Ergebnisses)

	fft(backward, -1); // Inverse FFT durchführen

	ofstream file("aufg1.txt", ios::trunc);	// Datei öffnen

	// File-Output:
	//	1 Re der FFT
	//	2 Im der FFT
	//	3 Betrag der FFT
	//	4 Re der Inversen der FFT
	//	5 Im der Inversen der FFT
	//	6 Abweichung der Beträge zwischen ursprünglichen Input-f(x)-Werten und Inverser FFT
	for(int i=0; i < 2*N; i+=2)
	{
		const double betrag = sqrt( forward[i]*forward[i] + forward[i+1]*forward[i+1] );
		const double diff = abs( sqrt(coeff[i]*coeff[i]) - sqrt(backward[i]*backward[i] + backward[i+1]*backward[i+1]) );
		file << forward[i] << '\t' << forward[i+1] << '\t' << betrag << '\t' << backward[i] << '\t' << backward[i+1] << '\t' << diff << endl;
	}

	// Datein schließen
	file.close();
	inputfile.close();

	cout << "Done." << endl;	// fertig!
}
