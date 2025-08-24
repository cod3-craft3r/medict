#include <string>
#include <vector>
#include <fstream>
#include "Term.hpp"
#include "json.hpp"

using json = nlohmann::json;

class Database {
private:
    std::vector<Term> terms;
    std::string filePath; // Path to the JSON file

public:
    // Method to load data from file (JSON/CSV)
    void loadData();

    // Method to save data to file
    void saveData();

    // Method to add a new term
    void addTerm(const Term& term);

    // Method to search for a term by name
    Term* searchTerm(const std::string& termName);

    // Method to list all terms
    void listTerms();

    // Method to update a term's learned status
    void updateLearnedStatus(const std::string& termName, bool status);
};