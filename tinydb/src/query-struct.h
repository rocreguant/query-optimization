#ifndef QUERYSTRUCT H
#define QUERYSTRUCT H

#include <string>
#include <vector>

using namespace std;

struct condition {
	string comparator; 	
	pair<string, string> l_attr; 
	pair<string, string> r_attr; 
} ;

struct query {
	vector<string> select; 	//the selection names
	vector<pair<string, string> > from; //only the table names <relation, binding>
	vector<condition> where; 
} ;


#endif
