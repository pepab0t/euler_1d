#include "Sit.h"

Sit::Sit(double _start, double _stop, int _n, double _x_b)
{
    start = _start;
    stop = _stop;
    n = _n;
    x_barrier = _x_b;
    dx = (stop - start)/n;

    xi.resize(n+1);

    for (int i=0; i<n+1; i++){
        xi[i] = start + i*dx;
    }
}
