#include "qfidf_ranking.h"
#include <time.h>
using namespace std;

#define MAX_BUF 1024

bool process_query(string&,bool);
void add_to_workload(string,string);

int main()
{
	string data_file = "data_files/lamb3.csv";
	//data_files/lamb3.csv
	//data_files/Sacramentorealestate.csv
	string work_file = "data_files/work2.csv";
	//data_files/work2.csv
	//data_files/Sacramentorealestate_workload.csv
    string total_tuples = "";

	//DB Index Generation
	Index index_gen(data_file);
	index_gen.display_tuple_index();
	index_gen.display_cell_index();
	index_gen.display_column_index();
	index_gen.display_column_cell_index();
	cout<<"***************************DATABASE INDEXING DONE****************************\n";

	//Workload Index Generation
	workload w1(work_file,index_gen);
	int num_of_queries_made = w1.get_data_file();
	cout << "num_of_queries_made = " << num_of_queries_made << endl;
	w1.display_workload_column_index();
	cout<<"***************************WORKLOAD INDEXING DONE****************************\n";

	//IDF Ranking
	#if 1
	Idf_rank i1(index_gen,index_gen.total_size,data_file);
	string query;
	string put_in_file;

    //Attaching total tuples for client statistics
    total_tuples = to_string(i1.tot_tups);


	//timestamp code
	clock_t t;

	//Connecting with named pipe for cgi based UI
	int fd,test = -1;
	char myfifo[] = "/usr/lib/cgi-bin/myfifo";
    char buf[MAX_BUF];


	while(true)
	{
		//t = clock();
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
		//start timer		
		t = clock();
		if(query == "exit" || query == "exit@")
		{
			break;
		}
		else
		{
            bool set_var_addwork = false;
            set_var_addwork = process_query(query,set_var_addwork);

            cout<<"After Processing = "<<query<<endl;

			i1.fetch_tuples(query);		//please enter query here

			//Putting stuff in workload query file on condition
            if(set_var_addwork)
            	add_to_workload(work_file,query);

		}
		
		//perform qf-idf only when workload is some large enough value, if cond->true
		//update string sent_result via qf-idf
		string sent_result;
		if(num_of_queries_made > 10)
		{
			//QF_IDF Ranking
			#if 1
			Qfidf_rank q1(i1.k_box_tuples,w1,i1.xclude_column);
			q1.calc_max_fq();
			q1.start_qfidf();
		    sent_result = q1.disp_results();
			#endif

		    //Fetching Time
		    t = clock() - t;
		    printf("%f seconds\n",((float)t)/CLOCKS_PER_SEC);
		}
		#if 1
		else
		{
			sent_result = i1.disp_results();
			
			//Fetching Time
		    t = clock() - t;
		    printf("%f seconds\n",((float)t)/CLOCKS_PER_SEC);
		}
		#endif
        //Attaching other statistics info to be displayed on client table

        sent_result += "!#" + total_tuples
                    + "$#" + to_string(k_box_size)
                    + "^#" + to_string(t);
        //Attachment complete

        char sent_res[10024];
        strcpy(sent_res, sent_result.c_str());
        sent_res[sent_result.size()] = '\0';

        /* writing results back */
        fd = open(myfifo,O_RDWR);
        if(write(fd, sent_res, sent_result.size()+1)<0)
            perror("Unable to write in pipe");

        close(fd);
        //results sent back

        cout << "####### TUPLES SENT ##########\n\n";
        cout<<sent_result;
        cout << "##############################" <<endl;
	}

	#endif
	return 0;
}

void add_to_workload(string workload_file, string put_in_file)
{

    //Appending user queries in workload
    ofstream query_file;
    query_file.open(workload_file,std::ios_base::app | std::ios_base::out);

    replace(put_in_file.begin(),put_in_file.end(), ' ', ',');
    query_file<<"\n"<<put_in_file;

    //Closing workload file
    query_file.close();
}

bool process_query(string& query,bool set_var_addwork)
{
    //Searching for query to be added to workload
    auto found = find(query.begin(),query.end(),'@');

    if(found != query.end())
    {
        set_var_addwork = true;
        query.erase(found,found+1);
    }

    for(auto& x: query)
    {
        //Capitalizing Characters
        x = toupper(x);
    }

    return set_var_addwork;
}
