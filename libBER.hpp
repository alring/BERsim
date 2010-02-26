#ifndef LIBBER_HPP
#define LIBBER_HPP

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
#ifndef NOPYTHON
#include <python2.6/Python.h>
#include <boost/python.hpp>
#include <boost/python/exec.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/implicit.hpp>
#endif

#define C1 		0.316218152546624 
#define C2 		0.948654457639872
#define HALFROOT2	0.7071068
#define MAX_PLOTABLE	5000	//the plotter gets pretty slow with more that about 5000 points

typedef std::complex<double> pt;

/* Default Constellations */

/* all the constellations are normalized so the average symbol energy is one */
pt bpsk_pts[] = {pt(1.0, 0.0), pt(-1.0,0.0)};
pt qpsk_pts[] = {pt(HALFROOT2, HALFROOT2), pt(-HALFROOT2, HALFROOT2), pt(HALFROOT2, -HALFROOT2), pt(-HALFROOT2, -HALFROOT2)};
pt qam16_pts[] = { 	pt(C1,C1), pt(C1,C2), pt(C2,C1), pt(C2,C2), 
			pt(-C1,C1), pt(-C1,C2), pt(-C2,C1), pt(-C2,C2), 
			pt(C1,-C1), pt(C1,-C2), pt(C2,-C1), pt(C2,-C2), 
			pt(-C1,-C1), pt(-C1,-C2), pt(-C2,-C1), pt(-C2,-C2)};

static const int M_values[] = {2, 4, 16, 32};

/* Templates  */

template <typename T, size_t N>
T* endof(T (&pArray)[N])
{
    return &pArray[0] + N;
}

template <typename T>
bool signMatch(T a, T b){
	return (a<0 && b<0) || (a>=0 && b>=0);
}

/* Class Definitions  */

class qam{

	public:
		qam(int M, int N);
		~qam();
		void addNoise(double pwr);
		void printSymbols(unsigned int upTo);
		void printNoiseSymbols(unsigned int upTo);
		#ifndef NOPYTHON
		void plot();
		void plotWithFile();
		#endif
		double sim();
		double find_power(pt* in);
		double dist(pt* a, pt* b);

		

	private:
		/* methods */
		void toFile();
		bool is_error(int num);
		std::complex<double> quantize(int num);

		/* variables */
		std::vector<pt>* symbols;
		std::vector<pt>* symbols_noise;
		int M;
		int N;};

/* Function Definitions   */

void assignVec(std::vector<pt>* vin, int M);


#endif
