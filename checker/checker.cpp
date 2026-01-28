#include <iostream>
#include <set>
#include <vector>
#include <stack>
#include <map>
#include <queue>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <windows.h>
#include <psapi.h>
#include <fstream>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;

enum RunResult { OK, TLE, MLE, HANG, COMPILE_ERR, UNKNOWN };

RunResult runWithTimeout(const string &exe, const string &inFile, const string &tmpFile, int timeoutMs, size_t memoryLimitMB = 512) {
    // Open input and output files
    HANDLE hIn  = CreateFileA(inFile.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hIn == INVALID_HANDLE_VALUE) {
        cerr << "Failed to open input file\n";
        return UNKNOWN;
    }

    HANDLE hOut = INVALID_HANDLE_VALUE;
    for (int attempt = 0; attempt < 5 && hOut == INVALID_HANDLE_VALUE; ++attempt) {
        hOut = CreateFileA(tmpFile.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hOut == INVALID_HANDLE_VALUE) Sleep(50); // retry after a short delay
    }

    if (hOut == INVALID_HANDLE_VALUE) {
        cerr << "Failed to open tmp file after retries: " << GetLastError() << "\n";
        return UNKNOWN;
    }

    // Make handles inheritable
    SetHandleInformation(hIn, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
    SetHandleInformation(hOut, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);

    // Setup process startup info with redirected stdin/stdout
    STARTUPINFOA si = { sizeof(si) };
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = hIn;
    si.hStdOutput = hOut;
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    PROCESS_INFORMATION pi;
    string cmd = "\"" + exe + "\"";

    // Starts a process
    if (!CreateProcessA(
            NULL,
            const_cast<char*>(cmd.c_str()), // full cmd line
            NULL, NULL, TRUE,
            CREATE_NO_WINDOW, NULL, NULL,
            &si, &pi)) {
        cerr << "Failed to start process\n";
        CloseHandle(hIn);
        CloseHandle(hOut);
        return COMPILE_ERR;
    }

    DWORD waitResult;
    RunResult result = OK;
    SIZE_T memMB;
    auto start = chrono::high_resolution_clock::now();

    // Poll every 100 ms to detect resource overuse
    while ((waitResult = WaitForSingleObject(pi.hProcess, 100)) == WAIT_TIMEOUT) {
        // Memory Limit Exceeded
        PROCESS_MEMORY_COUNTERS_EX memInfo;
        if (GetProcessMemoryInfo(pi.hProcess, (PROCESS_MEMORY_COUNTERS*)&memInfo, sizeof(memInfo))) {
            memMB = memInfo.WorkingSetSize / (1024 * 1024);
            if (memMB > memoryLimitMB) {
                result = MLE;
                TerminateProcess(pi.hProcess, 1);
                break;
            }
        }

        // Time Limit Exceeded
        auto now = chrono::high_resolution_clock::now();
        if (chrono::duration_cast<chrono::milliseconds>(now - start).count() > timeoutMs) {
            result = TLE;
            TerminateProcess(pi.hProcess, 1);
            break;
        }
    }

    // If process finished normally
    if (waitResult != WAIT_TIMEOUT && result == OK) result = OK;

    // Collect diagnostic info
    FILETIME createTime, exitTime, kernelTime, userTime;
    GetProcessTimes(pi.hProcess, &createTime, &exitTime, &kernelTime, &userTime);

    ULARGE_INTEGER uKernel, uUser;
    uKernel.LowPart = kernelTime.dwLowDateTime;
    uKernel.HighPart = kernelTime.dwHighDateTime;
    uUser.LowPart = userTime.dwLowDateTime;
    uUser.HighPart = userTime.dwHighDateTime;
    double cpuSeconds = (uKernel.QuadPart + uUser.QuadPart) / 1e7;

    PROCESS_MEMORY_COUNTERS_EX memInfo;
    GetProcessMemoryInfo(pi.hProcess, (PROCESS_MEMORY_COUNTERS*)&memInfo, sizeof(memInfo));
    SIZE_T peakMB = memInfo.PeakWorkingSetSize / (1024 * 1024);

    // Determine likely cause if timeout but low CPU
    if (result == TLE && cpuSeconds < (timeoutMs / 1000.0) * 0.3) {
        result = HANG;
    }

    TerminateProcess(pi.hProcess, 1);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hIn);
    CloseHandle(hOut);

    if (result != OK) {
        cout << fixed;
        cout.precision(3);
        cout << "CPU Time: " << cpuSeconds << " s, Peak Memory: " << peakMB << " MB\n";
    
        switch (result) {
            case TLE:  cout << "Time Limit Exceeded (CPU busy). "; break;
            case HANG: cout << "Possibly deadlock or waiting for input. "; break;
            case MLE:  cout << "Memory Limit Exceeded (" << memMB << " / " << memoryLimitMB << " MB). "; break;
            default:   cout << "Unknown termination reason. "; break;
        }

        cout << "Skipping...\n\n";
    }

    return result;
}

vector<string> readOutput(const string &fileName) {
    ifstream fin(fileName);
    vector<string> result;
    string word;
    while (fin >> word) result.push_back(word);
    return result;
}

bool answerCheck(vector<string> &expected, vector<string> &answer) {
    bool status = true;
    if (expected.size() != answer.size()) {
        cout << "Size Mismatch!: " << answer.size() << " (Expected: " << expected.size() << ")\n";
        return false;
    }

    size_t count = 0, limit = 30;
    for (size_t i = 0; i < expected.size(); ++i) {
        if (expected[i] != answer[i]) {
            if (count < limit) cout << "Mismatch!: " << answer[i] << " (Expected: " << expected[i] << ")\n";
            ++count;
            status = false;
        }
    }
    if (count > limit) {
        cout << "And other " << count - limit << " mismatches..." << endl;
    }
    
    if (status) cout << "Answer are correct!\n";

    return status;
}

void readyToDelete(string name) {
    // Edit config.json
    ifstream fin("config.json");
    json config;
    fin >> config;
    fin.close();

    config["cpp"]["delete_testcase"] = name;
    
    ofstream fout("config.json");
    fout << config.dump(2);
    fout.close();

    cout << name << " testcases are ready to be deleted!";

    // Edit order.txt
    fs::path filePath("../order.txt");
    ifstream in(filePath);

    vector<string> lines;
    string line;
    while (getline(in, line)) {
        lines.push_back(line);
    }
    in.close();

    auto getTargetName = [](const fs::path& p) -> std::string {
        fs::path pathObj(p);

        if (pathObj.has_extension()) {
            // It's a file → use parent folder
            return pathObj.parent_path().filename().string();
        } else {
            // It's a folder
            return pathObj.filename().string();
        }
    };

    string target = getTargetName("../Q4/BSTFurthestDistance/main.cpp");
    
    lines.erase(
        remove_if(lines.begin(), lines.end(), [&](const string& s) {
            return s.find(target) != string::npos;
        }),
        lines.end()
    );

    // Write back
    ofstream out(filePath);
    for (size_t i = 0; i < lines.size(); ++i) {
        out << lines[i];
        if (i + 1 != lines.size()) out << "\n"; // only add newline between lines
    }
}

void testCases(string name, int caseStart, int caseEnd, const string &exePath, int timeoutMs) {
    
    int scores = 0;
    int notFound = 0;
    for (int i = caseStart; i <= caseEnd; i++) {
        cout << "Test Case: " << i << endl;
        string dir = name + "." + to_string(i) + ".";
        
        string inFile  = "testcase/"  + dir + "in";
        string outFile = "testcase/" + dir + "sol";
        string tmpFile = "temp/temp_output.txt";
        
        ifstream fout(outFile), fin(inFile);
        if (!fout.good() || !fin.good()) {
            cout << "Test Case not found. Skipping... " << (!fin.good() ? "(input)\n\n" : "(output)\n\n");
            ++notFound;
            continue;
        }

        auto start = chrono::high_resolution_clock::now();
        bool ok = (runWithTimeout(exePath, inFile, tmpFile, timeoutMs) == OK);
        auto end = chrono::high_resolution_clock::now();
        
        if (!ok) continue; 
        
        vector<string> expected = readOutput(outFile);
        vector<string> answer = readOutput(tmpFile);
        if (answerCheck(expected, answer)) scores++;
        
        chrono::duration<double> duration = end - start;
        cout << "Execution time: " << duration.count() << " s\n\n";
    }
    cout << "Testcase Passed: " << scores << "/" << caseEnd - caseStart + 1 - notFound << endl;
    if (scores == caseEnd - caseStart + 1 - notFound) readyToDelete(name);
}

bool compileSolution(const string &problemCpp, const string &exeName) {
    string cmd = "g++ \"" + problemCpp   + "\" -O2 -o \"" + exeName + "\"";

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    if (!CreateProcessA(
            NULL,
            const_cast<char*>(cmd.c_str()),
            NULL, NULL, FALSE,
            CREATE_NO_WINDOW, NULL, NULL,
            &si, &pi)) {
        cerr << "Failed to start compiler process\n";
        return false;
    }

    DWORD result = WaitForSingleObject(pi.hProcess, 10000); // 10 sec timeout

    if (result == WAIT_TIMEOUT) {
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        cerr << "Compilation timed out!\n";
        return false;
    }

    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    if (exitCode != 0) {
        cerr << "Compilation failed!\n";
        return false;
    }

    return true;
}

int main() {
    ifstream file("config.json");
    json config;
    file >> config;

    string problemCpp = config["cpp"]["path"];
    string test_case = config["cpp"]["test_case"];

    string problemExe = "problem.exe";
    if (!compileSolution(problemCpp, problemExe)) return 1;

    int timeoutMs = 1000;
    testCases(test_case, 1, 20, problemExe, timeoutMs);
}