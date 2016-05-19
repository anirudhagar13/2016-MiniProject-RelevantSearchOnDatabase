#include "headers.h"
using namespace std;

typedef map<string,vector<unsigned int>> inner_map;
typedef map<int,inner_map> outer_map;

class Index
{
	public:

	outer_map col_index;
	outer_map col_cell_index;
	map<string,vector<unsigned int>> cell_index;
	map<unsigned int, pair<unsigned int,int>> tup_index;
	unsigned int total_size;

	//Default Ctor
	Index(){};

	//Parametrized ctor
	Index(string file_name)
	{
		unsigned int cur_pos,tid = 1;
		ifstream input_file(file_name);
		string line;
		cur_pos = 0;
		while(getline(input_file,line))
		{
			//Checking Initial condition if file has any content
			//cur_pos = input_file.tellg();

			create_column_index(line,tid);	
			create_cell_index(line,tid);
			create_column_cell_index(line,tid);

			if(cur_pos != -1)
				create_tuple_index(cur_pos,line.length(),tid);
			tid++;

			cur_pos = input_file.tellg();

		}

		total_size = tid - 1;
		input_file.close();
	}

	//Copy Assignment
	Index& operator=(const Index& rhs)
	{
		if(this != &rhs)
		{
			col_index = rhs.col_index;
			cell_index = rhs.cell_index;
			tup_index = rhs.tup_index;
			col_cell_index = rhs.col_cell_index;
		}
		return *this;
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

	void display_column_cell_index()
	{
		ofstream output_stream;
		output_stream.open("Indexes/col_cell_index.txt");
		for(auto key : col_cell_index) // key.first - col number
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

	void insert_in_map(int col, string word, unsigned int tid, outer_map& index)
	{
		if(index.find(col) == index.end()) // new col
		{
			index[col][word] = vector<unsigned int>({tid}); //new word - // make col and word
		}
		else // old col
		{
			if(index[col].find(word) == index[col].end()) // new word
			{
				index[col][word] = vector<unsigned int>({tid});
			}
			else // old word
			{
				index[col][word].push_back(tid);
			}
		}
	}
	public:
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
		v.push_back(final);
	}

    void create_column_cell_index(string line, unsigned int tid)
    {
        vector<string> col_data;
        tokenize(col_data,line,",");
        int col = 1;
        string new_value;
        for(auto e : col_data)
        {
            stringstream value(e);
            while(getline(value,new_value,' '))
                insert_in_map(col,new_value,tid,col_cell_index);

            col++;
        }
    }

	void create_column_index(string line, unsigned int tid)
	{
		vector<string> col_data;
		tokenize(col_data,line,",");
		int col = 1;
		for(auto e : col_data)
		{
			transform(e.begin(), e.end(), e.begin(), ::toupper);						//********************************
			insert_in_map(col,e,tid,col_index);
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

			transform(word.begin(), word.end(), word.begin(), ::toupper);						//********************************
			
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
