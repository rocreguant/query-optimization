#ifndef QUERYGRAPH H
#define QUERYGRAPH H

#include <string>
#include <vector>
#include <unordered_map>
#include <forward_list>
#include "query-struct.h"
#include "Database.hpp"

using namespace std;

struct ConstantPredicate{
	string comparator; 
	string attribute; 
	string constantValue;  

}; 

struct JoinPredicate{
	string comparator; 
	pair<string, string> leftAttribute; 
	pair<string, string> rightAttribute; 

}; 

class Node{
	
	private:
		unsigned estimatedCardinality; 
		vector<ConstantPredicate>* pushedDownPredicates; 
		string relation; 
		string binding; 
		
		unordered_map<string, string> joinedRelations; //Relationname, binding
	
	public: 
		Node(){
			pushedDownPredicates = new vector<ConstantPredicate>; 
		} 
		
		//Getters
		int getEstimatedCardinality(){
			return estimatedCardinality; 
		}
		
		vector<ConstantPredicate>& getPushedDownPredicates(){
			return *pushedDownPredicates; 
		}
		
		string getRelation(){
			return relation; 
		}
		
		string getBinding(){
			return binding; 
		}
		
		//Setters
		void setEstimatedCardinality(int e){
			estimatedCardinality = e; 
		}
		
		void setRelation(string r){
			relation = r; 
		}
		
		void setBinding(string b){
			binding = b; 
		}

		
		//Others
		void addConstantPredicate(ConstantPredicate cp){
			pushedDownPredicates->push_back(cp); 
		}
}; 

class Edge{
	
	private: 
		JoinPredicate* joinPredicate; 
		double estimatedSelectivity; 
	
	public: 
		Edge(){
		
		}
	
		//Getters
		JoinPredicate& getJoinPredicate(){
			return *joinPredicate; 
		}
		
		double getEstimatedSelectivity(){
			return estimatedSelectivity; 
		}
	
		//Setters
		void setJoinPredicate(JoinPredicate* p){
			joinPredicate = p; 
		}
		
		void setEstimatedSelectivity(double s){
			estimatedSelectivity = s; 
		}
	
		~Edge()
		{
			delete joinPredicate;
		}
};

class QueryGraph{

	private: 
		vector<Node> nodes; 
		vector<Edge> edges; 
		Database db; 
		
		void setCardinalities(); 
		void setSelectivities(); 
		
		
	public: 
		QueryGraph(query q, string databasePath);
		
		//Getters
		vector<Node> getNodes(){
			return nodes; 
		}
		
		vector<Edge> getEdges(){
			return edges; 
		}
		
		Node* getNodeWithBinding(string binding); 
		Node* getNode(string relationName); 
		string toString();


};





#endif
