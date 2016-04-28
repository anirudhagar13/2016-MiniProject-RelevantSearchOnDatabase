#include "qfidf_ranking.h"
#include <time.h>
using namespace std;

#define MAX_BUF 1024

int main()
{
	string data_file = "lamb3.csv";
	string work_file = "work2.csv";

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
	string put_in_file;

	//Appending user queries in workload
	ofstream query_file;
    query_file.open("work2.csv",std::ios_base::app | std::ios_base::out);

	//timestamp code
	clock_t t;

	//Connecting with named pipe for cgi based UI
	int fd,test = -1;
    char * myfifo = "/usr/lib/cgi-bin/myfifo";
    char buf[MAX_BUF];


	while(true)
	{
		t = clock();
		//cout <<"Enter query to proceed `exit` to Quit Search\n";
		//getline(cin,query);

		//Connecting with named pipe for cgi based UI
		while(test < 1)
        {
            fd = open(myfifo, O_RDONLY);
            test = read(fd, buf, MAX_BUF);
            buf[test] = '\0';
            close(fd);
        }
        string query(buf);
        test = -1;
        //UI Code ends here
        cout<<"RECEIVED = "<<query<<endl;

		if(query == "exit")
		{
			break;
			//Closing workload file
			query_file.close();
		}
		else
		{
			i1.fetch_tuples(query);		//please enter query here
			//Putting stuff in workload query file
			put_in_file = query;
			replace(put_in_file.begin(),put_in_file.end(), ' ', ',');
			query_file<<"\n"<<put_in_file;
		}

		//QF_IDF Ranking
		#if 1
		Qfidf_rank q1(i1.k_box_tuples,w1,i1.xclude_column);
		q1.calc_max_fq();
		q1.start_qfidf();
		q1.disp_results();
		#endif

		t = clock() - t;
		printf("%f seconds\n",((float)t)/CLOCKS_PER_SEC);
	}

	#endif

	/*QUERIES:
	"JEEP GRAND CHEROKEE 12/2/2009"
	2007 DODGE CHARGER V6 2.7L V6 M
	7/8/2009 2006 3 CHEVROLET MALIBU 4C 2.2L I4 MP LT 4D SEDAN LT MAROON AUTO
	2002 7 SATURN L SERIES L20 4D SEDAN L200 GOLD*/

	return 0;
}
