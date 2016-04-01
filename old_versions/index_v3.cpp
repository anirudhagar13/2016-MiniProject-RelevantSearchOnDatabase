#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
using namespace std;

typedef map<string,vector<unsigned int>> inner_map;
typedef map<int,inner_map> outer_map;

class Index
{
	public:

	outer_map col_index;
	map<string,vector<unsigned int>> cell_index;
	map<unsigned int, pair<unsigned int,int>> tup_index;
	Index(string file_name)
	{
		unsigned int cur_pos,tid = 1;
		ifstream input_file(file_name);
		string line;

		while(getline(input_file,line))
		{
			//Checking Initial condition if file has any content
			cur_pos = input_file.tellg();

			create_column_index(line,tid);
			create_cell_index(line,tid);

			if(cur_pos != -1)
				create_tuple_index(cur_pos,line.length(),tid);
			tid++;
		}

		input_file.close();
	}

	void display_tuple_index()
	{
		ofstream output_file;
	  	output_file.open ("Indexes/tuple_index.txt");

	  	//Just replace output_file with cout to print on stdout

		for(auto key : tup_index)
			output_file<<key.first<<" :-{ "<<key.second.first<<" , "<<key.second.second<<" }\n";

		output_file.close();
	}

	void display_cell_index()
	{
		ofstream output_stream;
		output_stream.open("Indexes/cell_index.txt");
		for(auto key : cell_index)
		{
			output_stream<<key.first<<" :->{";
			for(auto vl : key.second)
				output_stream<<vl<<",";
			output_stream<<" }\n";
		}

	}
	void display_column_index()
	{
		ofstream output_stream;
		output_stream.open("Indexes/col_index.txt");
		for(auto key : col_index) // key.first - col number
		{
			output_stream <<"Column NO - "<< key.first << " :\n\t";
			for(auto innerkey : key.second)
			{
				output_stream << innerkey.first << " in { "; // displays value of the column
				for(auto vl : innerkey.second)
					output_stream<<vl<<",";
				output_stream << " }";
				output_stream << "\n\t";
			}
			output_stream << "\n";
		}
	}

	void insert_in_map(int col, string word, unsigned int tid)
	{
		if(col_index.find(col) == col_index.end()) // new col
		{
			col_index[col][word] = vector<unsigned int>({tid}); //new word - // make col and word
		}
		else // old col
		{
			if(col_index[col].find(word) == col_index[col].end()) // new word
			{
				col_index[col][word] = vector<unsigned int>({tid});
			}
			else // old word
			{
				col_index[col][word].push_back(tid);
			}
		}
	}

	void tokenize(vector<string>& v, const string& line, const string& delim)
	{
		string final;
		char prev;
		for(auto pres : line)
		{
			if(delim.find(pres) == string::npos)
				final += pres;
			else
			{
				if(final != "")
				{
					v.push_back(final);
					final = "";
				}

				if(prev == ',' && pres == ',')	//Checkiing for empty fields to correct column indexing
					v.push_back("_E_M_P_T_Y_");
			}
			prev = pres;
		}
	}

	void create_column_index(string line, unsigned int tid)
	{
		vector<string> col_data;
		tokenize(col_data,line,",");
		int col = 1;
		for(auto e : col_data)
		{
			insert_in_map(col,e,tid);
			col++;
		}
	}

	void create_cell_index(string line, unsigned int tid)
	{
		vector<string> cell_data;
		tokenize(cell_data,line," ,");

		//Adding new entry or if exists adding tuples to it
		for(auto word : cell_data)
		{
			if(cell_index.find(word) != cell_index.end())
				cell_index[word].push_back(tid);
			else
				cell_index[word] = vector<unsigned int>({tid});
		}
	}

	void create_tuple_index(unsigned int offset, int length, unsigned int tid)
	{
		tup_index[tid] = make_pair(offset,length);
	}
};