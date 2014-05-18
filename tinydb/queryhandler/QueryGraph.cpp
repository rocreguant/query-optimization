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
		nodes.push_back(*tmpNode); 
	}
	
	
	//Add edges for each join predicate, and select predicate for nodes
	for(auto whereIterator = q.where.begin(); whereIterator != q.where.end(); ++whereIterator){
		if((*whereIterator).r_attr.first == ""){ // If constant predicate
			ConstantPredicate* cp = new ConstantPredicate(); 
			cp->comparator = (*whereIterator).comparator;
			cp->attribute = (*whereIterator).l_attr.second; 
			cp->constantValue = (*whereIterator).r_attr.second; 
			
			Node* node = QueryGraph::getNodeWithBinding((*whereIterator).l_attr.first); 
			node->addConstantPredicate(*cp); 
			
		}
		else{
			Edge *tmpEdge = new Edge(); 
			JoinPredicate* jp = new JoinPredicate(); 
			jp->comparator = (*whereIterator).comparator;

			Node* nodeLeft = QueryGraph::getNodeWithBinding((*whereIterator).l_attr.first); 
			Node* nodeRight = QueryGraph::getNodeWithBinding((*whereIterator).r_attr.first); 
			
			jp->leftAttribute.first = nodeLeft->getRelation() ; jp->leftAttribute.second = (*whereIterator).l_attr.second;
			jp->rightAttribute.first = nodeRight->getRelation(); jp->rightAttribute.second = (*whereIterator).r_attr.second;
			
			tmpEdge->setJoinPredicate(jp); 
			
			edges.push_back(*tmpEdge); 
		}
	}
	
	setCardinalities(); 
	setSelectivities(); 
}


void QueryGraph::setCardinalities(){
	for(auto nodeIterator = nodes.begin(); nodeIterator != nodes.end(); ++nodeIterator){
		Table& table = db.getTable((*nodeIterator).getRelation()); 
		
		if((*nodeIterator).getPushedDownPredicates().size() == 0){ //Set to table size if no push downs
			(*nodeIterator).setEstimatedCardinality(table.getCardinality()); 
		}
		else if((*nodeIterator).getPushedDownPredicates().size() > 0){ //Loop over all pushdowns and reduce cardinality depending on domain
			//Loop over all pushed down predicates
			unsigned cardinality = table.getCardinality(); 
			for(auto predicateIterator = (*nodeIterator).getPushedDownPredicates().begin(); predicateIterator != (*nodeIterator).getPushedDownPredicates().end(); ++predicateIterator){
				int attrIndex = table.findAttribute((*predicateIterator).attribute);
				const Attribute& attr = table.getAttribute(attrIndex);  
				unsigned domain = attr.getUniqueValues(); 		//Why this returns 100 always??? O.o 
				
				cardinality = domain/cardinality;  
			}
			(*nodeIterator).setEstimatedCardinality(cardinality); 
		}
	}
}


void QueryGraph::setSelectivities(){
	for(auto edgeIterator = edges.begin(); edgeIterator != edges.end(); ++edgeIterator){
		Node* leftNode = getNode((*edgeIterator).getJoinPredicate().leftAttribute.first); 
		Node* rightNode = getNode((*edgeIterator).getJoinPredicate().rightAttribute.first); 
		
		//get relations names
		string leftRelation = leftNode->getRelation(); 
		string rightRelation = rightNode->getRelation(); 
		
		//get attributes names
		string leftAttribute = (*edgeIterator).getJoinPredicate().leftAttribute.second;
		string rightAttribute = (*edgeIterator).getJoinPredicate().rightAttribute.second; 
		
		//get domains of both attributes
		Table& leftTable = db.getTable(leftRelation); 
		int leftAttrIndex = leftTable.findAttribute(leftAttribute);
		const Attribute& leftAttr = leftTable.getAttribute(leftAttrIndex);  
		unsigned leftDomain = leftAttr.getUniqueValues();
		
		Table& rightTable = db.getTable(rightRelation); 
		int rightAttrIndex = rightTable.findAttribute(rightAttribute);
		const Attribute& rightAttr = rightTable.getAttribute(rightAttrIndex);  
		unsigned rightDomain = rightAttr.getUniqueValues(); 
		
		if(leftDomain > rightDomain){
			(*edgeIterator).setEstimatedSelectivity((double(1)/double(leftDomain))); 
		}
		else{
			(*edgeIterator).setEstimatedSelectivity((double(1)/double(rightDomain))); 
		}
	}
	
}


Node* QueryGraph::getNode(string relationName){
	for(auto nodeIterator = nodes.begin(); nodeIterator != nodes.end(); ++nodeIterator){
		if((*nodeIterator).getRelation() == relationName){
			return &(*nodeIterator); 
		} 
	}
	
	throw;
	
}

Node* QueryGraph::getNodeWithBinding(string binding){
	for(auto nodeIterator = nodes.begin(); nodeIterator != nodes.end(); ++nodeIterator){
		if((*nodeIterator).getBinding() == binding){
			return &(*nodeIterator); 
		}
	}
	
	throw;
	
}


string QueryGraph::toString(){
	string s; 
	
	//Nodes
	s.append("Nodes: "); 
	s.append("\n"); 
	for(auto nodeIterator = nodes.begin(); nodeIterator != nodes.end(); ++nodeIterator){
		s.append("Relation: "); 
		s.append((*nodeIterator).getRelation()); 
		s.append(", Binding: ");
		s.append((*nodeIterator).getBinding());
		s.append(", Estimated Cardinality: ");
		s.append(to_string((*nodeIterator).getEstimatedCardinality()));  
		s.append("\n"); 
	}
	
	s.append("\n"); 
	s.append("Edges: "); 
	s.append("\n"); 
	
	//Edges
	for(auto edgeIterator = edges.begin(); edgeIterator != edges.end(); ++edgeIterator){
		s.append("Join attribute: "); 
		s.append((*edgeIterator).getJoinPredicate().leftAttribute.first); 
		s.append(":");
		s.append((*edgeIterator).getJoinPredicate().leftAttribute.second);
		s.append(" - "); 
		s.append((*edgeIterator).getJoinPredicate().rightAttribute.first); 
		s.append(":");
		s.append((*edgeIterator).getJoinPredicate().rightAttribute.second);
		s.append(", Selectivity: ");
		s.append(to_string((*edgeIterator).getEstimatedSelectivity())); 
	}
	
	cout << s << endl; 
	return s; 
}

