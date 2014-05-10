#include "Database.hpp"
#include "operator/Tablescan.hpp"
#include "operator/CrossProduct.hpp"
#include "operator/Selection.hpp"
#include "operator/Projection.hpp"
#include "operator/Printer.hpp"
#include "operator/Chi.hpp"
#include "operator/HashJoin.hpp"
#include <iostream>
#include <set>
using namespace std;

int main()
{
	//Open the Database and create references to the tables
	Database db; 
	db.open("data/uni"); 
	Table& professoren = db.getTable("professoren"); 
	Table& hoeren = db.getTable("hoeren"); 
	Table& vorlesungen = db.getTable("vorlesungen"); 
	
	//Tablescanners for used attributes
	unique_ptr<Tablescan> scanp(new Tablescan(professoren)); 
	const Register* namep = scanp->getOutput("name"); 
	const Register* persnrp =scanp->getOutput("persnr");
	
	unique_ptr<Tablescan> scanv(new Tablescan(vorlesungen)); 
	const Register* vorlnrv = scanv->getOutput("vorlnr");
	const Register* gelesenvon = scanv->getOutput("gelesenvon"); 
	
	unique_ptr<Tablescan> scanh1(new Tablescan(hoeren));
	const Register* matrnrh1 = scanh1->getOutput("matrnr"); 
	const Register* vorlnrh1 = scanh1->getOutput("vorlnr"); 
	
	unique_ptr<Tablescan> scanh2(new Tablescan(hoeren)); 
	const Register* matrnrh2 = scanh2->getOutput("matrnr"); 
	const Register* vorlnrh2 = scanh2->getOutput("vorlnr"); 
	
	//---------------Query Processing-----------------------
	
	//Crossproduct cp1 of two "hoerer"-tables
	unique_ptr<CrossProduct> cp1(new CrossProduct(move(scanh1), move(scanh2))); 
	
	//Select results from cp1 where matnr is not equal
	unique_ptr<Chi> chi1(new Chi(move(cp1),Chi::NotEqual,matrnrh1,matrnrh2));
	const Register* chiResult1=chi1->getResult();
	unique_ptr<Selection> s1(new Selection(move(chi1),chiResult1));
	
	//Select matnr from cp1 where vorlnr is equal. 
	unique_ptr<Chi> chi2(new Chi(move(s1),Chi::Equal,vorlnrh1,vorlnrh2));
	const Register* chiResult2=chi2->getResult();
	unique_ptr<Selection> s2(new Selection(move(chi2),chiResult2));
	
	//Join with vorlesungen on the ID of the lecture
	unique_ptr<CrossProduct> cp2(new CrossProduct(move(s2), move(scanv)));
	unique_ptr<Selection> s3(new Selection(move(cp2), vorlnrh1, vorlnrv)); 
	
	//Join with professoren on the personal number of the professor
	unique_ptr<CrossProduct> cp3(new CrossProduct(move(s3), move(scanp))); 
	unique_ptr<Selection> s4(new Selection(move(cp3), gelesenvon, persnrp)); 
	
	//Project the name of the Professors
	unique_ptr<Projection> p1(new Projection(move(s4), {namep}));  
	
	//Create a Set for printing unique values. 
	std::set<string> results; 
	p1->open();
	while (p1->next()) results.insert(namep->getString()); 
	p1->close();
	
	//Print the distinct values from the set
	for (auto i = results.begin(); i != results.end(); ++i) {
    std::cout << (*i) << std::endl;
	}
}
