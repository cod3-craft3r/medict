#include "Database.hpp"

// possibly make this method return bool, to indicate success/failure
void Database::loadData() {
    const std::filesystem::path dataPath = getDataDir();
    std::cout << "Which Knowledge Space would you like to load?" << "\n";

    for (auto const& dirEntry : std::filesystem::directory_iterator{dataPath})
        std::cout << dirEntry.path() << "\n";
    std::cout << "***********************************************************\n";


    std::string filePath = dataPath.c_str();
    filePath += "/test.json";
    std::ifstream i(filePath);

    try {
        i >> knowledgeSpace;
        std::cout << "Loaded " << knowledgeSpace.size() << " terms from " << filePath << "\n";
    } catch (const std::exception& e) {
        std::cout << "Failed to load JSON from: " << filePath << std::endl;
        throw std::runtime_error("Error loading JSON data: " + std::string(e.what()));
    }
}

void Database::list_all_terms()
{
    std::cout << knowledgeSpace.dump(4) << std::endl;
}