// My creative component is calling "do_all." it asks for two files, a dna THEN database file,
// then proceeds to do everything at once.
#include "ourvector.h"
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct Person{
  string name;
  ourvector<int> consecutives;
    // numbers of consecutive strands per person. 12 31 44 etc
};

void load_db(string dbfilename,
             ourvector<Person> &database,
             ourvector<ourvector<char> > &strands,
             ourvector<int> &consecutivesInDNA);
void parseStrandLine(string strandLine,
                     ourvector<ourvector<char> > &strands,
                     ourvector<int> &consecutivesInDNA);
  // parses first line of database
void parseRemainingLines(Person &person,
                         string lineFor2andBeyond);

void display(ourvector<Person> database,
             ourvector<char> DNABases,
             ourvector<ourvector<char> > strands,
             ourvector<int> consecutivesInDNA,
             bool processed);
void load_dna(string dnafilename,
              ourvector<char> &DNABases);
void process(ourvector<char> DNABases,
             ourvector<ourvector<char> > strands,
             ourvector<int> &consecutivesInDNA,
             bool &processed);
void search(ourvector<Person> database,
            ourvector<int> consecutivesInDNA);
void doAll();

void load_db(string dbfilename,
             ourvector<Person> &database,
             ourvector<ourvector<char> > &strands,
             ourvector<int> &consecutivesInDNA) {
  database.clear();
  ifstream inputFile;
  inputFile.open(dbfilename);
  if (inputFile.fail()) {
    cout << "Loading database...";
    cout << "\nError: unable to open "
         << "'" << dbfilename << "'" << endl;
    return;
  }
  string strandLine;
  getline(inputFile, strandLine);
  parseStrandLine(strandLine, strands, consecutivesInDNA);

  while (!inputFile.eof()) {
    string lineFor2andBeyond;
    Person person;
    getline(inputFile, lineFor2andBeyond);
    if (!lineFor2andBeyond.empty()) {
      parseRemainingLines(person, lineFor2andBeyond);
      database.push_back(person);
    }
  }
  cout << "Loading database..." << endl;
  inputFile.close();
}
void parseStrandLine(string strandLine,
                     ourvector<ourvector<char> > &strands,
                     ourvector<int> &consecutivesInDNA) {
  stringstream ss(strandLine);
  string tossName;
  getline(ss, tossName, ',');

    while (!ss.eof()) {
    string sequence;
    getline(ss, sequence, ',');
    ourvector<char> sequenceVector;  // [A G A T C]
    for (size_t i = 0; i < sequence.length(); i++) {
      sequenceVector.push_back(sequence[i]);
    }
    strands.push_back(sequenceVector);
    sequenceVector.clear();
    consecutivesInDNA.push_back(0);
  }
}

void parseRemainingLines(Person &person, string lineFor2andBeyond) {
  stringstream ss(lineFor2andBeyond);
  getline(ss, person.name, ',');
  while (!ss.eof()) {
    string getNumber;
    getline(ss, getNumber, ',');
    person.consecutives.push_back(stoi(getNumber));
  }
}

void load_dna(string dnafilename, ourvector<char> &DNABases) {
  DNABases.clear();
  ifstream inputFile;
  inputFile.open(dnafilename);
  if (inputFile.fail()) {
    cout << "Loading DNA...";
    cout << "\nError: unable to open "
         << "'" << dnafilename << "'" << endl;
    return;
  }
  char DNABase;
  while (inputFile.get(DNABase)) {
    DNABases.push_back(DNABase);
  }
  cout << "Loading DNA..." << endl;
}

void process(ourvector<char> DNABases,
             ourvector<ourvector<char> > strands,
             ourvector<int> &consecutivesInDNA,
             bool &processed) {
  cout << "Processing DNA..." << endl;
    for (int i = 0; i < strands.size(); i++) {
        // iterates through ourvector of ourvectors not their letters
    int consecutiveCounter = 0;
        // consecutive sequences of strand at strands[i]
    int j = 0;
        while (j < DNABases.size()) {
            // iterates through DNA file
            if (strands[i][0] == DNABases[j]) {
                // if the first letter in the strand (ourvector<char>)
        int k = j;
                // keeps track of current index in DNA file
        bool result = true;
                for (int m = 0; m < strands[i].size(); m++) {
                    // iterates through each letter of strand
                    if (k < DNABases.size() && strands[i][m] == DNABases[k]) {
            k++;
          } else {
            result = false;
            break;
          }
        }
        if (result) {
          consecutiveCounter++;
          j = k;
          if (consecutiveCounter > consecutivesInDNA[i]) {
            consecutivesInDNA[i] = consecutiveCounter;
          }
        } else {
          consecutiveCounter = 0;
          j++;
        }
      } else {
        j++;
      }
    }
  }
  processed = true;
}

void search(ourvector<Person> database, ourvector<int> consecutivesInDNA) {
  cout << "Searching database..." << endl;
  bool matchFound = true;
  for (int i = 0; i < database.size(); i++) {
    matchFound = true;
    for (int j = 0; j < consecutivesInDNA.size(); j++) {
      if (database[i].consecutives[j] != consecutivesInDNA[j]) {
        matchFound = false;
        break;
      }
    }
    if (matchFound) {
      cout << "Found in database! DNA matches: " << database[i].name << endl;
      break;
    }
  }
  if (!matchFound) {
    cout << "Not found in database." << endl;
  }
}

void display(ourvector<Person> database,
             ourvector<char> DNABases,
             ourvector<ourvector<char> > strands,
             ourvector<int> consecutivesInDNA,
             bool processed) {
  if (database.size() == 0) {
    cout << "No database loaded." << endl;
  } else {
    cout << "Database loaded: " << endl;
    for (auto per : database) {
      cout << per.name;
      for (int i = 0; i < per.consecutives.size(); i++) {
        cout << " " << per.consecutives[i];
      }
      cout << endl;
    }
  }

  if (DNABases.size() == 0) {
    cout << "No DNA loaded." << endl;
  } else {
    cout << "DNA loaded: " << endl;
    for (int j = 0; j < DNABases.size(); j++) {
      cout << DNABases[j];
    }
    cout << endl;
  }
  if (!processed) {
    cout << "No DNA has been processed." << endl;
  } else {
    cout << "DNA processed, STR counts:" << endl;
    for (int k = 0; k < strands.size(); k++) {
      for (int l = 0; l < strands[k].size(); l++) {
        cout << strands[k][l];
      }
      cout << ": " << consecutivesInDNA[k] << endl;
    }
  }
}

void doAll()
{
    
}

int main() {
  string choice = "start";
  ourvector<Person> database;
  ourvector<ourvector<char> > strands;
  ourvector<char> DNABases;  // holds DNAfile
  ourvector<int> consecutivesInDNA;
    //  keeps track of consecutive bases in DNAfile
  bool processed = false;
  bool result = false;
  bool result2 = false;
  bool result3 = false;

  cout << "Welcome to the DNA Profiling Application." << endl;
  while (choice != "#" && choice != "do_all") {
    cout << "Enter command or # to exit: ";
    cin >> choice;
    if (choice == "#") {
      exit(1);
    } else if (choice == "load_db") {
      string dbfilename;
      cin >> dbfilename;
      load_db(dbfilename, database, strands, consecutivesInDNA);
      result = true;
    } else if (choice == "display") {
      display(database, DNABases, strands, consecutivesInDNA, processed);
    } else if (choice == "load_dna") {
      string dnafilename;
      cin >> dnafilename;
      load_dna(dnafilename, DNABases);
      result3 = true;
    } else if (choice == "process") {
      if (result == true) {
          if (result3 == true) {
              process(DNABases, strands, consecutivesInDNA, processed);
              result2 = true;
          } else {
            cout << "No DNA loaded." << endl;
          }
      } else {
        cout << "No database loaded." << endl;
        result = false;
      }
    } else if (choice == "search") {
      if (result == true) {
        if (result3 == true) {
          if (result2 == false) {
            cout << "No DNA processed." << endl;
          } else {
            search(database, consecutivesInDNA);
          }
        } else {
          cout << "No DNA loaded." << endl;
        }
      } else {
        cout << "No database loaded." << endl;
      }
    } else if (choice == "do_all") {
        string dbfilename;
        cin >> dbfilename;
        string dnafilename;
        cin >> dnafilename;
        load_db(dbfilename, database, strands, consecutivesInDNA);
        load_dna(dnafilename, DNABases);
        process(DNABases, strands, consecutivesInDNA, processed);
        search(database, consecutivesInDNA);
        display(database, DNABases, strands, consecutivesInDNA, processed);
    } else if (choice != "#") {
      cout << "Please Try Again" << endl;
    }
      
  }
    return 0;
}
