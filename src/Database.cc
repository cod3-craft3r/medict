#include "Database.hpp"
#include "Render.hpp"

// possibly make this method return bool, to indicate success/failure
void Database::loadData() {
    const std::filesystem::path dataPath = getDataDir();
    std::cout << "Which Knowledge Space would you like to load?" << "\n";

    for (auto const& dirEntry : std::filesystem::directory_iterator{dataPath})
        std::cout << dirEntry.path() << "\n";
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";

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
        termJson["creationDate"] = newTerm.creationDate;
        termJson["lastReviewDate"] = newTerm.lastReviewDate;
        termJson["notes"] = newTerm.notes;
        
        // Spaced Repetition fields
        termJson["easinessFactor"] = newTerm.easinessFactor;
        termJson["interval"] = newTerm.interval;
        termJson["repetitionCount"] = newTerm.repetitionCount;
        termJson["nextReviewDate"] = newTerm.nextReviewDate;
        termJson["reviewCount"] = newTerm.reviewCount;
        termJson["correctRatio"] = newTerm.correctRatio;

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

std::vector<Term *> Database::search_term(const std::string& name)
{
    std::vector<Term *> results;
    for (auto& termJson : knowledgeSpace["terms"]) {
        std::string termName = termJson["termName"].get<std::string>();
        if (get_lower(termName).find(name) != std::string::npos) {
            Term* foundTerm = new Term();
            foundTerm->termName = termJson["termName"];
            foundTerm->definition = termJson["definition"];
            foundTerm->category = termJson["category"];
            foundTerm->learnedStatus = termJson["learnedStatus"];
            foundTerm->creationDate = termJson["creationDate"];
            foundTerm->lastReviewDate = termJson["lastReviewDate"];
            foundTerm->notes = termJson["notes"];
            
            // Spaced Repetition fields
            // foundTerm->easinessFactor = termJson.value("easinessFactor", 2.5);
            // foundTerm->interval = termJson.value("interval", 0);
            // foundTerm->repetitionCount = termJson.value("repetitionCount", 0);
            // foundTerm->nextReviewDate = termJson.value("nextReviewDate", "");
            // foundTerm->reviewCount = termJson.value("reviewCount", 0);
            // foundTerm->correctRatio = termJson.value("correctRatio", 0.0);

            if ( termJson["images"].is_array() ) {
                json img_list = termJson["images"];
                if ( img_list.size() > 0 ) {
                    std::cout << "Do you want to load the image associated w/ this term? (yes/no): ";
                    std::string response;
                    std::getline(std::cin, response);
                    if (get_lower(response) == "yes") {
                        int cnt = 1;
                        for ( auto& img_path : img_list ) {
                            std::string name = std::format("{} {}", foundTerm->termName, cnt++);
                            std::string imagePath = img_path.get<std::string>();
                            // std::thread renderThread([imagePath, name]() {
                                Render(imagePath, name);
                            // });
                            // renderThread.detach();
                        }
                    }
                }
            }

            // std::cout << termJson.dump(4) << std::endl;
            // return foundTerm;
            results.push_back(foundTerm);
        }
    }

    return results;
}

void Database::updateTerm(const Term& term)
{
    try {
        for (auto& termJson : knowledgeSpace["terms"]) {
            if (termJson["termName"].get<std::string>() == term.termName) {
                // Update all fields
                termJson["definition"] = term.definition;
                termJson["category"] = term.category;
                termJson["learnedStatus"] = term.learnedStatus;
                termJson["creationDate"] = term.creationDate;
                termJson["lastReviewDate"] = term.lastReviewDate;
                termJson["notes"] = term.notes;
                
                // Update SR fields
                termJson["easinessFactor"] = term.easinessFactor;
                termJson["interval"] = term.interval;
                termJson["repetitionCount"] = term.repetitionCount;
                termJson["nextReviewDate"] = term.nextReviewDate;
                termJson["reviewCount"] = term.reviewCount;
                termJson["correctRatio"] = term.correctRatio;
                
                // Save to file
                std::ofstream output_file(filePath);
                output_file << std::setw(4) << knowledgeSpace << std::endl;
                output_file.close();
                
                return;
            }
        }
        std::cout << "Term not found in database.\n";
    } catch (const std::exception& e) {
        std::cout << "Failed to update term: " << e.what() << std::endl;
    }
}
