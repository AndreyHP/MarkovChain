#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <random>

using std::string;
using std::unordered_map;

struct Chain {
    std::unordered_map<string, int> wordCount;
    int totalWords = 0;

    void getWords(const string& text) {
        string word;
        for (char ch : text) {
            if (ch == ' ') {
                if (!word.empty()) {
                    wordCount[word]++;
                    totalWords++;
                    word.clear();
                }
            } else {
                word.push_back(ch);
            }
        }
        // Last word
        if (!word.empty()) {
            wordCount[word]++;
            totalWords++;
        }
    }

    void serialize(const string& filename) {
        std::ofstream ofs(filename);
        for (const auto& pair : wordCount) {
            ofs << pair.first << " " << pair.second << "\n";
        }
        ofs << totalWords; // Save total words at the end
        ofs.close();
    }

    void deserialize(const string& filename) {
        std::ifstream ifs(filename);
        string word;
        int count;
        wordCount.clear();
        totalWords = 0;
        while (ifs >> word >> count) {
            wordCount[word] = count;
            totalWords += count;
        }
        ifs.close();
    }

    string generateText(int wordCountToGenerate) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, totalWords - 1);

        string generatedText;
        for (int i = 0; i < wordCountToGenerate; i++) {
            int r = distrib(gen);
            int cumulative = 0;
            for (const auto& pair : wordCount) {
                cumulative += pair.second;
                if (r < cumulative) {
                    generatedText += pair.first + " ";
                    break;
                }
            }
        }
        return generatedText;
    }

    void printMarkov() const {
        for (const auto& pair : wordCount) {
            string word = pair.first;
            int count = pair.second;
            int probability = (count * 100) / totalWords; // Calculate probability

            std::cout << "Word: " << word
                      << " || Count: " << count
                      << " || Probability: " << probability << "%" << std::endl;
        }
    }
};

string loadFileToString(const string& filePath) {
    std::ifstream file(filePath); // Open file
    if (!file) { // Check if the file opened successfully
        std::cerr << "Error opening file: " << filePath << std::endl;
        return ""; // Return empty string on error
    }

    std::stringstream buffer; // Create string stream buffer
    buffer << file.rdbuf(); // Read file content into buffer

    return buffer.str(); // Return the string
}


int main() {
    //string text = loadFileToString("text.txt");


    //Chain markovChain;
    //markovChain.getWords(text);

    // Print the initial Markov chain
    //markovChain.printMarkov();

    // Serialize the Markov chain to a file
    //markovChain.serialize("markov_chain.txt");

    // Create a new chain and load from the file
    Chain loadedChain;
    loadedChain.deserialize("markov_chain.txt");

    // Generate text based on the loaded chain
    string generatedText = loadedChain.generateText(100);
    std::cout << "Generated Text: " << generatedText << std::endl;

    return 0;
}
