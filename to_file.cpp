#include "to_file.h"
#include <fstream>

void to_file(std::ofstream& file, std::vector<Data> w, Sit S, double t){

    for (int i=0; i<S.n+1; i++){
        file
        << t << ";"
        << S.xi[i] << ";"
        << w[i].rho << ";"
        << w[i].u() << ";"
        << w[i].e << ";"
        << w[i].p() << std::endl;
    }

}
