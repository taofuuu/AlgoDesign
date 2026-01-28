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

    cout << "Deleted " << deleted << " file(s)!";
}