#include <iostream>
#include <fstream>
#include <string>
#include "idf_ranking.h"
using namespace std;

int main()
{
	//Index Generation
	Index index_gen("lamb2.csv");
	index_gen.display_tuple_index();
	index_gen.display_cell_index();
	index_gen.display_column_index();
	cout<<"***************************INDEXING DONE****************************\n";

	//IDF Ranking
	Idf_rank i1(index_gen,3000);		//please enter total DB tuples here
	string query;
	while(true)
	{
		cout <<"Enter query to proceed `exit` to Quit Search\n";
		getline(cin,query);
		if(query == "exit")
			exit(1);
		else
			i1.display_tuples(query);		//please enter query here
	}

	/*QUERIES:
	"JEEP GRAND CHEROKEE 12/2/2009"*/
	

	return 0;
}