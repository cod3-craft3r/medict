#include "Database.hpp"

// possibly make this method return bool, to indicate success/failure
void Database::loadData() {
    const std::filesystem::path dataPath = getDataDir();
    std::cout << "Which Knowledge Space would you like to load?" << "\n";

    for (auto const& dirEntry : std::filesystem::directory_iterator{dataPath})
        std::cout << dirEntry.path() << "\n";
    std::cout << "***********************************************************\n\n";


    filePath = dataPath.c_str();
    filePath += "/test.json";
    std::ifstream i(filePath);

    try {
        if (i.is_open()) {
            i >> knowledgeSpace;
        }
        termsCount = knowledgeSpace["terms"].size();
        std::cout << "Loaded " << termsCount << " terms from " << filePath << "\n";
        i.close();
    } catch (const std::exception& e) {
        std::cout << "Failed to load JSON from: " << filePath << std::endl;
        throw std::runtime_error("Error loading JSON data: " + std::string(e.what()));
    }
}

void Database::list_all_terms()
{
    std::cout << knowledgeSpace["terms"].dump(4) << std::endl;
}

bool Database::add_term(Term& newTerm)
{
    try {
        json termJson;
        termJson["termName"] = newTerm.termName;
        termJson["definition"] = newTerm.definition;
        termJson["category"] = newTerm.category;
        termJson["learnedStatus"] = newTerm.learnedStatus;
        termJson["creationDate"] = tm_to_str(newTerm.creationDate);
        termJson["lastReviewDate"] = tm_to_str(newTerm.lastReviewDate);
        termJson["notes"] = newTerm.notes;

        std::cout << termJson.dump(4) << std::endl;
        knowledgeSpace["terms"].push_back(termJson);
        termsCount++;
        // std::cout << "********************" << "\n";
        // std::cout << knowledgeSpace["terms"].dump(4) << std::endl;
        std::ofstream output_file(filePath);
        output_file << std::setw(4) << knowledgeSpace << std::endl;
        output_file.close();
        std::cout << "Successfully added term to knowledge base.\n";
    } catch (const std::exception& e) {
        std::cout << "Failed to add term to knowledge base: " << e.what() << std::endl;
        return false;
    }
    return true;
}