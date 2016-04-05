#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <math.h>
#include <algorithm>
#include <ctype.h>
using namespace std;
int main()
{
	/*string s = "lol";
	istringstream iss(s);

	int num = 0;

	if (!(iss >> num).fail()) {
    cout << num << std::endl;
	}
	else {
    cerr << "There was a problem converting the string to an integer!" << std::endl;
	}*/

	map<int,int> mp;
	mp[1] = 23;
	if(mp[1])
		cout<<"yes\n";
	if(mp[0])
		cout<<"no\n";
}
