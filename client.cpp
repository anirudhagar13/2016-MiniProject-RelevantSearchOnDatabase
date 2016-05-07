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

			//Putting stuff in workload query file on condition
			put_in_file = query;
			replace(put_in_file.begin(),put_in_file.end(), ' ', ',');
			query_file<<"\n"<<put_in_file;
		}

		//QF_IDF Ranking
		#if 1
		Qfidf_rank q1(i1.k_box_tuples,w1,i1.xclude_column);
		q1.calc_max_fq();
		q1.start_qfidf();
        string sent_result = q1.disp_results();
		#endif

        char sent_res[10024];
        strcpy(sent_res, sent_result.c_str());
        sent_res[sent_result.size()] = '\0';

        /* writing results back */
        fd = open(myfifo,O_RDWR);
        if(write(fd, sent_res, sent_result.size()+1)<0)
            perror("Unable to write in pipe");

        close(fd);
        //results sent back

		t = clock() - t;
		printf("%f seconds\n",((float)t)/CLOCKS_PER_SEC);

        cout << "####### TUPLES SENT ##########\n\n";
        cout<<sent_result;
        cout << "##############################" <<endl;
	}

	#endif
	return 0;
}
