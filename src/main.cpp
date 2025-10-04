#include <cstring>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <random>
#include <vector>
#include <iterator>

using std::string;
using std::unordered_map;
using std::vector;

struct Tokens {
    unordered_map<string, int> pairs;
    int totalpairs = 0;

    void getPairs(const string &text) {
        std::istringstream iss(text);
        vector<string> words((std::istream_iterator<string>(iss)), std::istream_iterator<string>());

        for (size_t i = 0; i < words.size() - 1; i++) {
            string pair = words[i] + " " + words[i + 1] + " " + words[i + 2];
            pairs[pair]++;
            totalpairs++;
        }
    }

    void printPairs() {
        int tprs = 0;
        for (const auto &prs : pairs) {
            string a = prs.first;
            tprs += prs.second;
            std::cout << "pairs: " << a << " (" << prs.second << ")" << std::endl;
        }
        std::cout << "total Pairs: " << tprs << std::endl;
    }

    void serialize(const string &filename) {
        std::ofstream ofs(filename);
        if (!ofs) {
            std::cerr << "Error opening file for writing: " << filename << std::endl;
            return;
        }
        for (const auto &pair : pairs) {
            ofs << pair.first << " " << pair.second << "\n";
        }
        ofs << "#TOTAL " << totalpairs << "\n";
        ofs.close();
    }

    void deserialize(const string &filename) {
        std::ifstream ifs(filename);
        if (!ifs) {
            std::cerr << "Error opening file for reading: " << filename << std::endl;
            return;
        }
        pairs.clear();
        totalpairs = 0;
        string line;
        while (std::getline(ifs, line)) {
            if (line.empty()) continue;
            if (line.substr(0, 6) == "#TOTAL") {
                std::istringstream iss(line);
                string dummy;
                iss >> dummy >> totalpairs;
                break;
            }
            // Find the last space to separate pair from count
            size_t last_space = line.rfind(' ');
            if (last_space == string::npos) {
                std::cerr << "Invalid line format: " << line << std::endl;
                continue;
            }
            string pair = line.substr(0, last_space);
            string count_str = line.substr(last_space + 1);
            try {
                int count = std::stoi(count_str);
                pairs[pair] = count;
                totalpairs += count;
            } catch (const std::exception &e) {
                std::cerr << "Error parsing count in line: " << line << std::endl;
            }
        }
        ifs.close();
    }

    string generateText(int wordCountToGenerate) {
        if (pairs.empty() || totalpairs == 0) {
            return "Error: No pairs available to generate text.";
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, totalpairs - 1);

        string generatedText;
        for (int i = 0; i < wordCountToGenerate; i++) {
            int r = distrib(gen);
            int cumulative = 0;
            for (const auto& pair : pairs) {
                cumulative += pair.second;
                if (r < cumulative) {
                    generatedText += pair.first + " ";
                    break;
                }
            }
        }
        return generatedText;
    }
};

string loadFileToString(const string& filePath) {
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    //string text = loadFileToString("text.txt");
    Tokens t;
    //t.getPairs(text);
    //t.serialize("pairs.txt");
    t.deserialize("pairs.txt");
    //t.printPairs(); // Debug: Print loaded pairs to verify
    string generatedText = t.generateText(20);
    std::cout << "Generated Text: " << generatedText << std::endl;
    return 0;
}
