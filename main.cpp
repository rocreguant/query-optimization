#include <iostream>
#include <vector>
#include "query-struct.h"
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
    
    while( getline(stream, word, ',') ){
    	vec.push_back(word);
    }
	return vec;
}

vector<string> process_where(){
	string str;
	
	string word;
    stringstream stream(str);
    
    vector<string> vec;
    
    bool first_it = true;
    while( getline(stream, word, '=') ){
    if(not first_it) cin >> aux;
    	vec.push_back(word);
    	getchar(); //takes the first space between select/from and the next word
    }
	return vec;
}



int main(){
	string aux;
	string phase; //where are we now select/from/where/FAIL
	
	while(cin>> aux){
		if(aux == "select" or aux == "from" or aux == "where") phase = aux;
		
		if(phase == "select"){
			q.select = process_select_from();
		}
		else if(phase == "from"){
			q.from = process_select_from();
		}
		else if(phase == "where"){
		
		}
		else{ //FAIL!!!!
			cout << "Main fail!!! \n we're not in the phase SELECT, FROM neither WHERE"<< endl;
			return -1;
		}
	}

}
