#pragma once
class Data
{
public:
	double rho;
	double rho_u;
	double e;

	double p();
	double a();
	double u();

	Data();
	Data(double _rho, double _rho_u, double _p);
	Data F() ;
	void count_e(double _p);
	void display();

	Data operator+ (const Data& B);
	Data operator- (const Data& B);

//	Data multiply(double& k);
//
//	Data operator* (double& k);
//	Data operator* (const Data& B, const double& k) ;

};

