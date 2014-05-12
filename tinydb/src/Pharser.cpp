#include <iostream>
#include <algorithm>
#include <string> 
#include <sstream>
//#include "query-struct.h"
#include "Pharser.hpp"

using namespace std;


class Phaser;
	Pharser::Pharser(){}
	
	Pharser::~Pharser(){}

	/**
	*** Post: Returns a pair with the two attributes <binding, attribute> separated by delim
	***					
	**/
	pair<string, string> Pharser::process_pairs(string word, char delim){

		vector<string> elems;
		std::stringstream ss(word);
		std::string item;
		
		while (std::getline(ss, item, delim)) {
		    elems.push_back(item);
		}
		
		pair<string, string> p;
		if(elems.size() == 1){
			p.first = "";
			p.second = elems[0];
		}
		else{
			p.first = elems[0];
			p.second = elems[1];
		}
	
		return p;
	}


	/**
	*** Post: Returns the condition propetly processed
	***	
	**/
	condition Pharser::process_where(string word){

		// split the condition '='
		pair<string, string> p = process_pairs(word, '=');
		condition c;
		c.comparator = '=';
		c.l_attr = process_pairs(p.first, '.');
		c.r_attr = process_pairs(p.second, '.');
		return c;
	
	}


	/**
	*** Post: Returns a  string without any coma at the end (it does remove the coma, otherwise does nothing
	***	
	**/
	string Pharser::comma_removal(string word){
		std::string::iterator it = word.end() - 2;
		if(*it == ',') throw "You're using more than one coma";
		it = word.end() - 1;
	    if (*it == ',') word.erase(it);
		return word;	
	}


	/**
	*** Post: Returns the query with select, from, where correcly read
	***					(*correctly: no more and no les than 1 time each)
	**/
	query Pharser::read(){
		string word, total = "";
		query vec;
		vector<bool> check(3,false); //this is the vector where we check if we have select/from/where
		int id =-1; //it says in which phase of the query are we
		while(cin >> word){
			std::transform(word.begin(), word.end(), word.begin(), ::tolower); //make the word lowercase
			if(word == "select"){
				if(check[0]) throw "You're using select more than once";
				else check[0] = true;
				id = 0;
				vec.select = vector<string>();
			}
			else if(word == "from"){
				if(check[1]) throw "You're using from more than once";
				else check[1] = true;
				if(!check[0]) throw "You forgot to use the select";
				id = 1;
				vec.from = vector<pair<string, string> >();
			}
			else if(word == "where"){
				if(check[2]) throw "You're using where more than once";
				else check[2] = true;
				if(!check[0]) throw "You forgot to use the select";
				else if(!check[1]) throw "You forgot to use the from";
				id = 2;
				vec.where = vector<condition>();
			}
			else{
				if(id == -1) throw "You're not starting with the select";
				if(id == 0){
					word = comma_removal(word);
					vec.select.push_back(word);
				}
				else if(id == 1){
					pair<string, string> p;
					p.first = word;
					cin >> p.second;
					p.second = comma_removal(p.second);
					vec.from.push_back(p);
				}
				else if(id == 2){
					if(word == "and"){ //this means that we've sucessfully read one condition
					if(vec.where.size() < 0) throw "You should write a condition before using the 'AND' ";
						vec.where.push_back(process_where(total));
						total = "";
					}
					else total = total + word;
				}
			}
		}
		//if the last word is an AND error
		if(word == "and") throw "The last condition can't be empty, otherwise remove the 'AND'";
		else vec.where.push_back(process_where(total)); //here we should processate the last condition
		
		return vec;
	}



/*
    int main()
    {
    	query total;
    	Pharser p;
    	try{
			total = p.read();
	    }
	    catch(char const* er) { std::cerr <<std::endl << "WARNING!! => " << er << std::endl << std::endl;}
	    
	    
	    cout << "query-select" << endl;
	    for(int i=0; i<total.select.size(); ++i) cout << i << " - " << total.select[i] << endl;
	    
	    cout << "query-from" << endl;
	    for(int i=0; i<total.from.size(); ++i) cout << i << " - " << total.from[i].first << ':' << total.from[i].second << endl;
	    
	    cout << "query-where" << endl;
	    for(int i=0; i<total.where.size(); ++i) cout << i << " - *" << total.where[i].l_attr.first << "+"<< total.where[i].l_attr.second << '$' << total.where[i].comparator << '$' <<  total.where[i].r_attr.first << "+"<< total.where[i].r_attr.second << endl;
	    
    }
*/
