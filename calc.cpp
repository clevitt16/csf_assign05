#include "calc.h"
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>


using std::string;
using std::cout;
using std::endl;
using std::map;
using std::vector;
using std::stringstream;


struct Calc {
private:
    map<string, int> variables;


public:
    // public member functions
    Calc();
    ~Calc();

    int evalExpr(const std::string &expr, int &result) {  // on error, just return 0!
		vector<string> tokens = tokenize(expr);
		int size = tokens.size();
		if (size == 1) {    // length 1: could be operand or error (Casey)
			string tok = tokens[1];
			if (isInt(tok)) {
				result = std::stoi(tok);
				return 1;
			} // check map for value of variable
			map<string, int>::iterator it = variables.find(tok);
			if (it == variables.end()) {
				return 0;
			}
			result = variables[tok];
			return 1;
			// also check for quit - jk, actually handled in calcInteractive!
		} else if (size == 3) {  // (Trisha)
			// write eval_operation function

		} else if (size == 5) {  // (Trisha)

		} else {
			return 0;
		}

	}

private:
    // private member functions
	vector<string> tokenize(const string &expr) {
    	vector<string> vec;
    	stringstream s(expr);
    	string tok;
    	while (s >> tok) {
        	vec.push_back(tok);
    	}
    	return vec;
	}

	int isOp (const string &expr) {
		char c = string[0];
		switch(c) {
			case '+':
			case '-': 
			case '*':
			case '/':
				return c;
				break;
			default:
				return 0;
		}
		return 0;
	}

	
	bool isInt (const string &expr) {
		stringstream ss;
		ss << expr;
		int num;
		ss >> num;
		if (ss.fail()) {
			return false;
		}
		return false;
	}


	bool isVarName (const string &expr) {
		for (string::iterator it = expr.begin(); it != expr.end(); it++) {
			char c = *it;
			if (!(c >= 'A' && c <= 'Z') && !(c >= 'a' && c <= 'z')) {
				return false;
			}
		}
		return true;
	}
	
};





// for printing: cout << s.length() << endl


extern "C" struct Calc *calc_create(void) {
    return new Calc();
}

extern "C" void calc_destroy(struct Calc *calc) {
    delete calc;
}

extern "C" int calc_eval(struct Calc *calc, const char *expr, int *result) {
    return calc->evalExpr(expr, *result);
}