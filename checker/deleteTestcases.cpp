#include <iostream>
#include <fstream>
#include <filesystem>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;
namespace fs = filesystem;

int main() {
    ifstream fin("config.json");
    json config;
    fin >> config;
    fin.close();

    string testcase = config["cpp"]["delete_testcase"];
    string folderName = "testcase";

    if (testcase.empty()) {
        cerr << "Error: testcases name is empty";
        return 1;
    }

    
    fs::path dir = fs::current_path() / folderName;
    size_t deleted = 0;
    for (const auto &entry : fs::directory_iterator(dir)) {
        if (entry.is_regular_file()) {
            string filename = entry.path().filename().string();

            if (!filename.rfind(testcase, 0)) {
                try {
                    fs::remove(entry.path());
                    cout << "Deleted " << filename << endl;
                    ++deleted;
                } catch (const exception &e) {
                    cerr << "Failed to delete " << filename << ": " << e.what() << endl;
                }
            }
        }
    }

    cout << "Deleted " << deleted << " file(s)!" << endl;
    
    string orderName = "order.txt";
    fs::path orderPath = fs::current_path().parent_path() / orderName;
    if (!fs::exists(orderPath)) {
        cerr << "Error: File not found at " << orderPath << endl;
    }

    fs::path tempPath = orderPath.parent_path() / (orderPath.stem().string() + "_temp" + orderPath.extension().string());

    ifstream inFile(orderPath);
    ofstream outFile(tempPath);

    string line;
    bool success = false;

    if (inFile && outFile) {
        while (getline(inFile, line)) {
            if (line != testcase) outFile << line << endl;
            else success = true;
        }
    }
    
    // close to enable delete/renaming file
    inFile.close();
    outFile.close();

    if (success) {
        try {
            fs::remove(orderPath);
            fs::rename(tempPath, orderPath);
            cout << "Successfully deleted problem from order.txt!" << endl;
        } catch (const fs::filesystem_error& e) {
            cerr << "Filesystem Error: " << e.what() << endl;
        }
    } else {
        fs::remove(tempPath);
        cout << "There's no " << testcase << " to be deleted!" << endl;
    }

}