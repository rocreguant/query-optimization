#ifndef GOO H
#define GOO H

#include "JoinTree.h"
#include "QueryGraph.h"

class goo {
	
	private:
		JoinTree* joinTree; 
	
	public:
		goo(){}
		~goo(){}
		void optimizeVoid(QueryGraph qg); 
		JoinTree* optimizeJoinTree(QueryGraph qg); 

}; 


#endif 
