#include "libBER.hpp"

qam::qam(int M, int n){
	static const int* M_max = endof(M_values);
	
	this->M=M;
	this->symbols = new std::vector<pt>(n);
	
	if(std::find(M_values, M_max, M)==M_max){
		std::cout << M << " not accectable" << std::endl;
		exit(0);
	}

	assignVec(symbols, this->M);
	this->symbols_noise = new std::vector<pt>(*this->symbols);
}

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

bool qam::is_error(int symbol_num){
	

	return false;
}

double qam::find_power(pt* in){
	return sqrt(pow(in->real(),2) + pow(in->imag(),2));
}

void qam::toFile(){
	
	std::ofstream Sym_stream("/tmp/symbols.txt");
	std::ofstream SymNoise_stream("/tmp/symbols_noise.txt");

	if(Sym_stream.is_open() && SymNoise_stream.is_open()){

		std::vector<pt>::iterator itr;

		Sym_stream << "Real\n";
		for(itr=this->symbols->begin(); itr<this->symbols->end(); itr++)
			Sym_stream << std::fixed << std::setprecision(7) << itr->real() << std::endl;

		SymNoise_stream << "Real\n";
		for(itr=this->symbols_noise->begin(); itr<this->symbols_noise->end(); itr++)
			SymNoise_stream << std::fixed << std::setprecision(7) << itr->real() << std::endl;

		Sym_stream << "\nImag\n";
		for(itr=this->symbols->begin(); itr<this->symbols->end(); itr++)
			Sym_stream << std::fixed << std::setprecision(7) << itr->imag() << std::endl;

		SymNoise_stream << "\nImag\n";
		for(itr=this->symbols_noise->begin(); itr<this->symbols_noise->end(); itr++)
			SymNoise_stream <<  std::fixed << std::setprecision(7) << itr->imag() << std::endl;


	}
	else{
		std::cerr << "error opening files in /tmp" << std::endl;
	}

	Sym_stream.close();
	SymNoise_stream.close();

	return;
}

void qam::plot(){

	Py_Initialize();

	using namespace boost::python;
	try{
		object main = import("__main__");
		object global(main.attr("__dict__"));
		object result = exec_file("scatterPlot.py", global, global);
	}
	catch(error_already_set){
		PyErr_Print();
	}

	Py_Finalize();
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
