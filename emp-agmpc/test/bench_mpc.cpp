#include <emp-tool/emp-tool.h>
#include "emp-agmpc/emp-agmpc.h"
using namespace std;
using namespace emp;

const string circuit_file_location = macro_xstr(EMP_CIRCUIT_PATH);

const static int nP = 3;
int party, port;
void bench_once(NetIOMP<nP> * ios[2], ThreadPool * pool, string filename) {
	if(party == 1)cout <<"CIRCUIT:\t"<<filename<<endl;
	//string file = circuit_file_location+"/"+filename;
	CircuitFile cf(filename.c_str());

	auto big_start = clock_start();

	auto start = clock_start();
	CMPC<nP>* mpc = new CMPC<nP>(ios, pool, party, &cf);
	ios[0]->flush();
	ios[1]->flush();
	double t2 = time_from(start);
//	ios[0]->sync();
//	ios[1]->sync();
	if(party == 1)cout <<"Setup:\t"<<party<<"\t"<< t2 <<"\n"<<flush;

	start = clock_start();
	mpc->function_independent();
	ios[0]->flush();
	ios[1]->flush();
	t2 = time_from(start);
	if(party == 1)cout <<"FUNC_IND:\t"<<party<<"\t"<<t2<<" \n"<<flush;

	start = clock_start();
	mpc->function_dependent();
	ios[0]->flush();
	ios[1]->flush();
	t2 = time_from(start);
	if(party == 1)cout <<"FUNC_DEP:\t"<<party<<"\t"<<t2<<" \n"<<flush;

	bool *in = new bool[cf.n1+cf.n2]; bool *out = new bool[cf.n3];
	memset(in, false, cf.n1+cf.n2);
	start = clock_start();
	mpc->online(in, out);
	ios[0]->flush();
	ios[1]->flush();
	t2 = time_from(start);
//	uint64_t band2 = io.count();
//	if(party == 1)cout <<"bandwidth\t"<<party<<"\t"<<band2<<endl;
	if(party == 1)cout <<"ONLINE:\t"<<party<<"\t"<<t2<<" \n"<<flush;
	delete mpc;

	double t_big = time_from(big_start);

	if(party == 1) cout << "TOTAL:\t"<<party<<"\t"<<t_big<<" \n"<<flush;
}
int main(int argc, char** argv) {
//	int func = 0;
	parse_party_and_port(argv, &party, &port);
	if(party > nP)return 0;
	NetIOMP<nP> io(party, port);
#ifdef LOCALHOST
	NetIOMP<nP> io2(party, port+2*(nP+1)*(nP+1)+1);
#else
	NetIOMP<nP> io2(party, port+2*(nP+1));
#endif
	NetIOMP<nP> *ios[2] = {&io, &io2};
	ThreadPool pool(2*(nP-1)+2);	

//	for(int i = 0; i < 10; ++i)	
//	if(func == 0)	
//	bench_once(ios, &pool, circuit_file_location+"AES-non-expanded.txt");
//	for(int i = 0; i < 10; ++i)
//	if(func == 1)	
//	bench_once(ios, &pool, circuit_file_location+"sha-1.txt");
//	for(int i = 0; i < 10; ++i)
//	if(func == 2)	
//	bench_once(ios, &pool, circuit_file_location+"sha-256.txt");
//	
//	bench_once(ios, &pool, "/home/wangxiao/git/emp-toolkit/constantmpc/circ.txt");

	bench_once(ios, &pool, "/home/ubuntu/cmp-1000.txt");
//	bench_once(ios, &pool, circuit_file_location+"mult-100.txt");

	return 0;
}
