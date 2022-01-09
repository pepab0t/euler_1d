#include "Data.h"
#include <cmath>
#include <iostream>

#define kappa 1.4

Data::Data(){
}

Data::Data(double _rho, double _rho_u, double _p)
    : rho(_rho), rho_u(_rho_u), e(_p){}// { count_e(_p); }


double Data::p() {
	return (kappa - 1) * (e - 0.5 * rho_u * u());
}

double Data::u() {
	return rho_u / rho;
}

double Data::a() {
	return std::sqrt(kappa * p() / rho);
}

Data Data::F() {
	Data F;

	F.rho = rho_u;
	F.rho_u = rho_u * u() + p();
	F.e = u() * (e + p());

	return F;
}

void Data::count_e(double _p) {
	e = _p / (kappa - 1) + 0.5 * rho_u * u();
}

void Data::display(){
    std::cout << "rho = " << rho << std::endl;
    std::cout << "rho_u = " << rho_u << std::endl;
    std::cout << "e = " << e << std::endl;
}

Data Data::operator+(const Data& B) {
	Data A;
	A.rho = rho + B.rho;
	A.rho_u = rho_u + B.rho_u;
	A.e = e + B.e;

	return A;
}

Data Data::operator-(const Data& B) {
	Data A;
	A.rho = rho - B.rho;
	A.rho_u = rho_u - B.rho_u;
	A.e = e - B.e;

	return A;
}

//Data Data::multiply(double& k) {
//    Data A;
//
//    A.rho = k * rho;
//    A.rho_u = k * rho_u;
//    A.e = k * e;
//
//    return A;
//}
//
//Data Data::operator* (double& k) {
//    return multiply(k);
//}

//Data Data::operator* (const Data& B, const double& k) const{
//    data A;
//
//    A.rho = k * B.rho;
//    A.rho_u = k * B.rho_u;
//    A.e = k * B.e;
//
//    return A;
//}
