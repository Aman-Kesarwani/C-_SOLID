// DIP: Dependency Inversion Principle
// High level modules should not depend on low level modules. Both should depend on absractions.
// Abstractions should not depend on details. Details should depend on abstractions.
#include <iostream>
#include <string>
#include <vector>
#include <tuple>

using namespace std;
enum class RelationshipType
{
	parent,
	child,
	sibling
};

struct Person
{
	string name;
};


struct RelationshipBrowser {
	virtual vector<Person> find_all_children_of(const string& name) = 0;
};

// low-level module- Relatioships provide fucntionality for data storage
struct Relationships: RelationshipBrowser {
	vector<tuple<Person, RelationshipType, Person>> relations;

	void add_parent_and_child(const Person& parent, const Person& child) {
		relations.push_back({ parent, RelationshipType::parent, child });
		relations.push_back({ child, RelationshipType::child, parent });
	}

	vector<Person>  find_all_children_of(const string& name) override {
		vector<Person> result;
		for (auto&& [first, rel, second] : relations) {
		
			if (first.name == name && rel == RelationshipType::parent) {
			
				result.push_back(second);
			}
		}
		return result;
	}
}; 

// High-level module - Research is going to be made in Relationship data
struct Research {

	Research(RelationshipBrowser& browser)
	{
		for (auto& child : browser.find_all_children_of("Dharmendra")) {
			cout << "Dharmendra has a child called "<< child.name << endl;
		}
	}
	// worst thing to do here is to take direct dependecny of low level module
	// if in future the relationship storage comoponent changes, we need to incorporate those changes here also which is absolutely absurd
	/*Research(Relationships& relationship) {
	
		auto& relations = relationship.relations;
		for (auto&& [first, rel, second] : relations) {
		
			if (first.name == "John" && rel = Relationship::parent) {
			
				cout << "John has a child called" << second.name << endl;
			}
		}
	}*/


};



int main()
{
	Person parent{ "Dharmendra" };
	Person child1{ "Bobby" }, child2{ "Sunny" };

	Relationships relationships;
	relationships.add_parent_and_child(parent, child1);
	relationships.add_parent_and_child(parent, child2);

	Research relation1(relationships);

    return 0;
}

