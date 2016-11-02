#include <iostream>
#include <stdio.h>
#include <fstream>
#include <thread>
#include <cctype>

inline bool isInteger(const std::string & s)
{
	if(s.empty() || ((!isdigit(s[0])))) return false ;
	char * p ;
	strtol(s.c_str(), &p, 10) ;

	return (*p == 0) ;
}

bool data_collector (void) {
	FILE *in;
	char buff[512];
	if(!(in = popen("busybox microcom -t 9600 /dev/ttyACM0", "r"))){
		return false;
	}
	
	std::ofstream ofs (
		"/home/ubermensch/Academics/sem4/PHY315/DAS/software/code/test/b41d86f9-20e2-42ca-8fa7-ab05a1f3ea96",
		std::ofstream::out);
	int i = 0;
	while(fgets(buff, sizeof(buff), in)!=NULL){
			if (i < 100) {
				++i;
				continue;
			}
			
			ofs << buff;
	}
	ofs.close();
	pclose(in);
	return true;
}

int main (void)
{
	std::thread t1 (data_collector);
	std::cerr << "Detaching Thread" << std::endl;
	t1.join ();
	
	return 9;
}
