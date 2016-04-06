#include "index_workload.h"
using namespace std;

class Qfidf_rank
{
	public:
	map<string,double> k_box_tuples;
	map<double,string> final_result;
	workload windex_obj;
	map<int,unsigned int> max_freq;

	Qfidf_rank(map<string,double> box,workload index)
	{
		k_box_tuples = box;
		windex_obj = index;
	}

	void start_qfidf()
	{
		double add_score;
		for(auto e : k_box_tuples)
		{
			add_score = tup_calc(e.first);
			k_box_tuples[e.first] += add_score; 

		}

		//Putting in a new map to arrange results
		for(auto g : k_box_tuples)
			final_result[g.second] = g.first;

	}


	void calc_max_fq()
	{
		unsigned int max = 0; 
		for(auto a : windex_obj.workload_col_index)
		{
			for(auto b : a.second)
			{
				if(max < b.second.size())
					max = b.second.size();
			}
			max_freq[a.first] = max;
			max = 0;
		}

		#if 0
		cout<<"\n*************RQFMAX of all columns in workload**************\n";
		for(auto rqf : max_freq)
			cout<<"column_no = "<<rqf.first<<" : "<<rqf.second<<"\n";
		cout<<"************************************************************\n";
		#endif
	}
	

	double tup_calc(string tuple)
	{
		vector<string> col;
		double cumm_score(0.0),score(0.0);
		tokenize(col,tuple,',');
		int col_no = 1;
		
		//RQF/RQF calculation using Indexing
		for(auto a : col)	//Exclude user queried columns
		{
			score = index_n_score(a,col_no);
			cumm_score += score;
			++col_no;
			//Printing each column and its QF Score obtained
			//cout<<a<<"-"<<score<<" | ";
		}	
			
		//To Delimit k-box tuples
		//cout<<"\n^^^^^^^^^^^^^^^^^^^^ "<<col.size()<<" ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n";
		return cumm_score;
	}

	double index_n_score(string one_ele,int column_no)	//one column one cell value @ a time
	{
		double col_score(0.0);
        stringstream value(one_ele);
        string new_value;
        unsigned int RQF(0);

        while(getline(value,new_value,' '))			//Actual Indexing each term of each column from workload col_cell_index
        {    	
        	if(max_freq[column_no])
        	{
	        	RQF = windex_obj.workload_col_index[column_no][new_value].size();
	        	col_score += (double)RQF / (double)max_freq[column_no];

	        	//RQF/RQFMAX for each column using workload column cell index
	        	//cout<<"Column No - "<<column_no<<" : "<<"RQF = "<<RQF<<" : "<<"RQFMAX = "<<max_freq[column_no]<<" : "<<new_value<<" : "<<col_score<<"\n";

			}
		}

		return col_score;
	}

	void tokenize(vector<string>& v, const string& line,char delim)
	{
		string final;
		for(auto pres : line)
		{
			if(pres != delim)
				final += pres;
			else
			{
				if(final != "")
				{
					v.push_back(final);
					final = "";
				}
				else
					v.push_back("_E_M_P_T_Y_");
			}
		}
		v.push_back(final);
	}

	void disp_results()
	{
		#if 0
		cout<<"\n++++++++++++++++++TupleNo Vs QFScore+++++++++++++++++++\n";
		for(auto b : k_box_tuples)		//k_box tuples(Tuple VS NewScore)
			cout<<b.first<<" +>>> "<<b.second<<"\n\n";
		cout<<"************************************************************\n";
		#endif

		#if 1
		cout<<"\n#################### Final Relevancy Ranked Result ##################\n\n";
		//Printing in reverse order as map already arranged
		map<double,string>::reverse_iterator rit(final_result.rbegin());
  		for (; rit!=final_result.rend(); ++rit)		//Final Arranged tie broken(Tuple VS NewScore)
			cout<<rit->first<<" +>>> "<<rit->second<<"\n\n";
		cout<<"************************************************************\n";
		#endif
	}
	
};