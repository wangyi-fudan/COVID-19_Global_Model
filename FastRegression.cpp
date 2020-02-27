/*
  Name:         FastRegression Class
  Copyright:    Free
  Author:       Wangyi
  Date: 		17-11-08 21:43
  Description:  z=ax+by+c
*/
#ifndef FastRegression_Included
#define FastRegression_Included

#include    <cmath>
#include    <vector>
using   namespace   std;
const   double  MinDelta=1e-30;

class   FastRegression{
private:
	double  sw, sx, sy, sz, xx,	xy,	xz,	yy,	yz,	zz;
public:
	double  a,  b,  c,	e,	n;
	void    clear(void);
	inline	void	push0(double	W,  double	Z);             			//  z=c
	inline	void	push1(double	W,  double	Z,	double  X);             //  z=ax
	inline	void	push2(double	W,  double	Z,	double	X);             //  z=ax+c
	inline	void	push3(double	W,  double	Z,	double	X,	double	Y); //  z=ax+by
	inline	void	push4(double	W,  double	Z,	double	X,	double	Y); //  z=ax+by+c
	double    predict0(void){ return  c;  }
	double    predict1(double	X){ return  a*X;  }
	double    predict2(double	X){ return  a*X+c;  }
	double    predict3(double	X,	double	Y){ return  a*X+b*Y;  }
	double    predict4(double	X,	double	Y){ return  a*X+b*Y+c;  }
	bool	estimate(int	T);
};

void    FastRegression::clear(void){
    sw=sx=sy=sz=xx=xy=xz=yy=yz=zz=a=b=c=e=n=0;
}

void	FastRegression::push0(double	W,  double	Z){
	sw+=W;	sz+=W*Z;	zz+=W*Z*Z;
}

void	FastRegression::push1(double	W,  double	Z,	double  X){
	sw+=W;	xx+=W*X*X;	xz+=W*X*Z;	zz+=W*Z*Z;
}

void	FastRegression::push2(double	W,  double	Z,	double  X){
	sw+=W;	sx+=W*X;    sz+=W*Z;
	xx+=W*X*X;  xz+=W*X*Z;  zz+=W*Z*Z;
}

void	FastRegression::push3(double	W,  double	Z,	double  X,	double	Y){
	xx+=W*X*X;  xy+=W*X*Y;  xz+=W*X*Z;
	yy+=W*Y*Y;  yz+=W*Y*Z;  zz+=W*Z*Z;
	sw+=W;
}

void	FastRegression::push4(double	W,  double	Z,	double  X,	double	Y){
	sw+=W;
	sx+=W*X;    sy+=W*Y;	sz+=W*Z;
	xx+=W*X*X;  xy+=W*X*Y;  xz+=W*X*Z;
	yy+=W*Y*Y;  yz+=W*Y*Z;  zz+=W*Z*Z;
}

bool    FastRegression::estimate(int	T){
	double  delta;
	
	switch(T){
	case    0:
		if(fabs(sw)<MinDelta)  return  false;
		n=sw;
		a=0;
		b=0;
		c=sz/sw;
		e=zz-c*sz;
	case    1:
		if(fabs(xx)<MinDelta)  return  false;
		n=sw;
		a=xz/xx;
		b=0;
		c=0;
		e=zz-a*xz;
		break;
		
	case    2:
		delta=sw*xx-sx*sx;
		if(fabs(delta)<MinDelta)   return  false;
		delta=1.0/delta;
		n=sw;
		a=delta*(sw*xz-sx*sz);
		b=0;
		c=delta*(sz*xx-sx*xz);
		e=zz-a*xz-c*sz;
		break;
		
	case    3:
		delta=xx*yy-xy*xy;
        if(fabs(delta)<MinDelta)   return  false;
        delta=1.0/delta;
       	n=sw;
        a=delta*(yy*xz-xy*yz);
        b=delta*(xx*yz-xy*xz);
        c=0;
		e=zz-a*xz-b*yz;
		break;
	case    4:
		delta=(yy*xx*sw-yy*sx*sx-xx*sy*sy-xy*xy*sw+2*sx*xy*sy);
		if(fabs(delta)<MinDelta)   return  false;
		delta=1.0/delta;
		n=sw;
		a=delta*(yy*xz*sw-yy*sx*sz-xz*sy*sy+sx*sy*yz+sy*xy*sz-xy*yz*sw);
		b=delta*(yz*xx*sw-yz*sx*sx-xy*xz*sw+xy*sx*sz-sy*xx*sz+sy*sx*xz);
		c=delta*(yy*xx*sz+sy*xy*xz-xx*sy*yz-yy*sx*xz-xy*xy*sz+sx*xy*yz);
		e=zz-a*xz-b*yz-c*sz;
		break;
	}
	return  true;
}
#endif

