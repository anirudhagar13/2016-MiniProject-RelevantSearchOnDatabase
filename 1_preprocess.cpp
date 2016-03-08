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

void create_cell_index(long tid,string line,map<string,vector<long>>& mp)
{
	vector<string> col_index = split(line,',');
	vector<string> cell_index = tokenize(col_index);
	for(auto word : cell_index)
	{

		if(mp.find(word) != mp.end())
			mp[word].push_back(tid);
		else
			mp[word] = vector<long>({tid});
			
	}
}

long create_tup_index(long tid,long offset,int length,map<long,pair<long,int>>& mp)
{
	mp[tid] = make_pair(offset,length);
	return tid+1;
}

void disp1(const map<string, vector<long>>& mp)
{
	ofstream myfile;
  	myfile.open ("term_index.txt");

  	//Just replace myfile with cout to print on stdout

  	for(auto key : mp)
	{
		myfile<<key.first<<" : {";
		for(auto vl : key.second)
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
  map<string,vector<long>> cell_index;
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

