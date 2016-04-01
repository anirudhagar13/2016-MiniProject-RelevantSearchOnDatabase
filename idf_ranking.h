#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <math.h>
#include <algorithm>
#include "index_v3.h"
using namespace std;
#define k_box_size 3

class Idf_rank
{
	public:
	Index index_obj;
	map<unsigned int,double> idf_calc;
	unsigned int tot_tups;
	vector<unsigned int> k_box;

	Idf_rank(Index i,unsigned int no)
	{
		index_obj = i;
		tot_tups = no;
	}

	void hash_terms(string query)
	{
		vector<unsigned int> tups;
		vector<string> split_words;
		tokenize(split_words,query);

		for(auto ele : split_words)
		{
			tups = index_obj.cell_index[ele];
			calc_idf(tups,log(tot_tups/tups.size()));
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
		k_box.clear();
		double min = -1;
		unsigned int min_tup;
		for(auto counter : idf_calc)
		{
			
			if(k_box.size() < k_box_size)
				k_box.push_back(counter.first);

			else
			{
				min_tup = min_box_element();
				if(idf_calc[min_tup] < counter.second)
					replace(k_box.begin(), k_box.end(),min_tup,counter.first);
			}

		}
	}

	void display_tuples(string user_query)
	{
		hash_terms(user_query);
		create_box();

		for(auto d : idf_calc)	//Value
			cout<<d.first<<"->"<<d.second<<"\n";
		cout<<"*******************************RESULTS************************************\n";

		/*for(auto a : k_box)			//K_box
			cout<<a<<"\t";*/
		
		ifstream file("lamb2.csv");
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
			}
			cout<<tup<<">"<<full_tuple<<"\n\n";
		}
		

	}
	
};