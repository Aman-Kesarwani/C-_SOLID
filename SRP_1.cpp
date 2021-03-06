// SRP_1.cpp : Defines the entry point for the console application.
//** Single Responsibility Principle:
// A class should have a single responsibility. There should not be more than one reason for a class to change.

#include "stdafx.h"
#include <string>
#include <fstream>
#include <vector>
#include <boost/lexical_cast.hpp>

using namespace std;
struct Journal
{
	string journalTitle;
	vector <string> journalEntries;

	explicit Journal(const string& title) :journalTitle(title) {}

	void add(const string& entry);
	 
	// persistence is the separate concern (we donot need here)
	void save(const string& entry);
};

void Journal::add(const string& entry) {
	static int count = 1;
	journalEntries.push_back(boost::lexical_cast<string>(count++)+": "+entry);
}

void Journal::save(const string& filename) {
	ofstream ofs(filename);
	for(auto& s: journalEntries){
		ofs << s << endl;
	}
}

struct JournalPersistentManager {
	static void save(const Journal& j, const string& filename) {
		ofstream ofs(filename);
		for (auto& s : j.journalEntries) {
			ofs << s << endl;
		}
	}
};
int main()
{
	Journal journal{ "Diary 2018" };
	journal.add("I will learn machine learning");
	journal.add("I will qualify GRE");
	journal.add("I will apply for masters");


	//journal.save("diary2018.txt");

	// Now concerns are separated. Journal is maintained by Journal class and Journal Persistence is maintained by 
	// JournalPersistentManager
	JournalPersistentManager persistentManager;
	persistentManager.save(journal, "diary.txt");

    return 0;
}

