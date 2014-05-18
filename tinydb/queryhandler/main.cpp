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
	    
		return 0;
	}
