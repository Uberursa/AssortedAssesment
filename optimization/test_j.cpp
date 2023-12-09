#include <stdio.h>
#include <math.h>
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

// gcc -lm test_j.c -o test_j

struct Vars_j
{
    double a;
   double b;
   double g;
   double pi;

   double fptildemin;

   double gC;
   double aC;

   double aX;
   double gX;

   double saC;
   double saX;

   double sbC;
   double sbX;

   double fpt;

   double alpha;
   double gamma;
   double sigma_a;
   double sigma_b;
   double exp1arg;
   double sigma;

   double exp2arg;

   double S;
};


double function_j(double f, double fp, double fptilde, Vars_j *vars) {

   vars->exp1arg = -1.25 * pow((f/fp),-4);
   vars->sigma   = (f <= fp) * vars->sigma_a + (f > fp) * vars->sigma_b;

   vars->exp2arg = -0.5 * pow((f-fp)/(vars->sigma*fp), 2);

   vars->S = vars->alpha * pow(vars->g, 2) * pow((2*vars->pi), -4) * pow(f,-5) * exp(vars->exp1arg) * pow(vars->gamma, exp(vars->exp2arg));

   return vars->S;
}

void setConstants_j(Vars_j *vars) {
    vars->a  = 0.0081;
    vars->b  = 0.6;
    vars->g  = 9.807;
    vars->pi = 4.*atan(1.);

    vars->fptildemin = (1.0/2.0/vars->pi) * pow((4.0 * vars->b / 5.0), (1.0/4.0));

    vars->gC = 5.87;
    vars->aC = 0.0317;

    vars->aX  = (log(vars->a)-log(vars->aC))/log(vars->fptildemin);
    vars->gX  = -log(vars->gC)/log(vars->fptildemin);

    vars->saC = 0.0547;
    vars->saX = 0.32;

    vars->sbC = 0.0783;
    vars->sbX = 0.16;
}

int main() {

  double S, f, fp, fptilde;
  Vars_j vars;

//   for (f = -5.; f <= 5.; f += 0.01) {
//     for (fp = 0.; fp <= 10.; fp += 0.01) {
//       for (fptilde = 0.; fptilde <= 10.; fptilde += 0.01) {
//         S = function_j(f, fp, fptilde);
//       }
//     }
//   }

//Stage 1, constants
setConstants_j(&vars);

// stripped down version of the loop for faster testing
//   for (f = -5.; f <= 5.; f += 0.1) {
//     for (fp = 0.; fp <= 10.; fp += 0.1) {
//       for (fptilde = 0.; fptilde <= 10.; fptilde += 0.1) {
//         S = function_j(f, fp, fptilde);
//       }
//     }
//   }

    for (fptilde = 0.; fptilde <= 10.; fptilde += 0.1) {
        vars.fpt = MAX(fptilde, vars.fptildemin);

        vars.alpha   = vars.aC  * pow(vars.fpt, vars.aX);
        vars.gamma   = vars.gC  * pow(vars.fpt, vars.gX);
        vars.sigma_a = vars.saC * pow(vars.fpt, vars.saX);
        vars.sigma_b = vars.sbC * pow(vars.fpt, vars.sbX);

        for (f = -5.; f <= 5.; f += 0.1) {
            for (fp = 0.; fp <= 10.; fp += 0.1) {
                S = function_j(f, fp, fptilde, &vars);
            }
        }
    }
}
