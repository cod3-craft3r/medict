#include <string>
#include <vector>
#include <fstream>
#include "Term.hpp"
#include "util.hpp"
#include "json.hpp"

using json = nlohmann::json;

class Database {
private:
    // std::vector<Term> terms;
    std::string filePath;    // Path to the JSON file
    json knowledgeSpace;
    int termsCount = 0;     // to keep track of number of terms in the current knowledge space

public:

    // explicit Database(const std::string& filepath);

    Database() = default;
    ~Database() = default;
    Database(const Database&) = delete;  // Prevent copying
    Database& operator=(const Database&) = delete;
    Database(Database&&) noexcept = default;  // Allow moving
    Database& operator=(Database&&) noexcept = default;

    // Method to load data from file (JSON/CSV)
    void loadData();

    // Method to save data to file
    void saveData();

    // Method to add a new term
    bool add_term(Term& newTerm);

    // Method to search for a term by name
    Term* searchTerm(const std::string& termName);

    // Method to list all terms
    void list_all_terms();

    // Method to update a term's learned status
    void updateLearnedStatus(const std::string& termName, bool status);
};