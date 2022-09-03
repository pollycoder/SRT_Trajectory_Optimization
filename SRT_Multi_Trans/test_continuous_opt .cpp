#include <iostream>
#include <vector>
#include "PSO_DE_NLOPT.hpp"
#include <math.h>

/*double obj_func(const std::vector<double>& X, std::vector<double>& grad, void* f_data)
{
	double* para = (double*)f_data; //类型强制转换
	
	double obj = pow(X[0], 3) + pow(X[1], 3) + pow(X[2], 3);
	
	return obj;
}



int main()
{
	std::vector<double> x = { 0.5,0.5,0.5 };
	double f=1000.0;
	PSO(obj_func,nullptr, x, f, 3,0, 1000, 1001); // x^3 +y ^3+ z^3
	std::cout << " PSO结果：" << f << std::endl;

	std::vector<double> x1 = { 0.5,0.5,0.5 };
	double f1 = 1000.0;
	DE(obj_func, nullptr, x1, f1, 3, 0, 1000, 1001); // x^3 +y ^3+ z^3
	std::cout << " DE结果：" << f << std::endl;

	//std::vector<double> x2 = { 0.5,0.5,0.5 };
	//double f2 = 1000.0;
	//nlopt_main(obj_func, nullptr, x2, f2, 3,0,1000); // x^3 +y ^3+ z^3
	//std::cout << " NLOPT结果：" << f << std::endl;
	
	//system("pause");

	
}
*/
