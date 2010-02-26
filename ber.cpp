/*
 * =====================================================================================
 *
 *       Filename:  ber.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/16/2009 08:57:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Devin Kelly (), dkelly@wpi.edu
 *        Company:  WPI
 *
 * =====================================================================================
 */


#include <iostream>
#include <vector>
#include <cstdlib>
#include <boost/lexical_cast.hpp>
#include "libBER.hpp"

using namespace std;

void printHelp();
bool checkNoise(double* N0, double* var);

int main(int argc, char** argv){
	unsigned int counter=0;
	int M=0;
	double variance = 0.0;
	double N0 = 0.0;
	int n=0;
	double rate = 0.0;
	vector<string>* v = new  vector<string>(argv, argv + argc);

        for(counter = 1; counter < v->size(); counter++){
                if(v->at(counter).compare("-h")==0 || v->at(counter).compare("--help")==0){
			printHelp();
			exit(0);
		}
                if(v->at(counter).compare("-M")==0 && counter < v->size())
                        M=boost::lexical_cast<int>(v->at(counter+1));
                if(v->at(counter).compare("-n")==0 && counter < v->size())
			n=boost::lexical_cast<int>(v->at(counter+1));
                if(v->at(counter).compare("-var")==0 && counter < v->size())
			variance=boost::lexical_cast<double>(v->at(counter+1));
                if(v->at(counter).compare("-N0")==0 && counter < v->size())
			N0=boost::lexical_cast<double>(v->at(counter+1));
	}
	delete v;
	v = NULL;

	if(!checkNoise(&N0, &variance))
		return -1;
	
	qam::qam* q =  new qam(M, n);
	q->addNoise(variance);

	/* prints symbols to stdout if there are less than 15 symbols */
	if(M!=16 && n<15){
		q->printSymbols(n);
		q->printNoiseSymbols(n);
		}
	
	rate = q->sim();

	cout << "The Bit Error Rate is " << rate << endl;
	q->plot();
	
	delete q;
	q = NULL;

	return 0;
 }

bool checkNoise(double* N0, double* var){
	/* checking these are complicated:
	 * both must be greater than 0
	 * if both are greater than 0, 2*N0==var
	 * otherwise error
	 */

	/* TODO make smart poiner? */
	if(N0==NULL || var==NULL){
		cerr << "Error Checking noise power\n";
		return false;
		}
	else if((*N0) < 0 || (*var) < 0){
		cerr << "Neither N0 nor var can be negative\n";
		return false;
		}
	else if((*N0>0 && *var>0) && (*var != 2.0 * (*N0))){
		cerr << "If both the variance and N0 are set, then the variance must be 2*N0\n";
		return false;
		}
	else if((*N0) > 0 && (*var) == 0){
		*var = (*N0)*2.0;
		return true;
		}
	else if((*N0) == 0 && (*var) > 0){
		return true;
		}

	}

void printHelp(){
	std::cout	<< "-help           	print this message\n" 
			<< "-var            	noise variance \n"
			<< "-n              	number of samples \n"
			<< "-N			Noise Power\n"
		  	<< "-M              	number of constellation points, only 2 or 4 suppoerted currently" 
		  	<< std::endl;
	return;
	}
