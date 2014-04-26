#include "Database.hpp"
#include "operator/Tablescan.hpp"
#include "operator/CrossProduct.hpp"
#include "operator/Selection.hpp"
#include "operator/Projection.hpp"
#include "operator/Printer.hpp"
#include "operator/Chi.hpp"
#include <iostream>
using namespace std;

int main()
{
	//Open the Database and create references to the tables
	Database db; 
	db.open("data/uni"); 
	Table& studenten = db.getTable("studenten"); 
	Table& hoeren = db.getTable("hoeren"); 
	
	//Tablescanners for used attributes
	unique_ptr<Tablescan> scans1(new Tablescan(studenten)); 
	const Register* names1 = scans1->getOutput("name"); 
	const Register* matrnrs1 = scans1->getOutput("matrnr"); 
	
	unique_ptr<Tablescan>  scanh1(new Tablescan(hoeren)); 
	const Register* matrnrh1 = scanh1->getOutput("matrnr"); 
	const Register* vorlnrh1 = scanh1->getOutput("vorlnr");
	
	unique_ptr<Tablescan>  scanh2(new Tablescan(hoeren)); 
	const Register* matrnrh2 = scanh2->getOutput("matrnr"); 
	const Register* vorlnrh2 = scanh2->getOutput("vorlnr");
	
	unique_ptr<Tablescan>  scans2(new Tablescan(studenten)); 
	const Register* names2 = scans2->getOutput("name"); 
	const Register* matrnrs2 = scans2->getOutput("matrnr"); 
	 

	//---------------Query Processing-----------------------
	
	// Join hoerer1(h1) with studenten1(s1) on matrnr
	unique_ptr<CrossProduct> cp1(new CrossProduct(move(scanh1),move(scans1)));
	unique_ptr<Selection> s1(new Selection(move(cp1),matrnrh1, matrnrs1 ));
	
	// Select all from s1 where name = Schopenhauer
	Register schopenhauer; schopenhauer.setString("Schopenhauer"); 
	unique_ptr<Selection> s2(new Selection(move(s1),names1, &schopenhauer ));
	
	// Join s2(lectures that Schopenhauer attends) with hoerer2 on vorlesungsnummer
	unique_ptr<CrossProduct> cp2(new CrossProduct(move(s2), move(scanh2))); 
	unique_ptr<Selection> s3(new Selection(move(cp2), vorlnrh1, vorlnrh2)); 
	
	// Join s3 with studenten2(2) on matrikelnummer
	unique_ptr<CrossProduct> cp3(new CrossProduct(move(s3), move(scans2)));
	unique_ptr<Selection> s4(new Selection(move(cp3), matrnrh2, matrnrs2));
	
	//Select where name is not equal to "Schopenhauer"
	unique_ptr<Chi> chi1(new Chi(move(s4),Chi::NotEqual,names2,&schopenhauer));
	const Register* chiResult=chi1->getResult();
	unique_ptr<Selection> s5(new Selection(move(chi1),chiResult));
	
	// Project the name of the students
	unique_ptr<Projection> p1(new Projection(move(s5), {names2})); 
	

	//Print the names of the students
	Printer out(move(p1));
	out.open();
	while (out.next());
	out.close();
}
