#include <iostream>
#include <string>

using namespace std;

class Fail{
public:

	Fail(string s){
		cout << "ERROR!" << endl << s << endl;
		exit(-1);
	}
};
