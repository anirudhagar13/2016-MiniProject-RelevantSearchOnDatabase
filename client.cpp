#include "qfidf_ranking.h"
using namespace std;

int main()
{
	string data_file = "lamb3.csv";
	string work_file = "work.csv";

	//DB Index Generation
	Index index_gen(data_file);
	index_gen.display_tuple_index();
	index_gen.display_cell_index();
	index_gen.display_column_index();
	index_gen.display_column_cell_index();
	cout<<"***************************DATABASE INDEXING DONE****************************\n";

	//Workload Index Generation
	workload w1(work_file,index_gen);
	w1.get_data_file();
	w1.display_workload_column_index();
	cout<<"***************************WORKLOAD INDEXING DONE****************************\n";

	//IDF Ranking
	#if 1
	Idf_rank i1(index_gen,index_gen.total_size,data_file);	
	string query;

	while(true)
	{
		cout <<"Enter query to proceed `exit` to Quit Search\n";
		getline(cin,query);
		if(query == "exit")
			break;
		else
			i1.fetch_tuples(query);		//please enter query here

		//QF_IDF Ranking
		#if 1
		Qfidf_rank q1(i1.k_box_tuples,w1);
		q1.calc_max_fq();
		q1.start_qfidf();
		q1.disp_results();
		#endif
	}
	
	#endif

	/*QUERIES:
	"JEEP GRAND CHEROKEE 12/2/2009"
	2007 DODGE CHARGER V6 2.7L V6 M
	7/8/2009 2006 3 CHEVROLET MALIBU 4C 2.2L I4 MP LT 4D SEDAN LT MAROON AUTO
	2002 7 SATURN L SERIES L20 4D SEDAN L200 GOLD*/

	return 0;
}