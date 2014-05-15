#ifndef PHARSER_HPP
#define PHARSER_HPP

#include "query-struct.h"


class Pharser  {

		public: 
		Pharser(); 
		~Pharser(); 
//		void execute(query q); 
		query read();
		
		
		
		private: 
		pair<string, string> process_pairs(string word, char delim);
		condition process_where(string word);
		string comma_removal(string word);
		
		

}; 


#endif
