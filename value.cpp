/**************************************************************************************
Project: JSON implementation
Programmer: Rylan Wassem
Date: 11.12.14
Description: JSON parser.
**************************************************************************************/

#include "value.hpp"

string json_value::data = "";
bool json_value::prev_is_sibling = false;

json_value::json_value() {
	this->parent = nullptr;
	this->left_child = nullptr;
	this->right_sibling = nullptr;
}

json_value::json_value(string value){
	set_data(value);
	this->parent = nullptr;
	this->left_child = nullptr;
	this->right_sibling = nullptr;
	this->start_index = 0;
	this->end_index = value.length();
	this->length = this->end_index - this->start_index;
}

string json_value::get_data(){
	return data.substr(this->get_start(), (this->length));
}

void json_value::set_data(string& json){
	//remove white space
	json.erase(remove_if(json.begin(), json.end(), ::isspace), json.end());
	data = json;
}

void json_value::set_prev_is_sib(bool val){
	prev_is_sibling = val;
}

void json_value::set_type(string& type){
	this->type = type;
}

void json_value::set_parent(json_value& parent){
	this->parent = &parent;
}

void json_value::set_child(json_value& child){
	this->left_child = &child;
}

void json_value::set_sibling(json_value& sibling){
	this->right_sibling = &sibling;
}

void json_value::set_start(int& start){
	this->start_index = start;
}

int json_value::get_start(){
	return this->start_index;
}

void json_value::set_end(int& end){
	this->end_index = end;
}

int json_value::get_end(){
	return this->end_index;
}

int json_value::get_length(){
	return this->length;
}

void json_value::set_length(){
	this->length = (this->end_index - this->start_index) + 1;
}

void json_value::set_key(bool is_key){
	this->is_key = is_key;
}

string json_value::get_key(){
	return data.substr((this->get_start() + 1), (this->length - 2));
}

bool json_value::is_a_key(){
	return this->is_key;
}

//call the appropriate parse function for current type
json_value& json_value::parseVal(int& index){

	int length(data.length());

	for (int i(index); i < length; i++){

		switch (data[i]){

		case '{':
			return parseObj(i);
		case '[':
			return parseAry(i);
		case '\"':
			return parseStr(i);
		case 't':
			return parseTru(i);
		case 'f':
			return parseFls(i);
		case 'n':
			return parseNul(i);
		case ':':
			++index;
			break;
		case ',':
			++index;
			set_prev_is_sib(true);
			break;
		case '}':
			++index;
			break;
		case']':
			++index;
			break;
		default:
			return parseNum(i);
		}
	}
	return *this;
}


json_value& json_value::parseObj(int& index){

	//make a pointer to a new object
	json_value* obj_ptr = new json_value;

	//set the parent pointer to point to
	//the thing that called parseObj()
	obj_ptr->set_parent(*this);

	//set the child or sybling pointer
	// of thething that called parsObj() 
	//to point to the new object
	this->set_child(*obj_ptr);

	//set the type of current value to "object"
	string type = "object";
	obj_ptr->set_type(type);

	//where the current object begins
	obj_ptr->set_start(index);

	//find the end of the object
	int num_open(0);

	for (int i(index); i < data.length(); i++){

		if (data[i] == '{') num_open++;

		if (data[i] == '}'){
			num_open--;
			if (num_open == 0){
				obj_ptr->set_end(i);
				obj_ptr->set_length();
				break;
			}
		}
	}

	//the object calls parseVal on each of its members
	//int j(obj_ptr->get_start());
	//while (j != obj_ptr->get_end()){
	obj_ptr->parseVal(++index);
	//}

	return *obj_ptr;
}

json_value& json_value::parseStr(int& index){
	//make a pointer to a new string
	json_value* str_ptr = new json_value;

	//set the parent pointer to point to
	//the thing that called parseObj()
	str_ptr->set_parent(*this);

	//set the child or sybling pointer
	//of the thing that called parsStr() 
	//to point to the new object
	this->set_child(*str_ptr);

	//set the type of the value 
	string type = "string";
	str_ptr->set_type(type);

	//set start_index
	str_ptr->set_start(index);

	//find the end of the string
	int i(index + 1);
	while (data[i] != '\"'){
		++i;
	}

	//set str_ptr's end index
	str_ptr->set_end(i);

	//check to see if this string is a key
	if (data[i + 1] == ':'){
		str_ptr->set_key(true);
	}
	else str_ptr->set_key(false);

	//set str_ptr's length
	str_ptr->set_length();

	index += str_ptr->get_length();

	//cout << str_ptr->get_key() << str_ptr->is_key << endl;
	
	//call parseVal on next value
	str_ptr->parseVal(index);
	return *str_ptr;
}

json_value& json_value::parseNum(int& index){
	//make a pointer to a new number
	json_value* num_ptr = new json_value;
	parseVal(++index);
	return *num_ptr;
}

json_value& json_value::parseAry(int& index){
	//make a pointer to a new array
	json_value* ary_ptr = new json_value;

	//set the parent pointer to point to
	//the thing that called parseObj()
	ary_ptr->set_parent(*this);

	//set the child or sybling pointer
	// of thething that called parsObj() 
	//to point to the new object
	this->set_child(*ary_ptr);

	//set the value type 
	string type = "array";
	ary_ptr->set_type(type);

	//where array begins
	ary_ptr->set_start(index);

	//find the end of the array
	int num_open(0);

	for (int i(index); i < data.length(); i++){

		if (data[i] == '[') num_open++;

		if (data[i] == ']'){
			num_open--;
			if (num_open == 0){
				ary_ptr->set_end(i);
				ary_ptr->set_length();
				break;
			}
		}
	}

	//the array calls parseVal on each of its members
	//int j(ary_ptr->get_start());
	//while (j != ary_ptr->get_end()){
	ary_ptr->parseVal(++index);
	//}
	return *ary_ptr;
}

json_value& json_value::parseFls(int& index){
	//make a pointer to a new false
	json_value* fls_ptr = new json_value;
	parseVal(++index);
	return *fls_ptr;
}

json_value& json_value::parseTru(int& index){
	//make a pointer to a new true
	json_value* tru_ptr = new json_value;
	parseVal(++index);
	return *tru_ptr;
}

json_value& json_value::parseNul(int& index){
	//make a pointer to a new null
	json_value* nul_ptr = new json_value;
	parseVal(++index);
	return *nul_ptr;
}

//void json_value::set_key(json_value& key){
//	this->key = &key;
//}

//void json_value::parseKey(int& index){
//    //set the key to the current key
//	set_key(parseStr(index));
//}
