#include "idf_ranking.h"
using namespace std;
#ifndef WORKLOAD_H
#define WORKLOAD_H

typedef map<string,vector<unsigned int>> inner_map;
typedef map<int,inner_map> outer_map;

class workload
{
    public:
    int workload_tid; // this is the query entered.
    outer_map workload_col_index;
    string queries_filename;
    Index ind_obj;

    workload();
    workload(string,Index);
    workload& operator=(const workload& rhs);
    void display_workload_column_index();
    int get_data_file();
    void insert_in_map(int , string , unsigned int );
    int identify_column_insert_map(string, int);
};
#endif
