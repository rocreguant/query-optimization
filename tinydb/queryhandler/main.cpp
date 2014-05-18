#include <iostream>
#include <vector>
#include "query-struct.h"
#include <stdio.h>
#include <sstream>
#include <string>
#include "executor.hpp"
#include "Pharser.hpp"
#include "QueryGraph.h"
//#include <algorithm>

using namespace std;


	int main(){
	
		Pharser p;
		query q;
    	try{
			q = p.read();
	    }
	    catch(char const* er) { std::cerr <<std::endl << "WARNING!! => " << er << std::endl << std::endl;}
	    
	    
	    QueryGraph* qg = new QueryGraph(q, "data/uni"); 
	    qg->toString(); 
	    
	    //Executor* executor = new Executor(); 
	    //executor->execute(q);
	    
/*
		Parser result checker



	    
	    cout << "query-select" << endl;
	    for(int i=0; i<q.select.size(); ++i) cout << i << " - " << q.select[i] << endl;
	    
	    cout << "query-from" << endl;
	    for(int i=0; i<q.from.size(); ++i) cout << i << " - " << q.from[i].first << ':' << q.from[i].second << endl;
	    
	    cout << "query-where" << endl;
	    for(int i=0; i<q.where.size(); ++i) cout << i << " - *" << q.where[i].l_attr.first << "+"<< q.where[i].l_attr.second << '$' << q.where[i].comparator << '$' <<  q.where[i].r_attr.first << "+"<< q.where[i].r_attr.second << endl;
    */
		return 0;
	}
