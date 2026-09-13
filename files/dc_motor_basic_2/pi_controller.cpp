// Automatically generated C++ file on Tue Jul 21 19:41:50 2026
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD -o pi_controller.cpp kernel32.lib

#include <cmath>
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
#undef wref
#undef wm
#undef Ia
#undef clk
#undef pwm



/*---------------- Sampling ----------------*/
double Ts=100e-6;
double phase =0;

/*---------------- Speed Controller ----------------*/
double speed_ref;
double speed_measured;
double speed_error;
double speed_error_prev=0;

double KP_speed=0.15;
double KI_speed=0.01;

double speed_prop;
double speed_integ;
double cont_integ_temp;

double Iq_ref;

double iq_limit_pos=10;
double iq_limit_neg=-10;

int speed_integ_enable = 1;


/*---------------- Current Controller ----------------*/
double Iq_measured;
double Iq_error;
double Iq_error_prev=0;

double KP_i=0.05;
double KI_i=0.5;

double iq_prop;
double iq_integ;

double Vq;

double vq_limit_pos=0.9;
double vq_limit_neg=0.1;

int iq_integ_enable = 1;


/*---------------- Motor Feedback ----------------*/
double w;
double ia;

double tri;


/*----------------Speed controller ----------------*/
void Speed_Controller(void)
{
    /* Measured speed */
    speed_measured = w;

    /* Speed error */
    speed_error = speed_ref - speed_measured;

    /* Proportional term */
    speed_prop = KP_speed * speed_error;

    /* Trapezoidal integration */
    cont_integ_temp = (KI_speed * Ts * 0.5) *
                      (speed_error + speed_error_prev);

    if (speed_integ_enable)
    {
        speed_integ += cont_integ_temp;
    }

    /* PI output */
    Iq_ref = speed_prop + speed_integ;

    /* Anti-windup */
    if ((speed_error > 0.0 && Iq_ref > iq_limit_pos) ||
        (speed_error < 0.0 && Iq_ref < iq_limit_neg))
    {
        speed_integ_enable = 0;
    }
    else
    {
        speed_integ_enable = 1;
    }

    /* Saturation */
    if (Iq_ref > iq_limit_pos)
        Iq_ref = iq_limit_pos;
    else if (Iq_ref < iq_limit_neg)
        Iq_ref = iq_limit_neg;

    /* Store previous error */
    speed_error_prev = speed_error;
}


/*-------CURRENT CONTROLLER (Inner PI Loop)-------*/

void Current_Controller(void)
{
    /* Measured current */
    Iq_measured = ia;

    /* Current error */
    Iq_error = Iq_ref - Iq_measured;

    /* Proportional term */
    iq_prop = KP_i * Iq_error;

    /* Trapezoidal integration */
    cont_integ_temp = (KI_i * Ts * 0.5) *
                      (Iq_error + Iq_error_prev);

    if (iq_integ_enable)
    {
        iq_integ += cont_integ_temp;
    }

    /* PI output */
    Vq = iq_prop + iq_integ;

    /* Anti-windup */
    if ((Iq_error > 0.0 && Vq > vq_limit_pos) ||
        (Iq_error < 0.0 && Vq < vq_limit_neg))
    {
        iq_integ_enable = 0;
    }
    else
    {
        iq_integ_enable = 1;
    }

    /* Saturation */
    if (Vq > vq_limit_pos)
        Vq = vq_limit_pos;
    else if (Vq < vq_limit_neg)
        Vq = vq_limit_neg;



    /* Store previous error */
    Iq_error_prev = Iq_error;
}

extern "C" __declspec(dllexport) void pi_controller(void **opaque, double t, union uData *data)
{
   double  wref = data[0].d; // input
   double  wm   = data[1].d; // input
   double  Ia   = data[2].d; // input
   double  clk  = data[3].d; // input
   double &pwm  = data[4].d; // output

// Implement module evaluation code here:


//------------ Generate triangle-----------//

phase  = t/Ts - floor(t/Ts);

if(phase<0.5){
   tri = 2.0*phase;
}
else{
   tri = 2.0*(1.0-phase);
}

if((clk>0.9999)&&(clk<=1.0001)){

   speed_ref = wref;
   w=wm;
   ia=Ia;
   Speed_Controller();
   Current_Controller();


}

if(Vq>tri) {pwm = 1;}
else{pwm =0;}


}
