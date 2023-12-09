#include <stdio.h>
#include <math.h>
#include <pthread.h>
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

// gcc -lm test_j.c -o test_j

struct Const_j {
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
};

struct Vars_j {
   double fpt;
   double alpha;
   double gamma;
   double sigma_a;
   double sigma_b;
};

double function_j(double f, double fp, double fptilde, Const_j *const_j, Vars_j *vars) {

   double exp1arg = -1.25 * pow((f/fp),-4);
   double sigma   = (f <= fp) * vars->sigma_a + (f > fp) * vars->sigma_b;

   double exp2arg = -0.5 * pow((f-fp)/(sigma*fp), 2);

   double S = vars->alpha * pow(const_j->g, 2) * pow((2*const_j->pi), -4) * pow(f,-5) * exp(exp1arg) * pow(vars->gamma, exp(exp2arg));

   return S;
}

struct Const_j setConstants_j() {
    struct Const_j const_j;
    const_j.a  = (double) 0.0081;
    const_j.b  = 0.6;
    const_j.g  = 9.807;
    const_j.pi = 4.*atan(1.);

    const_j.fptildemin = (1.0/2.0/const_j.pi) * pow((4.0 * const_j.b / 5.0), (1.0/4.0));

    const_j.gC = 5.87;
    const_j.aC = 0.0317;

    const_j.aX  = (log(const_j.a)-log(const_j.aC))/log(const_j.fptildemin);
    const_j.gX  = -log(const_j.gC)/log(const_j.fptildemin);

    const_j.saC = 0.0547;
    const_j.saX = 0.32;

    const_j.sbC = 0.0783;
    const_j.sbX = 0.16;

    return const_j;
}


void* thread1(void *const_j_struct) {
    Const_j *const_j = (Const_j*) const_j_struct;
    double S, f, fp, fptilde;
    Vars_j vars;
    for (fptilde = 0.; fptilde <= 5.; fptilde += 0.1) {
        vars.fpt = MAX(fptilde, const_j->fptildemin);

        vars.alpha   = const_j->aC  * pow(vars.fpt, const_j->aX);
        vars.gamma   = const_j->gC  * pow(vars.fpt, const_j->gX);
        vars.sigma_a = const_j->saC * pow(vars.fpt, const_j->saX);
        vars.sigma_b = const_j->sbC * pow(vars.fpt, const_j->sbX);

        for (f = -5.; f <= 5.; f += 0.1) {
            for (fp = 0.; fp <= 10.; fp += 0.1) {
                S = function_j(f, fp, fptilde, const_j, &vars);
            }
        }
    }
    pthread_exit(0);
}

void* thread2(void *const_j_struct) {
    Const_j *const_j = (Const_j*) const_j_struct;
    double S, f, fp, fptilde;
    Vars_j vars;
    for (fptilde = 5.; fptilde <= 10.; fptilde += 0.1) {
        vars.fpt = MAX(fptilde, const_j->fptildemin);

        vars.alpha   = const_j->aC  * pow(vars.fpt, const_j->aX);
        vars.gamma   = const_j->gC  * pow(vars.fpt, const_j->gX);
        vars.sigma_a = const_j->saC * pow(vars.fpt, const_j->saX);
        vars.sigma_b = const_j->sbC * pow(vars.fpt, const_j->sbX);

        for (f = -5.; f <= 5.; f += 0.1) {
            for (fp = 0.; fp <= 10.; fp += 0.1) {
                S = function_j(f, fp, fptilde, const_j, &vars);
            }
        }
    }
    pthread_exit(0);
}

int main() {

  double S, f, fp, fptilde;
  //Const_j const;

//   for (f = -5.; f <= 5.; f += 0.01) {
//     for (fp = 0.; fp <= 10.; fp += 0.01) {
//       for (fptilde = 0.; fptilde <= 10.; fptilde += 0.01) {
//         S = function_j(f, fp, fptilde);
//       }
//     }
//   }

//Constants
Const_j const_j = setConstants_j();

// stripped down version of the loop for faster testing
//   for (f = -5.; f <= 5.; f += 0.1) {
//     for (fp = 0.; fp <= 10.; fp += 0.1) {
//       for (fptilde = 0.; fptilde <= 10.; fptilde += 0.1) {
//         S = function_j(f, fp, fptilde);
//       }
//     }
//   }

    // for (fptilde = 0.; fptilde <= 10.; fptilde += 0.1) {
    //     vars.fpt = MAX(fptilde, vars.fptildemin);

    //     vars.alpha   = vars.aC  * pow(vars.fpt, vars.aX);
    //     vars.gamma   = vars.gC  * pow(vars.fpt, vars.gX);
    //     vars.sigma_a = vars.saC * pow(vars.fpt, vars.saX);
    //     vars.sigma_b = vars.sbC * pow(vars.fpt, vars.sbX);

    //     for (f = -5.; f <= 5.; f += 0.1) {
    //         for (fp = 0.; fp <= 10.; fp += 0.1) {
    //             S = function_j(f, fp, fptilde, &vars);
    //         }
    //     }
    // }

    // Multithreaded stuff
    pthread_t id[2];
    pthread_create(&id[0], NULL, thread1, &const_j);
    pthread_create(&id[1], NULL, thread2, &const_j);

    int *ret1;
    pthread_join(id[0], (void**)&ret1);
    int *ret2;
    pthread_join(id[0], (void**)&ret2);
}
