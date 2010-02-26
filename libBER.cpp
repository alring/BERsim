#include "libBER.hpp"

qam::qam(int M, int N){
	static const int* M_max = endof(M_values);
	
	this->M=M;
	if(std::find(M_values, M_max, M)==M_max){
		std::cout << M << " not accectable" << std::endl;
		exit(0);
		}

	this->N=N;
	if(N <= 0){
		std::cout << "N must be 1 or greater" << std::endl;
		exit(0);
		}


	this->symbols = new std::vector<pt>(this->N);
	assignVec(symbols, this->M);
	this->symbols_noise = new std::vector<pt>(*this->symbols);
	}

qam::~qam(){
	delete this->symbols;	
	this->symbols = NULL;
	delete this->symbols_noise;
	this->symbols_noise = NULL;
	
	return;
	}

qam::~qam(){

}	

/******************************************
** Methods for doing the actual simulation
*******************************************/

void qam::addNoise(double pwr){
        boost::minstd_rand generator(42u);
	generator.seed(static_cast<unsigned int>(std::time(0)));
        boost::normal_distribution<> N(0,pwr); // AWGN!!
        boost::variate_generator<boost::minstd_rand&, boost::normal_distribution<> > norm(generator, N);

	std::vector<pt>::iterator itr;
	for(itr = this->symbols_noise->begin(); itr < this->symbols_noise->end(); itr++){
		itr->imag() += norm();
		itr->real() += norm();
		}
	return;
	}

double qam::sim(){
	int counter=0;
	int errors=0;
	
	for(counter=0; counter<this->N; counter++){
		if(!this->is_error(counter))
			errors++;
		}	

	return (double) errors/(this->N);
	}

bool qam::is_error(int num){
	bool ret;
	pt a = this->symbols->at(num);
	pt b = this->symbols_noise->at(num);
	
	switch(this->M) {
		case 2:
			ret = signMatch(a.real(), b.real());
			break;
		case 4:
			ret =    signMatch(a.real(), b.real()) 
			      && signMatch(a.imag(), b.imag());
			break;
		case 16:
			/* For bpsk/qpsk it's easiest to just compare signs... can't do that exactly with 16-QAM */
			if(quantize(num) == this->symbols->at(num))
				ret=true;
			else
				ret=false;
			break;
	}
	return ret;
}

std::complex<double> qam::quantize(int num){
	std::complex<double> ret = qam16_pts[0];

	pt s = this->symbols_noise->at(num);

	#ifdef DEBUG
	std::cout << "init: " << ret.real() << "  " << ret.imag() << "\n";
	std::cout << "s: " << s.real() << "  " << s.imag() << "\n";
	#endif

	int counter = 0;
	double d1 = 0.0;
	double d2 = 0.0;

	for(counter=0; counter < (this->M); counter++){
		d1 = (this->dist(&s, &qam16_pts[counter]));
		d2 =  (this->dist(&ret, &s));
	
		#ifdef DEBUG
		std::cout << "d1 is " << d1 << "  d2 is " << d2 << std::endl;
		#endif
		if(d1 < d2)
			ret=qam16_pts[counter];
		}
	#ifdef DEBUG
	std::cout << "symbol is "	<< this->symbols->at(num).real() << ", " 
					<< this->symbols->at(num).imag() << "  returned symbol is " 
					<< ret.real() << ", " << ret.imag() << std::endl;
	#endif	

	return ret;
	}

double qam::find_power(pt* in){
	return sqrt(pow(in->real(),2) + pow(in->imag(),2));
	}

double qam::dist(pt* a, pt* b){
	return sqrt(pow(a->real() - b->real(), 2) + pow(a->imag() - b->imag(), 2));
	}

/**************************
** Pring symbols to std out
*****************************/


/* Prints I&Q of noisy symbols to stdout, up to a specified number */
void qam::printSymbols(unsigned int upTo){
	unsigned int counter = 0;

	if(upTo > this->symbols->size())
		upTo=this->symbols->size();
	else if(upTo == 0)
		upTo=this->symbols->size();

	for(counter=0; counter<upTo; counter++)
		std::cout << this->symbols->at(counter) << std::endl;
	return;
	}

/* Prints I&Q of noisy symbols to stdout, up to a specified number */
void qam::printNoiseSymbols(unsigned int upTo){
	unsigned int counter = 0;

	if(upTo > this->symbols_noise->size())
		upTo=this->symbols_noise->size();
	else if(upTo == 0)
		upTo=this->symbols_noise->size();

	for(counter=0; counter<upTo; counter++)
		std::cout << this->symbols_noise->at(counter) << std::endl;
	return;
	}

/************************************
** Methods for ploting constellations
************************************/

#ifndef NOPYTHON
/* Plot using matplotlib & python */
void qam::plot(){
	unsigned int i=0;
	(MAX_PLOTABLE < this->symbols_noise->size()) ? i=MAX_PLOTABLE : i=this->symbols_noise->size();
	std::vector<double> I(i); // inphase
	std::vector<double> Q(i); // quadrature

	for(i=0; i<MAX_PLOTABLE&&i<this->symbols_noise->size(); i++){
		I[i]=this->symbols_noise->at(i).real();
		Q[i]=this->symbols_noise->at(i).imag();
		}

	Py_Initialize();

	using namespace boost::python;

	boost::python::class_<std::vector<double> >("PyVec")
		.def(boost::python::vector_indexing_suite<std::vector<double> >());

	try{
		object main = import("__main__");
		object global(main.attr("__dict__"));
		global["x"] = I;  // inphase is x-axis
		global["y"] = Q; // quadrature is y-axis
		object result = exec_file("scatterPlot.py", global, global);
		}
	catch(error_already_set){
		PyErr_Print();
		}

	Py_Finalize();
	return;
	}

/* plot using matplotlib and python, but write files to /tmp */
void qam::plotWithFile(){
	this->toFile();

	Py_Initialize();

	using namespace boost::python;
	try{
		object main = import("__main__");
		object global(main.attr("__dict__"));
		object result = exec_file("scatterPlot_old.py", global, global);
		}
	catch(error_already_set){
		PyErr_Print();
		}

	Py_Finalize();
	return;
	}
#endif

/* write data to files in /tmp */
void qam::toFile(){
	std::ofstream Sym_stream("/tmp/symbols.txt");
	std::ofstream SymNoise_stream("/tmp/symbols_noise.txt");
	std::vector<pt>::iterator LIM1, LIM2;

	if(this->N < MAX_PLOTABLE){
		LIM1 = this->symbols->end();
		LIM2 = this->symbols_noise->end();
	}
	else{
		LIM1 = this->symbols->begin() + MAX_PLOTABLE;
		LIM2 = this->symbols_noise->begin() + MAX_PLOTABLE;

	}

	if(Sym_stream.is_open() && SymNoise_stream.is_open()){
		std::vector<pt>::iterator itr;

		Sym_stream << "Real\n";
		for(itr=this->symbols->begin(); itr<LIM1; itr++)
			Sym_stream << std::fixed << std::setprecision(7) << itr->real() << std::endl;

		SymNoise_stream << "Real\n";
		for(itr=this->symbols_noise->begin(); itr<LIM2; itr++)
			SymNoise_stream << std::fixed << std::setprecision(7) << itr->real() << std::endl;

		Sym_stream << "\nImag\n";
		for(itr=this->symbols->begin(); itr<LIM1; itr++)
			Sym_stream << std::fixed << std::setprecision(7) << itr->imag() << std::endl;

		SymNoise_stream << "\nImag\n";
		for(itr=this->symbols_noise->begin(); itr<LIM2; itr++)
			SymNoise_stream <<  std::fixed << std::setprecision(7) << itr->imag() << std::endl;
	}
	else{
		std::cerr << "error opening files in /tmp" << std::endl;
	}

	Sym_stream.close();
	SymNoise_stream.close();
	return;
	}


/* this is ugly, I am aware */
void assignVec(std::vector<pt>* vin, int M){
        boost::minstd_rand generator(42u);
        boost::uniform_real<> M_gen(0,M);
        boost::variate_generator<boost::minstd_rand&, boost::uniform_real<> > uni(generator, M_gen);
	std::vector<pt>::iterator itr;

	if(M==2){
		std::vector<pt> pts(bpsk_pts, bpsk_pts + sizeof(bpsk_pts)/sizeof(pt));
		for(itr=vin->begin(); itr<vin->end(); itr++){
			*itr = pts[uni()];
			}
		}
	else if(M==4){
		std::vector<pt> pts(qpsk_pts, qpsk_pts + sizeof(qpsk_pts)/sizeof(pt));
		for(itr=vin->begin(); itr<vin->end(); itr++)
			*itr = pts[uni()];
		}
	else if(M==16){
		std::vector<pt> pts(qam16_pts, qam16_pts + sizeof(qam16_pts)/sizeof(pt));
		for(itr=vin->begin(); itr<vin->end(); itr++)
			*itr = pts[uni()];
		}
	else
		std::cout << "error" << std::endl;

	return;
}
