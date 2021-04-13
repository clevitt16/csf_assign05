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
//    Calc();
//    ~Calc(); 

    int evalExpr(const std::string &expr, int &result) {  // on error, just return 0!
		vector<string> tokens = tokenize(expr);
		int size = tokens.size();
		bool success = false; 
		if (size == 1) {    // length 1: could be operand or error (Casey)
			//string tok = tokens[1];
			if (isInt(tokens[0])) {
				result = std::stoi(tokens[0]);
				return 1;
			} // check map for value of variable
			map<string, int>::iterator it = variables.find(tokens[0]);
			if (it == variables.end()) {
				return 0;
			}
			result = variables[tokens[0]];
			return 1;
			// also check for quit - jk, actually handled in calcInteractive!
		} else if (size == 3) {  // (Trisha)
			int value =  performOp(tokens[0], tokens[1], tokens[2], success); 
			if (!success) {
			    return 0; 
			} 
			result = value; 
			return 1; 
		} else if (size == 5) {  // (Trisha)
			int value = performOp(tokens[2], tokens[3], tokens[4], success);
			if (!success) {
			    return 0; 
			}
			if (tokens[1] != "=") {
			    return 0; 
			}
			
			if (!isVarName(tokens[0])) {
			    return 0;
			}
			variables[tokens[0]] = value; 
			result = value; 
			return 1; 
		} 
		
	    return 0; 	

	};

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
	};

	
	bool isInt (const string &expr) {
		stringstream ss;
		ss << expr;
		int num;
		ss >> num;
		if (ss.fail()) {
			return false;
		}
		return true;
	};


	bool isVarName (const string &expr) {
		for (string::const_iterator it = expr.begin(); it != expr.end(); it++) {
			char c = *it;
			if (!(c >= 'A' && c <= 'Z') && !(c >= 'a' && c <= 'z')) {
				return false;
			}
		}
		return true;
	};

	int performOp (const string &num1, const string &op, const string &num2, bool &success) {
		int left, right;
	
		right = getNumForOp(num2, success);
		if (!success) { return 0; }
		
		if (op == "=") {
		    return performAssign(num1, right, success); 	
		}
		left = getNumForOp(num1, success); 
		if (!success) { return 0; } 

		success = true; 
		char c = op[0]; 
		switch(c) {
		    case '+': 
			return left + right; 
		    case '-': 
			return left - right; 
		    case '*': 
			return left * right; 
		    case '/': 
			if (right == 0) { 
			    success = false; 
			    return 0; 
			}
			return left / right; 
		    default: 
			success = false; 
		}

		return 0; 

	};

	int performAssign(const string &var, int value, bool &success) {

	    if (!isVarName(var)) {
		success = false; 
		return 0;
            }

	    success = true; 
            variables[var] = value;
            return value; 
	
	}
	int getNumForOp (const string &numStr, bool &success) {
	    int numVal; 
	    if (isInt(numStr)) {
                    stringstream ss;
                    ss << numStr;
                    ss >> numVal;
		    success = true; 
		    return numVal;
	    }
	    if (isVarName(numStr)) {
		map<string, int>::iterator it = variables.find(numStr);
                if (it != variables.end()) {
		    success = true; 
		    return variables[numStr]; 
		}
	    }

	    success = false; 
	    return 0; 
	};
	

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

