#include <iostream>
#include <math.h>
#include <vector>
#include "gnuplot-iostream.h"

int main() {
   Gnuplot gp;
   std::vector<double> x;
   std::vector<double> sinx;
   for(int i=0;i<360;i++) {
	double xTemp=i*3.14/180;
	x.push_back(xTemp);
	double sinxTemp = sin(xTemp);
	sinx.push_back(sinxTemp);
   }
   gp << "plot '-' with lines\n";
   gp.send1d(sinx);
}
	
