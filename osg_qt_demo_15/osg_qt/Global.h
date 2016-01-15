#pragma once
#include <string>
using namespace std;
const string RESOURCESPATH = "../resources/";
const string MODELPATH = "../models/";
const string PATHPATH = "../paths";

struct MyCompRule{
	bool operator() (const string& l, const string& r) const{
		const char *lchar = l.c_str();
		const char *rchar = r.c_str();
		int i = 0, lval = 0, rval = 0;
		while(lchar[i] != '\0'){
			lval += lchar[i];
			i++;
		}

		i = 0;
		while(rchar[i] != '\0'){
			rval += rchar[i];
			i++;
		}
		if (lval > rval) return true;
		else return false;
    }
};