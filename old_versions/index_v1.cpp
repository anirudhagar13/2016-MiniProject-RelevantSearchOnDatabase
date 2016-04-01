#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <typeinfo>
#include <sstream>
using namespace std;

vector<string> &split(const string &s, char delim, vector<string> &elems) 
{
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

auto tokenize(const vector<string>& temp)
{
	vector<string> v;
	vector<string> b;
	for(auto a : temp)
	{
		b = split(a,' ');
		v.insert(v.end(), b.begin(), b.end());
	}
	return move(v);
}

void create_cell_index(long tid,string line,map<string,pair<int,vector<long>>>& mp)
{
	vector<string> col_index = split(line,',');
	vector<string> cell_index = tokenize(col_index);
	int i = 0;
	for(auto word : col_index)
	{

		if(mp.find(word) != mp.end())
			mp[word].second.push_back(tid);
		else
			mp[word].second = vector<long>({tid});

		mp[word].first = i;
		++i;
			
	}
}

long create_tup_index(long tid,long offset,int length,map<long,pair<long,int>>& mp)
{
	mp[tid] = make_pair(offset,length);
	return tid+1;
}

void disp1(const map<string, pair<int,vector<long>>>& mp)
{
	ofstream myfile;
  	myfile.open ("term_index.txt");

  	//Just replace myfile with cout to print on stdout

  	for(auto key : mp)
	{
		myfile<<key.first<<" : {"<<key.second.first<<"+  =>";
		for(auto vl : key.second.second)
			myfile<<vl<<",";
		myfile<<" }\n";
	}

	myfile.close();
}


void disp2(const map<long,pair<long,int>>& mp)
{
	ofstream myfile;
  	myfile.open ("tuple_index.txt");

  	//Just replace myfile with cout to print on stdout

	for(auto key : mp)
		myfile<<key.first<<" :{ "<<key.second.first<<" , "<<key.second.second<<" }\n";

	myfile.close();
}

int main() 
{
  string line;
  long cur_pos,tid = 1,lm = 1;
  map<long,pair<long,int>> tup_index;
  map<string,pair<int,vector<long>>> cell_index;
  ifstream myfile ("lamb.csv");

  while (lm)
   {
   		auto cur_pos = myfile.tellg();
   		if(!getline(myfile,line))
   			lm = 0;

   		create_cell_index(tid,line,cell_index);

   		if(cur_pos != -1)
    		tid = create_tup_index(tid,cur_pos,line.length(),tup_index);
   }

    disp1(cell_index);
    disp2(tup_index);

  myfile.close();
  return 0;
}

