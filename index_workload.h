#include "workload.h"
#include <ios>
using namespace std;

workload::workload(string rhs,Index index)
{
    queries_filename = rhs;
    ind_obj = index;
    workload_tid = 0; // a number given to the query
}

//Default ctor
workload::workload(){};

//Copy Assignment
workload& workload::operator=(const workload& rhs)
    {
        if(this != &rhs)
        {
            workload_tid = rhs.workload_tid;
            workload_col_index = rhs.workload_col_index;
            queries_filename = rhs.queries_filename;
            ind_obj = rhs.ind_obj;

        }
        return *this;
    }

int workload::get_data_file()
{
        ifstream input_file(queries_filename);
        string line,new_line;
        int col_num;
        //getline(input_file,line);
        while(getline(input_file,line))
        {
            workload_tid += 1;
            stringstream linestream(line);
            //getline(linestream,new_line,',');
            while(getline(linestream,new_line,','))
            {
                //new_line.erase(remove(new_line.begin(),new_line.end(),'"'),new_line.end());
                if(new_line != "")
                    col_num = workload::identify_column_insert_map(new_line, workload_tid);
            }
        }
		return workload_tid;
}
void workload::display_workload_column_index()
    {
        ofstream output_stream;
        output_stream.open("Indexes/workload_col_index.txt");
        for(auto key : workload_col_index) // key.first - col number
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
        output_stream.close();
    }

void workload::insert_in_map(int col, string word, unsigned int tid)
    {
        if(workload_col_index.find(col) == workload_col_index.end()) // new col
        {
            workload_col_index[col][word] = vector<unsigned int>({tid}); //new word - // make col and word
        }
        else // old col
        {
            if(workload_col_index[col].find(word) == workload_col_index[col].end()) // new word
            {
                workload_col_index[col][word] = vector<unsigned int>({tid});
            }
            else // old word
            {
                workload_col_index[col][word].push_back(tid);
            }
        }
    }

int workload::identify_column_insert_map(string value, int tid)
{
    stringstream linestream(value);
    string new_value;

    int freq = 0;
    int max_freq = 0;
    int max_freq_col = -1;
    while(getline(linestream,new_value,' '))
    {

        for(auto e : ind_obj.col_cell_index)
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
        workload::insert_in_map(max_freq_col,new_value,tid);
    }
    return max_freq_col;
}
