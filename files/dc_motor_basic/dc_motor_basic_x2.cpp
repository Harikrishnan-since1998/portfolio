// Automatically generated C++ file on Tue Jul 21 18:55:37 2026
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD -o dc_motor_basic_x2.cpp kernel32.lib

#include <cmath>

double Ts, Ra, La, J, B, Kb;
double alpha1,beta1,alpha2,beta2;
double x1,y1,x2,y2,x1prev=0,y1prev=0,x2prev=0,y2prev=0;

union uData
{
   bool b;
   char c;
   unsigned char uc;
   short s;
   unsigned short us;
   int i;
   unsigned int ui;
   float f;
   double d;
   long long int i64;
   unsigned long long int ui64;
   char *str;
   unsigned char *bytes;
};

// int DllMain() must exist and return 1 for a process to load the .DLL
// See https://docs.microsoft.com/en-us/windows/win32/dlls/dllmain for more information.
int __stdcall DllMain(void *module, unsigned int reason, void *reserved) { return 1; }

// #undef pin names lest they collide with names in any header file(s) you might include.
#undef Vin
#undef TL
#undef I
#undef W
#undef clk

extern "C" __declspec(dllexport) void dc_motor_basic_x2(void **opaque, double t, union uData *data)
{
   double  Vin = data[0].d; // input
   double  TL  = data[1].d; // input
   double  clk = data[2].d; // input
   double &I   = data[3].d; // output
   double &W   = data[4].d; // output

// Implement module evaluation code here:

Ts = 10e-6; Ra = 0.651 ; La = 243.75e-6 ; J = 246.1e-6; B = 37.89e-6; Kb = 0.027129;

alpha1 = (La-(0.5*Ts*Ra))/(La+(0.5*Ts*Ra));
beta1  = (0.5*Ts)/(La+(0.5*Ts*Ra));

alpha2 = (J-(0.5*Ts*B))/(J+(0.5*Ts*B));
beta2  = (0.5*Ts)/(J+(0.5*Ts*B));



if((clk>0.9999)&&(clk<=1.0001)){

   x1 = Vin - Kb*y2prev;
   x2 = Kb*y1prev - TL;

   y1 = alpha1*y1prev + beta1*(x1+x1prev);
   y2 = alpha2*y2prev + beta2*(x2+x2prev);

   x1prev = x1;
   x2prev = x2;

   y1prev = y1;
   y2prev = y2;

}

I = y1;
W = y2;

}
