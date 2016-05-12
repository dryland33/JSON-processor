/**************************************************************************************
Project: JSON implementation
Programmer: Rylan Wassem
Date: 11.12.14
Description: JSON parser.
**************************************************************************************/

#ifndef JSON_VALUE_H
#define JSON_VALUE_H

#include <string>
#include <iostream>
#include <algorithm> //remove_if
using namespace std;


class json_value{

private:
	//the type of value (string, number, object, array, true, false, and null)
	string type;
	json_value* key;
	json_value* parent;
	
	json_value* right_sibling;
	int start_index;
	int end_index;
	int length;
	bool is_key;

public:
	json_value* left_child;

	//stores a string of JSON that is shared by all values
	static string data;

	//tracks if previously parsed object is a sibling of
	//the current object
	static bool prev_is_sibling;
	static void set_prev_is_sib(bool prev);

	//makes an empty value
	json_value();

	//makes a value from a string
	json_value(string value);

	//returns a string representation of the value
	string get_data();

	//sets the data member to the specified string
	static void set_data(string& datum);

	//sets parent pointer to point to a value's key
	void set_parent(json_value&);

	//sets child pointer to point to a key's first value
	void set_child(json_value&);

	//sets sibling pointer to point to 
	//the next value in a compound value
	void set_sibling(json_value&);

	//sets the value's type
	void set_type(string&);

	//sets start index of value
	void set_start(int&);

	//sets end index of value
	void set_end(int&);

	//sets the length of the object
	void set_length();

	//is set to true if the value is a key
	void set_key(bool);

	//returns a key
	string get_key();

	//returns true if value is a key
	bool is_a_key();

	//returns the length of the object
	int get_length();

	//returns the start index
	int get_start();

	//returns the end index
	int get_end();

	//value parsing functions:
	json_value& parseVal(int&);
	json_value& parseStr(int&);
	json_value& parseNum(int&);
	json_value& parseObj(int&);
	json_value& parseAry(int&);
	json_value& parseFls(int&);
	json_value& parseTru(int&);
	json_value& parseNul(int&);

	//auxillary parsing functions
	//void parseKey(int&);
};

#endif // !define JSON_VALUE_H


