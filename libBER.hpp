#include <python2.6/Python.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <complex>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/python.hpp>
#include <boost/python/exec.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/implicit.hpp>

#define c1 		0.316218152546624 
#define c2 		0.948654457639872
#define root2		0.7071068

typedef std::complex<double> pt;

pt bpsk_pts[] = {pt(1.0, 0.0), pt(-1.0,0.0)};
pt qpsk_pts[] = {pt(root2, root2), pt(-root2, root2), pt(root2, -root2), pt(-root2, -root2)};

/* this 16-qam constellation is normalized so the average symbol energy is 1*/
pt qam16_pts[] = { 	pt(c1,c1), pt(c1,c2), pt(c2,c1), pt(c2,c2), 
			pt(-c1,c1), pt(-c1,c2), pt(-c2,c1), pt(-c2,c2), 
			pt(c1,-c1), pt(c1,-c2), pt(c2,-c1), pt(c2,-c2), 
			pt(-c1,-c1), pt(-c1,-c2), pt(-c2,-c1), pt(-c2,-c2)};

static const int M_values[] = {2, 4, 16, 32};


template <typename T, size_t N>
T* endof(T (&pArray)[N])
{
    return &pArray[0] + N;
}

class qam{
	std::vector<pt>* symbols;
	std::vector<pt>* symbols_noise;
	int M;

	public:
		qam(int M, int n);
		void addNoise(double pwr);
		void printSymbols(unsigned int upTo);
		void printNoiseSymbols(unsigned int upTo);
		bool is_error(int symbol_num);
		double find_power(pt* in);
		void plot();
		void toFile();

};

BOOST_PYTHON_MODULE(vector_indexing_suite_ext){
	boost::python::class_<std::vector<double> >("PyVec")
        .def(boost::python::vector_indexing_suite<std::vector<double> >());
}

void assignVec(std::vector<pt>* vin, int M);

