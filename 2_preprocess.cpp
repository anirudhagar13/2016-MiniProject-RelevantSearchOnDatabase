#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<map>
using namespace std;
typedef map<string,vector<unsigned int>> inner_map;
typedef map<int,inner_map> outer_map;

void display_tuple_index(const map<unsigned int,pair<long,int>>& mp)
{
	ofstream output_file;
  	output_file.open ("tuple_index.txt");

  	//Just replace myfile with cout to print on stdout

	for(auto key : mp)
		output_file<<key.first<<" :{ "<<key.second.first<<" , "<<key.second.second<<" }\n";

	output_file.close();
}
void display_cell_index(const map<string,vector<unsigned int>>& mp)
{
	ofstream output_stream;
	output_stream.open("cell_index.txt");
	for(auto key : mp)
	{
		output_stream<<key.first<<" : {";
		for(auto vl : key.second)
			output_stream<<vl<<",";
		output_stream<<" }\n";
	}

}
void display_column_index(const outer_map& omp)
{
	ofstream output_stream;
	output_stream.open("col_index.txt");
	for(auto key : omp) // key.first - col number
	{
		output_stream << key.first << " :\t";
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

void insert_in_map(outer_map& outerMap, int col, string word, unsigned int tid)
{
	if(outerMap.find(col) == outerMap.end()) // new col
	{
		outerMap[col][word] = vector<unsigned int>({tid}); //new word - // make col and word
	}
	else // old col
	{
		if(outerMap[col].find(word) == outerMap[col].end()) // new word
		{
			outerMap[col][word] = vector<unsigned int>({tid});
		}
		else // old word
		{
			outerMap[col][word].push_back(tid);
		}
	}
}

vector<string>& tokenize(vector<string>& v, const string& line, const string& delim)
{
	//vector<string> words;
	size_t prev = 0,pos;
        while((pos = line.find_first_of(delim,prev)) != string::npos)
        {
            if(pos > prev)
		{
			v.push_back(line.substr(prev,pos-prev));
		}

            prev = pos+1;
        }
        if(prev < line.length())
	{
		v.push_back(line.substr(prev,string::npos));
	}

	return v;
}

void create_column_index(outer_map& omap, string line, unsigned int tid)
{
	vector<string> col_data;
	col_data = tokenize(col_data,line,",");
	int col = 1;
	for(auto e : col_data)
	{
		insert_in_map(omap,col,e,tid);
		col++;
	}
}
void create_cell_index(map<string,vector<unsigned int>>& mp, string line, unsigned int tid)
{
	vector<string> cell_data;
	cell_data = tokenize(cell_data,line," ,");
	for(auto e : cell_data)
	{
		if(mp.find(e) != mp.end())
			mp[e].push_back(tid);
		else
			mp[e] = vector<unsigned int>({tid});
	}
}
void create_tuple_index(map<unsigned int, pair<long,int>>& mp, long offset, int length, unsigned int tid)
{
	mp[tid] = make_pair(offset,length);
}
int main()
{
    	ifstream input_file("sample_input_file.csv");
    	string line;
	unsigned int tid = 1;
    	vector<string> col_data;
	vector<string> cell_data;
	outer_map col_index;
	map<string,vector<unsigned int>> cell_index; // maps the value to a vector containing the tuples in which that value occurs.
	map<unsigned int, pair<long,int>> tup_index;
	auto cur_pos = input_file.tellg();
	while(getline(input_file,line))
	{
		create_column_index(col_index,line,tid);
		create_cell_index(cell_index,line,tid);
		if(cur_pos != -1)
			create_tuple_index(tup_index,cur_pos,line.length(),tid);
		tid++;
		cur_pos = input_file.tellg();
	}
	display_cell_index(cell_index);
	display_column_index(col_index);
	display_tuple_index(tup_index);
	return 0;
}
