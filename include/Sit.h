#ifndef SIT_H
#define SIT_H
#include <vector>

class Sit
{
    public:
        Sit(double _start, double _stop, int _n, double _x_b);
        double start;
        double stop;
        double x_barrier;
        int n;

        double dx;

        std::vector<double> xi;
};

#endif // SIT_H
