#include <iostream>
#include <vector>
#include "query-struct.h"
#include <stdio.h>
#include <sstream>
#include <string>
#include <boost/algorithm/string.hpp>
//#include <algorithm>

using namespace std;
query q;
bool atribute_checker(string s){

}

/****
	* pre: You're at the select or from phase pharsing
	* post: you'll get all the words in a vector which where separated by commas
	*/
vector<string> process_select_from(){
	string str;
	getchar(); //takes the first space between select/from and the next word
	string word;
    stringstream stream(str);
    
    vector<string> vec;
    
    //while( getline(stream, word, ',') ){
    while(cin>> word){
    cout << "push word: " << word << endl;
    	vec.push_back(word);
    }
	return vec;
}

pair<string, string> process_where(){
	string str;
	
	string word;
    stringstream stream(str);
    
    vector<pair<string, string> > vec;
    char charAux[255]; 
    
	cin.getline(charAux, 255);

	str = charAux;
	std::vector<std::string> x;
	boost::split(x, str, boost::is_any_of("="));
	pair<string, string> p;
	if(x.size()>2) {
		cout << "ERROR: More than one argument for a comparasion" << endl;
		//return -1;
	}
	else{

		p.first = x[0];
		p.second = x[1];
	}

	return p;
}



int main(){
	string aux;
	string phase; //where are we now select/from/where/FAIL
	vector<bool> check(3,false); //this is the vector where we check if we have select/from/where
	while(cin>> aux){
		if(aux == "where") phase = aux; //it's separated because of the order of the words
		if(aux == "select" or aux == "from") phase = aux;
		else{
			if(phase == "select"){ //might not have any argument ???????????
				cout << "select: " << aux << endl;
				q.select.push_back(aux);
				check[0] = true;
			}
			else if(phase == "from"){
				cout << "from: " << aux << endl;
				q.from.push_back(aux);
				check[1] = true;
			}
			else if(phase == "where"){
				q.where.push_back(process_where());
				cout << "comparasion: " << q.where[0].first << " equals " << q.where[0].second << endl;
				check[2] = true;
			}
			else{ //FAIL!!!!!
				cout << "Main fail!!! \n we're not in the phase SELECT, FROM neither WHERE"<< endl;
				return -1;
			}
		}
	}
	
	for(int i=0; i< check.size(); ++i){
		if(not check[i]){
			cout << "There was not ";
			if(i==0) cout << "SELECT" << endl;
			else if(i==1) cout << "FROM" << endl;
			else if(i==2) cout << "WHERE" << endl;
			return -1;
		}
	}
	
}
