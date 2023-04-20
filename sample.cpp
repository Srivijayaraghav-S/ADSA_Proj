#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "cgicc/CgiDefs.h"
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

using namespace std;

struct Person
{
    string name;
    int age;
    string gender;
};

int main()
{
    string input_string;
    getline(cin, input_string);
    // Parse input_string and do some processing
    vector<Person> people;
    Person p1 = {"Alice", 25, "Female"};
    Person p2 = {"Bob", 30, "Male"};
    people.push_back(p1);
    people.push_back(p2);
    // Serialize the vector of structs as a JSON string
    string json = "[";
    for (int i = 0; i < people.size(); i++)
    {
        if (i > 0)
        {
            json += ",";
        }
        json += "{";
        json += "\"name\":\"" + people[i].name + "\",";
        json += "\"age\":" + to_string(people[i].age) + ",";
        json += "\"gender\":\"" + people[i].gender + "\"";
        json += "}";
    }
    json += "]";
    // Send the JSON string as a response
    cout << "Content-Type: application/json\n\n";
    cout << json << endl;
    return 0;
}
