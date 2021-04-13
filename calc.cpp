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
    int evalExpr(const std::string &expr, int &result) {  // on error, just return 0!
		vector<string> tokens = tokenize(expr);
		int size = tokens.size();
		bool success = false; //used to track whether operation pefromed successfully
		if (size == 1) {    // length 1: could be operand or error 
			int value = getInt(tokens[0], success); 
			if (success) { //valid number
			    result = value; 
			    return 1; 
			}

			 // check map for value of variable
			map<string, int>::iterator it = variables.find(tokens[0]);
			if (it != variables.end()) { //variable in the map
			    result = variables[tokens[0]]; //get the variable from the map 
			    return 1;
			} 
			
			return 0; //not a number, not a variable in the map
			
		} else if (size == 3) {  // Can be an operation (with numbers or variables) or an assignment
			int value =  performOp(tokens[0], tokens[1], tokens[2], success); 
			if (!success) {
			    return 0; 
			} 
			result = value; 
			return 1; 
		} else if (size == 5) {  // Assignment to an operation
			
			//must be an assignment operation
			if (tokens[1] != "=") { return 0; }
			
			 //cannot have two assignment operations (a = 5 = 3)
			if (tokens[3] == "=") {  return 0; }
			
			//assignment must happen with a variable
			if (!isVarName(tokens[0])) { return 0; }
			
			int value = performOp(tokens[2], tokens[3], tokens[4], success);
			
			if (!success) { return 0; }
			variables[tokens[0]] = value; 
			result = value; 
			return 1; 
		} 
		
	    return 0; 	

	};

private:
	vector<string> tokenize(const string &expr) {
	   vector<string> vec;
	   stringstream s(expr);
	   string tok;
	   while (s >> tok) {
        	vec.push_back(tok);
	   }
	   return vec;
	};

	
	int getInt (const string &expr, bool &success) {
		stringstream ss;
		ss << expr;
		int num;
		ss >> num;
		if (ss.fail()) {
		    success = false; 	
		} else {
		    success = true; 
		}
		return num;
	};


	bool isVarName (const string &expr) {
		for (string::const_iterator it = expr.begin(); it != expr.end(); it++) {
			char c = *it;
			if (!(c >= 'A' && c <= 'Z') && !(c >= 'a' && c <= 'z')) { //variable name must only be letters
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

		
		if (op.length() > 1) { success = false; return 0; } //check if valid operator
		char c = op[0]; 
		
		success = true; 
		switch(c) {
		    case '+': 
			return left + right; 
		    case '-': 
			return left - right; 
		    case '*': 
			return left * right; 
		    case '/': 
			if (right == 0) { //operation fails if div by 0 
			    success = false; 
			    return 0; 
			}
			return left / right; 
		    default:	//op is not a valid operator 
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
	    numVal = getInt(numStr, success); 
	    if (success) { //valid number
		return numVal; 
	    }
	    if (isVarName(numStr)) { //get number from map
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


extern "C" struct Calc *calc_create(void) {
    return new Calc();
}

extern "C" void calc_destroy(struct Calc *calc) {
    delete calc;
}

extern "C" int calc_eval(struct Calc *calc, const char *expr, int *result) {
    return calc->evalExpr(expr, *result);
}

