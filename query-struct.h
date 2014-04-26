#include<string>
#include <vector>

using namespace std;

struct query {
	vector<string> select; 	//the selection names
	vector<string> from; //only the table names
	vector<pair<string, string>> where; //left argument equals (always as said in the example) right argument
} ;
