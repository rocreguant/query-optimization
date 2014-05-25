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

class Node{
	
	private:
		unsigned estimatedCardinality; 
		vector<ConstantPredicate*> pushedDownPredicates; 
		string relation; 
		string binding; 
	
	public: 
		Node(){
			//pushedDownPredicates = new vector<ConstantPredicate*>; 
		} 
		
		//Getters
		int getEstimatedCardinality(){
			return estimatedCardinality; 
		}
		
		vector<ConstantPredicate*>& getPushedDownPredicates(){
			return pushedDownPredicates; 
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
		void addConstantPredicate(ConstantPredicate* cp){
			pushedDownPredicates.push_back(cp); 
		}
		
		~Node(){
			for(auto it = pushedDownPredicates.begin(); it != pushedDownPredicates.end(); ++it){
				delete (*it); 
			}
		}
		
		std::string toString()
		{
			std::string s = "";
			s += (relation + " " + binding); //+ "{"+ SSTR(estimatedCardinality)+"}";
			s += " [";
			for(unsigned i = 0; i < pushedDownPredicates.size(); i++)
			{
				s += (" (" + pushedDownPredicates[i]->attribute + " " + pushedDownPredicates[i]->comparator + " " + pushedDownPredicates[i]->constantValue + ")");
			}
			s+="]";
			return s;
		}
}; 

struct JoinPredicate{
	JoinPredicate(Node& ln, Node& rn)
		:leftNode(ln), rightNode(rn) //Pre-initializing
	{}
	string comparator; 
	string leftAttribute; 
	string rightAttribute; 
	Node& leftNode; 
	Node& rightNode; 
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
		
		std::string toString()
		{
			std::string s = "";
			s += joinPredicate->leftNode.toString();
			s += "---";
			s += joinPredicate->leftAttribute;				
			s += " " + joinPredicate->comparator + " ";
			s += joinPredicate->rightAttribute;				
			s += "---";
			s += joinPredicate->rightNode.toString();
			
			return s;
		}
};

class QueryGraph{

	private: 
		vector<Node*> nodes; 
		vector<Edge*> edges; 
		Database db; 
		
		void setCardinalities(); 
		void setSelectivities(); 
		
		
	public: 
		QueryGraph(query q, string databasePath);
		
		//Getters
		vector<Node*> getNodes(){
			return nodes; 
		}
		
		vector<Edge*> getEdges(){
			return edges; 
		}
		
		Node& getNodeWithBinding(string binding); 
		Node& getNode(string relationName); 
		string toString();
		
		~QueryGraph(); 


};





#endif
