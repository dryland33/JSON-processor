#include "value.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

void indent(int);
void pretty_print(string&);
void makeObject(string&, string&, string&);

int main(int argc, char* argv[]){

	//string arg1 = "firstName";//argv[1];
	//string arg2 = "lastName";//argv[2];
	
	string arg1 = argv[1];
	string arg2 = argv[2];

	string json;

	for (string line; getline(cin, line);) {
	json += line;
	}

	const char* url = R"(curl http://bl.ocks.org/mbostock/raw/4063269/flare.json)";
		
	FILE* output;
	output = _popen(url, "r");

	fstream fileStream(output);
	string line;
	while (getline(fileStream, line)){
		json += line;
	}
	_pclose(output);

	json_value jv(json);

	/*json_value jv( R"( { "employees":[
	{"firstName":"John", "lastName" : "Doe"},
	{ "firstName":"Anna", "lastName" : "Smith" },
	{ "firstName":"Peter", "lastName" : "Jones" }
	] } )" );*/

	/*string raw_j = R"( { "employees":[
	{"firstName":"John", "lastName" : "Doe"},
	{ "firstName":"Anna", "lastName" : "Smith" },
	{ "firstName":"Peter", "lastName" : "Jones" }
	] } )";*/

	//pretty_print(raw_j);

	
	//pretty_print(jv.get_data());

	int index(0);
	json_value* jo = &jv.parseVal(index);

	string new_object = R"( {"my_json_object":[ )";
	string pair1, pair2;
	string cur_object;

	while (jo->left_child){

		//if the current value is a key
		if (jo->is_a_key()){

			//if the key matches one of our arguments
			if (jo->get_key() == arg1){
				//gets called against previous values for pair1 and pair2
				makeObject(pair1, pair2, cur_object);
				pair1 = jo->get_data() + ":" + jo->left_child->get_data();
				new_object += cur_object;
			}
			if (jo->get_key() == arg2){
				if (pair2 == "")
				pair2 += jo->get_data() + " : " + jo->left_child->get_data();
				else
			    pair2 += "," + jo->get_data() + " : " + jo->left_child->get_data();
			}

		}
		jo = jo->left_child;
	}
	makeObject(pair1, pair2, cur_object);
	new_object += cur_object;
	new_object += "]}";

	//json_value new_json(new_object);
	pretty_print(new_object);

	cin.get();
	return 0;
}

void indent(int idnt){
	for (int j(0); j < idnt; j++)
		cout << ' ';
}

void pretty_print(string& out_ject){

	bool in_string = false;
	int ind(0);
	int length(out_ject.length());

	for (int i(0); i < length; i++){

		if (out_ject[i] == '"')in_string = !in_string;

		switch (out_ject[i]){
		case '{':
			if (in_string){
				cout << out_ject[i];
				break;
			}
			cout << '{' << '\n';
			indent(ind += 2);
			break;
		case '[':
			if (in_string){
				cout << out_ject[i];
				break;
			}
			cout << '[' << '\n';
			indent(ind += 2);
			break;
		case '}':
			if (in_string){
				cout << out_ject[i];
				break;
			}
			cout << '\n';
			indent(ind -= 2);
			cout << '}';
			break;
		case ']':
			if (in_string){
				cout << out_ject[i];
				break;
			}
			cout << '\n';
			indent(ind -= 2);
			cout << ']';
			break;
		case ':':
			if (in_string){
				cout << out_ject[i];
				break;
			}
			cout << out_ject[i] << " ";
			break;
		case ',':
			if (in_string){
				cout << out_ject[i];
				break;
			}
			cout << out_ject[i] << '\n';
			indent(ind);
			break;
		case ' ':
			break;
		default:
			cout << out_ject[i];
		}
	}
	cout << endl << endl;
}

void makeObject(string& pair1, string& pair2, string& cur_object){

	if (pair1 == "" && pair2 == "") cur_object = "";

	else if (pair1 == "" || pair2 == ""){
		cur_object = "{" + pair1 + pair2 + "}";
	}
	else {
		cur_object = "{" + pair1 + "," + pair2 + "}";
	}
	pair1 = "";
	pair2 = "";
}