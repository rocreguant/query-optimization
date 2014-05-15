#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include "Database.hpp"
#include "query-struct.h"
#include <unordered_map>
#include <string>



class Executor  {

		public: 
		Executor(); 
		~Executor(); 
		void execute(query q); 
		
		
		private: 
		
		unordered_map<string, unordered_map<string, const Register*>> getRegisters(query q, Database db); 


}; 


#endif
