std::string line;
std::string full_input;

std::cout << "Enter multiple lines of text (enter an empty line to finish):\n";

// Keep reading lines until an empty line is entered
while (true) {
    std::getline(std::cin, line);  // Read a whole line

    // Check if the line is empty
    if (line.empty()) {
        break;  // Stop if the line is empty
    }

    // Add the line to the full input string
    full_input += line + "\n";
}