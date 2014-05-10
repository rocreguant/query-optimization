#include "executor.hpp"
#include "Database.hpp"
#include "Table.hpp"
#include "operator/Tablescan.hpp"
#include "operator/CrossProduct.hpp"
#include "operator/Selection.hpp"
#include "operator/Projection.hpp"
#include "operator/Printer.hpp"
#include "operator/Chi.hpp"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
using namespace std;



void Executor::execute(query q){
	 
	Database db; 
	db.open("data/uni"); 
	unordered_map<string, unique_ptr<Tablescan>> tablescanners;
	
	if(q.from.size() > 1){
		 
		//Get all tablescanners
		for(auto it = q.from.begin(); it != q.from.end(); ++it) {
			Table& table = db.getTable(it->first); 
			tablescanners[it->second] (new Tablescan(table)); 
		}
		
		//for(auto it = )
	}
	
	 
	
}


//Constructor
Executor::Executor(){
}


//Desctructor
Executor::~Executor(){

}
