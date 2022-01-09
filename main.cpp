#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "data.h"
#include "Sit.h"
#include "to_file.h"

#define Log(x) std::cout << x << std::endl;

Data operator*(const double& k, const Data& B){
    Data A;

    A.rho = B.rho * k;
    A.rho_u = B.rho_u * k;
    A.e = B.e * k;

    return A;
}

Data operator*(const Data& B, const double& k){
    Data A;

    A.rho = B.rho * k;
    A.rho_u = B.rho_u * k;
    A.e = B.e * k;

    return A;
}

Data rusanov_flux(Data& ul, Data& ur, char sch, double dx, double dt){
    double q;
    if (sch == 'r'){
        q = std::max( std::abs(ul.u()) + ul.a() , std::abs(ur.u()) + ur.a() );
    } else if (sch == 'l'){
        q = 1 * dx/dt;
    } else if (sch == 't'){
        q = 0.8 * dx/dt;
    }
//    Log(q);
    return 0.5 * (ul.F() + ur.F()) - q/2 * (ur - ul);
}

Data hll_flux(Data& ul, Data& ur){
    double sr, sl;

    sl = std::min( ul.u() - ul.a(), ur.u() - ur.a() );
    sl = std::min(sl,0.0);

    sr = std::max( ul.u() + ul.a(), ur.u() + ur.a() );
    sr = std::max(sr,0.0);

    return ( -sl*ur.F() + sr*ul.F() )* (1/(sr-sl)) + sr*sl/(sr-sl) * (ur-ul);
}

Data ausm_flux(Data& ul, Data& ur){
    double Ml = ul.u() / ul.a();
    double Mr = ur.u() / ur.a();

    double M_plus, P_plus, M_minus, P_minus;

    if (std::fabs(Ml) <= 1){
        M_plus = 1/4 * pow((Ml+1),2);
        P_plus = 1/4 * pow((Ml+1),2)*(2-Ml);
    } else {
        M_plus = 0.5 * ( Ml + std::fabs(Ml));
        if (Ml < -1) {
            P_plus = 0.0;
        } else {
            P_plus = 1.0;
        }
    }

    if (std::fabs(Mr) <= 1){
        M_minus = -1/4 * pow((Mr+1),2);
        P_minus = 1/4 * pow((Mr-1),2)*(2+Mr);
    } else {
        M_minus = 0.5 * ( -Mr + std::fabs(Mr));
        if (Mr < -1) {
            P_minus = 1.0;
        } else {
            P_minus = 0.0;
        }
    }

//    double M = M_plus + M_minus;
//
//    Data Fc;
//    if (M>0){
//        Fc = M*ul.a()*ul;
//    } else {
//        Fc = M*ur.a()*ur;
//    }
//
//    return Fc + P_plus*Data(0, ul.p(), ul.p()*ul.u()) + P_minus*Data(0, ur.p(), ur.p()*ur.u());

    double c = 0.5*(ul.a()+ur.a());
    Data Fp1(0, ul.p(), ul.p()*ul.u());
    Data Fp2(0, ur.p(), ur.p()*ur.u());

    return c*(M_plus*ul + M_minus*ur) + P_plus*Fp1 + P_minus*Fp2;
}

int main() {
	Sit S(0, 1, 1000, 0.3);
    char sch = 'h';

	std::vector<Data> w(S.n+1);
	std::vector<Data> wn(S.n+1);

    //pocatecni podminka
    for (int i=0; i<S.n+1; i++){
        Data Y;

        if (S.xi[i] <= S.x_barrier){
            Y.rho = 1;
            Y.rho_u = 0;
            Y.count_e(1);
        } else {
            Y.rho = 0.1;
            Y.rho_u = 0;
            Y.count_e(0.1795);
        }
        w[i] = Y;
    }

    double t_part = 0.1;
    double t_max = 0.2;
    double CFL = 0.5;
    double t = 0;
    double den;
    double dt;
    int it = 0;

    //checkpoint
    bool checkpoint = false;

    std::string filename1;
    if (sch=='r'){
        filename1 = "rus1.txt";
    } else if(sch=='l'){
        filename1 = "lf1.txt";
    } else if(sch=='t'){
        filename1 = "rus1_konst.txt";
    } else if(sch=='h'){
        filename1 = "hll1.txt";
    } else if(sch=='a'){
        filename1 = "ausm1.txt";
    }


    std::ofstream file1(filename1);
    file1 << "t;x;rho;u;e;p" << std::endl;

    while (t < t_max){
        den = std::abs(w[0].u()) + w[0].a();

        for (int i=0; i<S.n+1; i++){
            if ((std::abs(w[i].u()) + w[i].a()) > den){
                den = abs(w[i].u()) + w[i].a();
            }
        }

        dt = CFL * S.dx / den;

        if (!checkpoint && t+dt>t_part){
            dt = t_part-t;
            checkpoint = true;
        }

        t += dt;

//        eps = S.dx/dt;
        if (sch == 'h'){
            for (int i=1; i<S.n; i++){
                wn[i] = w[i] - dt/S.dx * (hll_flux(w[i], w[i+1]) - hll_flux(w[i-1], w[i]));
            }
        } else if (sch == 'a'){
            for (int i=1; i<S.n; i++){
                wn[i] = w[i] - dt/S.dx * (ausm_flux(w[i], w[i+1]) - ausm_flux(w[i-1], w[i]));
//                std::cout << ausm_flux(w[i], w[i+1]).rho << ", " << ausm_flux(w[i-1], w[i]).rho <<std::endl;
            }
        } else {
            for (int i=1; i<S.n; i++){
                wn[i] = w[i] - dt/S.dx * (rusanov_flux(w[i], w[i+1], sch, S.dx, dt) - rusanov_flux(w[i-1], w[i], sch, S.dx, dt));
            }
        }

        wn[0] = wn[1];
        wn[S.n] = wn[S.n-1];

        w = wn;

        if (t == t_part){
            to_file(file1, w, S, t);
            file1.close();
        }

        if (it%100 == 0){
            std::cout << "Time: " << t << ", dt = " << dt << std::endl;
//            to_file(file, w, S, t);
        }

        it++;
    }

//    file.close();

    std::string filename2;
    if (sch == 'r'){
        filename2 = "rus2.txt";
    } else if (sch =='l') {
        filename2 = "lf2.txt";
    } else if (sch=='t'){
        filename2 = "rus2_konst.txt";
    } else if (sch=='h'){
        filename2 = "hll2.txt";
    } else if (sch=='a'){
        filename2 = "ausm2.txt";
    }

    std::ofstream file2(filename2);
    file2 << "t;x;rho;u;e;p" << std::endl;
    to_file(file2, w, S, t_max);
    file1.close();

    }
