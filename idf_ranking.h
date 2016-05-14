#include "index_data.h"
using namespace std;
#define k_box_size 30

class Idf_rank
{
	public:
	Index index_obj;
	map<unsigned int,double> idf_calc;
	unsigned int tot_tups;
	vector<unsigned int> k_box;
	map<string,double> k_box_tuples;
	string data_file;
	vector<int> xclude_column;

	//Default Ctor
	Idf_rank(){};

	Idf_rank(Index i,unsigned int no,string file)
	{
		index_obj = i;
		tot_tups = no;
		data_file = file;
	}

	//Copy Assignment
	Idf_rank& operator=(const Idf_rank& rhs)
	{
		if(this != &rhs)
		{
			index_obj = rhs.index_obj;
			idf_calc = rhs.idf_calc;
			tot_tups = rhs.tot_tups;
			k_box = rhs.k_box;
		}
		return *this;
	}

	void hash_terms(string query)
	{
		vector<unsigned int> tups;
		vector<string> split_words;
		tokenize(split_words,query);

		//Clearing all collections
		xclude_column.clear();
		k_box.clear();
		idf_calc.clear();
		k_box_tuples.clear();
		//Collections cleared

		for(auto ele : split_words)
		{
			tups = index_obj.cell_index[ele];
			if(tups.size() != 0)		//Handles zero occurence case
				calc_idf(tups,log((double)tot_tups/(double)tups.size()));

			//user query columns xclusion
			xclude_column.push_back(exclude_query_columns(ele));
		}
	}

	void calc_idf(vector<unsigned int> tups,double score)
	{
		for(auto a : tups)
		{
			if(idf_calc.find(a) != idf_calc.end())
				idf_calc[a] += score;
			else
				idf_calc[a] = score;
		}

	}

	void tokenize(vector<string>& v, const string& line)
	{
		string final;
		for(auto pres : line)
		{
			if(pres != ' ')
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

	unsigned int min_box_element()
	{
		double min = 100000;
		unsigned int tup;
		for(auto a : k_box)
		{
			if(min > idf_calc[a])
			{
				min = idf_calc[a];
				tup = a;
			}
		}

		return tup;
	}

	void create_box()
	{
		double min = -1;
		unsigned int min_tup = 0;
		for(auto counter : idf_calc)
		{
			if(k_box.size() < k_box_size)
				k_box.push_back(counter.first);

			else
			{
				if(min_tup == 0)		//makes k_box creation faster for first time k_box gets filled
					min_tup = min_box_element();

				if(idf_calc[min_tup] < counter.second)		//Only searches for min_element when a new entry made in k_box
				{
					//cout<<"Replacing "<<min_tup<<" with "<<counter.first<<"\n";

					replace(k_box.begin(), k_box.end(),min_tup,counter.first);
					min_tup = min_box_element();
				}
			}

		}
	}

	void fetch_tuples(string user_query)
	{
		hash_terms(user_query);
		create_box();

		//Fetching and displaying tuples
		ifstream file(data_file);
		string full_tuple;
		pair<unsigned int,int> seeking;
		for(auto tup : k_box)
		{
			if(file.is_open())
			{
					seeking = index_obj.tup_index[tup];
		    		file.seekg(seeking.first);
		    		full_tuple.resize(seeking.second);
		    		file.read(&full_tuple[0], seeking.second);

		    		//Attaching tuple-id to tuple for stat presentation

		    		full_tuple += "*&^" + to_string(tup);
			}

			k_box_tuples[full_tuple] = idf_calc[tup];

			//Displaying tuples,scores and their tupleno
			//cout<<tup<<"-> ("<<idf_calc[tup]<<") >>"<<full_tuple<<"\n\n";
		}

		disp_info();

	}

	int exclude_query_columns(string value)
	{
		stringstream linestream(value);
		string new_value;

		int freq = 0;
		int max_freq = 0;
		int max_freq_col = -1;
		while(getline(linestream,new_value,' '))
		{

		    for(auto e : index_obj.col_cell_index)
		    {

		        //cout<<"Trying to find = "<<new_value<<"\n";
		        if(e.second.find(new_value) != e.second.end())
		        {
		            freq = e.second[new_value].size();
		            //cout<<new_value<<"Found in col_cell_index @ COl no ="<< e.first<<"\n";
		        }

		        if(freq > max_freq)
		        {
		            max_freq = freq;
		            max_freq_col = e.first;
		        }
		        freq=0;
		    }
		}
    	return max_freq_col;
	}

	void disp_info()
	{
		#if 0
		cout<<"\n************************ Tupleno & IDF Scores ****************\n";
		for(auto d : idf_calc)	// Tuple_no VS Scores
			cout<<d.first<<"->"<<d.second<<"\n";
		cout<<"************************************************************\n";

		cout<<"\n********************** K_box : ******************************";
		for(auto a : k_box)			// K_box(Top n tuples)
			cout<<a<<"\t";
		cout<<"\n";
		#endif

		#if 0
		cout<<"\n******************** After - IDF ******************************\n";
		for(auto b : k_box_tuples)		//k_box tuples(Tuple VS Score)
			cout<<b.first<<" +>>> "<<b.second<<"\n\n";
		cout<<"*****************************************************************\n";
		#endif
	}
#if 1
	string disp_results()
	{
		string send_result = "";
		map<double,vector<string>> sorted_k_box_tuples;
		for(auto e : k_box_tuples)
		{		
			if(sorted_k_box_tuples.find(e.second) == sorted_k_box_tuples.end()) // new score
			{
				sorted_k_box_tuples[e.second] = vector<string>({e.first}); //new word - // make col and word
			}
			else // old score
			{
				sorted_k_box_tuples[e.second].push_back(e.first);
			}
		}
		for (map<double, vector<string>>::reverse_iterator iter = sorted_k_box_tuples.rbegin(); iter != sorted_k_box_tuples.rend(); ++iter)
		{
    		for( auto vec_iter = iter->second.cbegin() ; vec_iter != iter->second.cend() ; ++vec_iter )
				send_result += to_string(iter->first) + " +>>> " + *vec_iter + "\n\n";                
				//std::cout << *vec_iter << "  " ;
		}	
		return send_result;
	}
#endif
};
