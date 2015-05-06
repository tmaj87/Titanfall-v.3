#pragma once
#include <fstream>

using namespace std;


class dbg
{
private:
	ofstream handle;
	char *name;
public:
	dbg()
	{
		name = "__debug.txt";
	}
	void cleanFile()
	{
		handle.open(name, fstream::trunc);
		handle.close();
	}
	void toFile(const char* str)
	{
		handle.open(name, fstream::app);
		handle << str << endl;
		handle.close();
	}
};