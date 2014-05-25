#include <iostream>
#include "QueryGraph.h"
#include "Table.hpp"

using namespace std; 

QueryGraph::QueryGraph(query q, string databasePath){

	db.open(databasePath); 
	
	//Add nodes 
	for(auto fromIterator = q.from.begin(); fromIterator != q.from.end(); ++fromIterator){
		Node *tmpNode = new Node(); 
		tmpNode->setRelation((*fromIterator).first); 
		tmpNode->setBinding((*fromIterator).second); 

		nodes.push_back(tmpNode); 
	}
	
	//Add edges for each join predicate, and select predicate for nodes
	for(auto whereIterator = q.where.begin(); whereIterator != \
		q.where.end(); ++whereIterator){
		if((*whereIterator).r_attr.first == ""){ // If constant predicate
			ConstantPredicate* cp = new ConstantPredicate(); 
			cp->comparator = (*whereIterator).comparator;
			cp->attribute = (*whereIterator).l_attr.second; 
			cp->constantValue = (*whereIterator).r_attr.second; 			
			Node& node = QueryGraph::getNodeWithBinding( \
									(*whereIterator).l_attr.first); 
			node.addConstantPredicate(cp); 		
			
		}
		else{
			Edge *tmpEdge = new Edge(); 

			Node& nodeLeft = QueryGraph::getNodeWithBinding( \
									(*whereIterator).l_attr.first); 
			Node& nodeRight = QueryGraph::getNodeWithBinding( \
									(*whereIterator).r_attr.first);
									
			JoinPredicate *jp = new JoinPredicate(nodeLeft, nodeRight); 
			jp->comparator = (*whereIterator).comparator; 
			 
			jp->leftAttribute = (*whereIterator).l_attr.second;
			jp->rightAttribute = (*whereIterator).r_attr.second;
			
			tmpEdge->setJoinPredicate(jp); 	
			edges.push_back(tmpEdge); 
		}
		
		for(auto it = edges.begin(); it != edges.end(); ++it){
		}
	}
	setCardinalities(); 
	setSelectivities();
}


void QueryGraph::setCardinalities(){
	for(auto nodeIterator = nodes.begin(); nodeIterator != nodes.end(); ++nodeIterator){
		Table& table = db.getTable((*nodeIterator)->getRelation()); 
		
		if((*nodeIterator)->getPushedDownPredicates().size() == 0){ //Set to table size if no push downs
			(*nodeIterator)->setEstimatedCardinality(table.getCardinality()); 
		}
		else if((*nodeIterator)->getPushedDownPredicates().size() > 0){ //Loop over all pushdowns and reduce cardinality depending on domain
			//Loop over all pushed down predicates
			unsigned cardinality = table.getCardinality(); 
			for(auto predicateIterator = (*nodeIterator)->getPushedDownPredicates().begin(); predicateIterator != (*nodeIterator)->getPushedDownPredicates().end(); ++predicateIterator){
				int attrIndex = table.findAttribute((*predicateIterator)->attribute);
				const Attribute& attr = table.getAttribute(attrIndex);  
				unsigned domain = attr.getUniqueValues(); 		//Why this returns 100 always??? O.o 
				
				cardinality = domain/cardinality;  
			}
			(*nodeIterator)->setEstimatedCardinality(cardinality); 
		}
	}
}


void QueryGraph::setSelectivities(){
	for(auto edgeIterator = edges.begin(); edgeIterator != edges.end(); ++edgeIterator){
		Node& leftNode = (*edgeIterator)->getJoinPredicate().leftNode; 
		Node& rightNode = (*edgeIterator)->getJoinPredicate().rightNode; 
		
		//get attributes names
		string leftAttribute = (*edgeIterator)->getJoinPredicate().leftAttribute;
		string rightAttribute = (*edgeIterator)->getJoinPredicate().rightAttribute; 
		
		//get domains of both attributes
		Table& leftTable = db.getTable(leftNode.getRelation()); 
		int leftAttrIndex = leftTable.findAttribute(leftAttribute);
		const Attribute& leftAttr = leftTable.getAttribute(leftAttrIndex);  
		unsigned leftDomain = leftAttr.getUniqueValues();
		
		Table& rightTable = db.getTable(rightNode.getRelation()); 
		int rightAttrIndex = rightTable.findAttribute(rightAttribute);
		const Attribute& rightAttr = rightTable.getAttribute(rightAttrIndex);  
		unsigned rightDomain = rightAttr.getUniqueValues(); 
		
		if(leftDomain > rightDomain){
			(*edgeIterator)->setEstimatedSelectivity((double(1)/double(leftDomain))); 
		}
		else{
			(*edgeIterator)->setEstimatedSelectivity((double(1)/double(rightDomain))); 
		}
	}
	
}


Node& QueryGraph::getNode(string relationName){
	for(auto nodeIterator = nodes.begin(); nodeIterator != nodes.end(); ++nodeIterator){
		if((*nodeIterator)->getRelation() == relationName){
			return *(*nodeIterator); 
		} 
	}
	
	throw;
	
}

Node& QueryGraph::getNodeWithBinding(string binding){
	for(auto nodeIterator = nodes.begin(); nodeIterator != nodes.end(); ++nodeIterator){
		if((*nodeIterator)->getBinding() == binding){
			return *(*nodeIterator); 
		}
	}
	throw;
	
}

//Destructor
QueryGraph::~QueryGraph(){
	for(auto nodeIterator = nodes.begin(); nodeIterator != nodes.end(); ++nodeIterator){
		delete (*nodeIterator); 
	}

	for(auto edgeIterator = edges.begin(); edgeIterator != edges.end(); ++edgeIterator){
		delete (*edgeIterator); 
	}
}



string QueryGraph::toString(){
	string s; 
	
	//Nodes
	s += "Nodes: "; 
	s.append("\n"); 
	for(auto nodeIterator = nodes.begin(); nodeIterator != nodes.end(); ++nodeIterator){
		s.append("Relation: "); 
		s.append((*nodeIterator)->getRelation()); 
		s.append(", Binding: ");
		s.append((*nodeIterator)->getBinding());
		s.append(", Estimated Cardinality: ");
		s.append(to_string((*nodeIterator)->getEstimatedCardinality()));  
		s.append("\n"); 
	}
	
	s.append("\n"); 
	s.append("Edges: "); 
	s.append("\n"); 
	
	//Edges
	for(auto edgeIterator = edges.begin(); edgeIterator != edges.end(); ++edgeIterator){
		s.append("Join attribute: "); 
		s.append((*edgeIterator)->getJoinPredicate().leftNode.getRelation()); 
		s.append(":");
		s.append((*edgeIterator)->getJoinPredicate().leftAttribute);
		s.append(" - "); 
		s.append((*edgeIterator)->getJoinPredicate().rightNode.getRelation()); 
		s.append(":");
		s.append((*edgeIterator)->getJoinPredicate().rightAttribute);
		s.append(", Selectivity: ");
		s.append(to_string((*edgeIterator)->getEstimatedSelectivity())); 
	}
	
	cout << s << endl; 
	
	return s; 
}

