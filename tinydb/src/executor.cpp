#include "executor.hpp"
#include "Database.hpp"
#include "Table.hpp"
#include "operator/Tablescan.hpp"
#include "operator/CrossProduct.hpp"
#include "operator/Selection.hpp"
#include "operator/Projection.hpp"
#include "operator/Printer.hpp"
#include "operator/Chi.hpp"
#include "operator/Operator.hpp"
#include <iostream>
#include <cstddef>
#include <unordered_map>
#include <vector>
#include <string>
using namespace std;


// Populates the map according to protocol mentioned in executeCanonicalPlan
void populateRegisterTable(std::unordered_map<std::string, std::unordered_map<std::string, const Register*>>* map, Tablescan& scanner, std::vector<std::string>* names, std::string binding)
{
   for(int i = 0; i < names->size(); i++)
   {
      string name = (*names)[i];
      if(map->count(binding) == 0)
      {
         (*map)[binding] = unordered_map<std::string, const Register*>();
      }
      if((*map)[binding].count(name) == 0)
      {
         (*map)[binding][name] = scanner.getOutput(name);
         if((*map)[binding][name] == 0)
         {
            std::cout << "noooooooooooo, name:" << name << " for binding: " << binding << std::endl;
            exit(0);
         }
      }
   }
}


void Executor::execute(query q){
	 
	Database db; 
	db.open("data/uni"); 
	if(q.from.size() > 1){
		
		unordered_map<string, unordered_map<string, const Register*>> registers; 
		unique_ptr<Operator> crossproduct(nullptr); 
		
		//Make crossproducts
		int counter = 0; 
		for(auto it = q.from.begin(); it != q.from.end(); ++it) {
			unique_ptr<Tablescan> tablescan(new Tablescan(db.getTable(it->first))); 
			//Used for constants push down
	
			//Get names of all attributes an populate tables
			vector<string> names = db.getTable(it->first).getAttributeNames();
			populateRegisterTable(&registers, *tablescan, &names, it->second); 		
			unique_ptr<Operator> selection(move(tablescan)); 
			for(auto it2 = q.where.begin(); it2 != q.where.end(); it2++){
				if((*it2).r_attr.first == "" && (*it2).l_attr.first == it->second){ //See if bindings are the same  
					Register* registertmp = new Register(); //tmpp
					
					//Fix for loop here later for all different types
					registertmp->setString((*it2).r_attr.second);
					
					
					unique_ptr<Operator> seltmp(new Selection(move(selection), registers[it->second][(*it2).l_attr.second] ,registertmp )); //tmp
					selection.swap(seltmp); 
					
				}
			}
			if(crossproduct == nullptr){ 
				crossproduct.swap(selection); 
			}
			else{
				unique_ptr<Operator> dummy(new CrossProduct(move(selection), move(crossproduct)));
				crossproduct.swap(dummy);   
			}
		}
	
		for(auto it2 = q.where.begin(); it2 != q.where.end(); it2++){
			if((*it2).r_attr.first != ""){ //See if bindings are the 
				string rightTable; 
				string leftTable; 
				for(auto it3 = q.from.begin(); it3 != q.from.end(); ++it3 ){
					if(it3->second == (*it2).r_attr.first) {
						rightTable = it3->second; 
					}
					else if(it3->second == (*it2).l_attr.first) {
						leftTable = it3->second; 
					}
				}
				unique_ptr<Operator> seltmp(new Selection(move(crossproduct), registers[leftTable][(*it2).l_attr.second] ,registers[rightTable][(*it2).r_attr.second] )); //tmp
				crossproduct.swap(seltmp); 
			}	
		}
		
		//Print the names of the students
		Printer out(move(crossproduct));
		out.open();
		while (out.next());
		out.close();
	}	
}


//Constructor
Executor::Executor(){
}


//Desctructor
Executor::~Executor(){

}

/**
 * just an example to show that the code can process the query: 
 * SELECT * FROM studenten s, hoeren h, vorlesungen v
 * WHERE s.matrnr=h.matrnr
 * AND s.name = Schopenhauer
 * AND v.titel = Grundzüge
 */
int main(){
	
	Executor* executor = new Executor(); 
	
	query q; 
	condition c;
	condition c2; 
	condition c3; 
	
	c3.comparator = "="; 
	c3.l_attr = make_pair("v", "titel"); 
	c3.r_attr = make_pair("", "Grundzüge"); 
	
	c.comparator = "="; 
	c.l_attr = make_pair("s", "name"); 
	c.r_attr = make_pair("", "Schopenhauer"); 
	
	c2.comparator = "="; 
	c2.l_attr = make_pair("s", "matrnr"); 
	c2.r_attr = make_pair("h", "matrnr"); 
	
	
	condition c4; 
	c4.comparator = "="; 
	c4.l_attr = make_pair("v", "vorlnr"); 
	c4.r_attr = make_pair("h", "vorlnr"); 

	q.from.push_back(make_pair("studenten", "s"));
	q.from.push_back(make_pair("hoeren", "h"));
	q.from.push_back(make_pair("vorlesungen", "v"));
	q.where.push_back(c); 
	q.where.push_back(c3); 
	q.where.push_back(c2); 
	q.where.push_back(c4); 
	
	executor->execute(q);
	  
	
	
	return 0; 
}
