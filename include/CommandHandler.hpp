#include <string>
#include "Database.hpp"
#include "util.hpp"
#include "Term.hpp"
#include "SpacedRepetition.hpp"

class CommandHandler {
private:
    // db ptr?
    std::unique_ptr<Database> db;
    std::atomic<bool>* runningFlag;

public:
    // Constructor
    CommandHandler() = default;
    ~CommandHandler() = default;
    CommandHandler(const CommandHandler&) = delete;  // Prevent copying
    CommandHandler& operator=(const CommandHandler&) = delete;
    CommandHandler(CommandHandler&&) noexcept = default;  // Allow moving
    CommandHandler& operator=(CommandHandler&&) noexcept = default;

    void set_database(std::unique_ptr<Database> database);
    
    // Methods to handle commands
    void list_terms_in_know_base();
    bool add_term_to_know_base();
    void find_term();
    void handleLearnedStatusCommand(const std::string& name, bool status);
    
    // Spaced Repetition review commands
    void showDueTerms();           // Show terms due for review
    void reviewTerm();              // Interactive review session
    void updateTermReview(const std::string& termName, int quality);  // Update after review

    // void setRunningFlag(std::atomic<bool>* flag) {
    //     runningFlag = flag;
    // }
};