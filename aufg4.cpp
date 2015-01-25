#include <iostream>
#include <fstream>
#include <cmath>
#include "dftintegrate.h"

using namespace std;

class F
{
public:
  double operator  () (double x)
  {
    return exp (-2 * x * x);
  }
};

class G
{
  const double vf;
public:
    G ():vf (sqrt (3.14159265358979 / 2))
  {
  };
  double operator  () (double k)
  {
    return vf * exp (-k * k / 8);
  }
};



int
main ()
{
  F f;
  G g;
  double gr = 0, gi = 0;
  FFTint < F > h (f, -10, 10);
  ofstream file1 ("aufg4_interp.txt", ios::trunc);
  ofstream file2 ("aufg4_abw.txt", ios::trunc);


  for (double k = 0.; k < 10; k += 0.01)
    {
      h.interp (k, gr, gi);
      file1 << k << " " << gr << " " << gi << endl;
      file2 << k << " " << gr - g (k) << endl;
    }

  file1.close ();
  file2.close ();
}
