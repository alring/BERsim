/*
 * =====================================================================================
 *
 *       Filename:  ver.cpp
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
#include <boost/lexical_cast.hpp>
#include "libBER.hpp"

using namespace std;

int main(int argc, char** argv){
	unsigned int counter=0;
	int M=0;
	double variance = 0.0;
	int n=0;
	vector<string>* v = new  vector<string>(argv, argv + argc);

        for(counter = 1; counter < v->size(); counter++){
                if(v->at(counter).compare("-M")==0 && counter < v->size())
                        M=boost::lexical_cast<int>(v->at(counter+1));
                if(v->at(counter).compare("-n")==0 && counter < v->size())
			n=boost::lexical_cast<int>(v->at(counter+1));
                if(v->at(counter).compare("-var")==0 && counter < v->size())
			variance=boost::lexical_cast<double>(v->at(counter+1));
	}
	qam::qam* q =  new qam(M, n);
	q->addNoise(variance);
	if(M!=16 && n<15){
		q->printSymbols(n);
		q->printNoiseSymbols(n);
	}
	q->toFile();
	q->plot();
	
	return 0;
 }
