#ifndef _DFTINT_H_
#define _DFTINT_H_

#include <cmath>
#include "interp_1d.h"
#include "fft.h"
#include "helfer.h"
using namespace std;

// **********
template <class T>
class FFTint
{
	T &func;
	double a, b;
	static const int M=256, NDFT=4096, MPOL=6;

	double delta;
	VecDoub data, endpts;

	void dftcor(const double w, const double delta, const double a, const double b, VecDoub& endpts, double& corre, double& corim, double& corfac);
	
	public:
	FFTint(T& funcc, const double aa, const double bb) : func(funcc), data(NDFT), endpts(8)
	{
		set(aa,bb);
	}

	void set(const double aa, const double bb);
	void interp(const double w, double& cosint, double& sinint);
};

// **********
template <class T>
void FFTint<T>::set(const double aa, const double bb)
{
	if (a != aa || b != bb)
	{
		a=aa;
		b=bb;
		delta=(b-a)/M;

		for (int j=0; j<M+1; ++j)
			data[j]=func(a+j*delta);

		for (int j=M+1; j<NDFT; ++j)
			data[j]=0.0;

		for (int j=0; j<4; ++j)
		{
			endpts[j]=data[j];
			endpts[j+4]=data[M-3+j];
		}
		realfft(data,1);
		data[1]=0.0;
	}
}

// **********
template <class T>
void FFTint<T>::interp(const double w, double& cosint, double& sinint)
{
	double en = w*delta * NDFT / (2.*M_PI) + 1.;
	int nn = MIN( MAX(int(en-0.5*MPOL+1.),1), NDFT/2-MPOL+1 );

	VecDoub cpol(MPOL),spol(MPOL),xpol(MPOL);

	for (int j=0; j<MPOL; ++j, ++nn)
	{
		cpol[j]=data[2*nn-2];
		spol[j]=data[2*nn-1];
		xpol[j]=nn;
	}

	double cdft = Poly_interp(xpol,cpol,MPOL).interp(en);
	double sdft = Poly_interp(xpol,spol,MPOL).interp(en);

	double corfac,corim,corre;
	dftcor(w,delta,a,b,endpts,corre,corim,corfac);

	cdft *= corfac;
	sdft *= corfac;
	cdft += corre;
	sdft += corim;

	double c = delta*cos(w*a);
	double s = delta*sin(w*a);

	cosint = c*cdft-s*sdft;
	sinint = s*cdft+c*sdft;
}

// **********
template <class T>
void FFTint<T>::dftcor(const double w, const double delta, const double a, const double b, VecDoub& endpts, double& corre, double& corim, double& corfac)
{
	double a0i,a0r,a1i,a1r,a2i,a2r,a3i,a3r,arg,c,cl,cr,s,sl,sr,t,t2,t4,t6, cth,ctth,spth2,sth,sth4i,stth,th,th2,th4,tmth2,tth4i;
	th=w*delta;

	if (a >= b || th < 0.0e0 || th > 3.1416e0)
		throw("bad arguments to dftcor");

	if (abs(th) < 5.0e-2)
	{
		t=th;
		t2=t*t;
		t4=t2*t2;
		t6=t4*t2;
		corfac=1.0-(11.0/720.0)*t4+(23.0/15120.0)*t6;
		a0r=(-2.0/3.0)+t2/45.0+(103.0/15120.0)*t4-(169.0/226800.0)*t6;
		a1r=(7.0/24.0)-(7.0/180.0)*t2+(5.0/3456.0)*t4-(7.0/259200.0)*t6;
		a2r=(-1.0/6.0)+t2/45.0-(5.0/6048.0)*t4+t6/64800.0;
		a3r=(1.0/24.0)-t2/180.0+(5.0/24192.0)*t4-t6/259200.0;
		a0i=t*(2.0/45.0+(2.0/105.0)*t2-(8.0/2835.0)*t4+(86.0/467775.0)*t6);
		a1i=t*(7.0/72.0-t2/168.0+(11.0/72576.0)*t4-(13.0/5987520.0)*t6);
		a2i=t*(-7.0/90.0+t2/210.0-(11.0/90720.0)*t4+(13.0/7484400.0)*t6);
		a3i=t*(7.0/360.0-t2/840.0+(11.0/362880.0)*t4-(13.0/29937600.0)*t6);
	}

	else
	{
		cth=cos(th);
		sth=sin(th);
		ctth=cth*cth-sth*sth;
		stth=2.0e0*sth*cth;
		th2=th*th;
		th4=th2*th2;
		tmth2=3.0e0-th2;
		spth2=6.0e0+th2;
		sth4i=1.0/(6.0e0*th4);
		tth4i=2.0e0*sth4i;
		corfac=tth4i*spth2*(3.0e0-4.0e0*cth+ctth);
		a0r=sth4i*(-42.0e0+5.0e0*th2+spth2*(8.0e0*cth-ctth));
		a0i=sth4i*(th*(-12.0e0+6.0e0*th2)+spth2*stth);
		a1r=sth4i*(14.0e0*tmth2-7.0e0*spth2*cth);
		a1i=sth4i*(30.0e0*th-5.0e0*spth2*sth);
		a2r=tth4i*(-4.0e0*tmth2+2.0e0*spth2*cth);
		a2i=tth4i*(-12.0e0*th+2.0e0*spth2*sth);
		a3r=sth4i*(2.0e0*tmth2-spth2*cth);
		a3i=sth4i*(6.0e0*th-spth2*sth);
	}

	cl=a0r*endpts[0]+a1r*endpts[1]+a2r*endpts[2]+a3r*endpts[3];
	sl=a0i*endpts[0]+a1i*endpts[1]+a2i*endpts[2]+a3i*endpts[3];
	cr=a0r*endpts[7]+a1r*endpts[6]+a2r*endpts[5]+a3r*endpts[4];
	sr= -a0i*endpts[7]-a1i*endpts[6]-a2i*endpts[5]-a3i*endpts[4];
	arg=w*(b-a);
	c=cos(arg);
	s=sin(arg);
	corre=cl+c*cr-s*sr;
	corim=sl+s*cr+c*sr;
}

#endif