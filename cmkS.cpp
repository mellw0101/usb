#include <cstring>
#include <iostream>
#include <thread>
#include <vector>
#include <string>                                          // for strcmp
#include <chrono>
// cmkS -o /home/mellw/Shell-0.1/o_files/Shell.o -f /usb/Shell-0.1/Shell.cpp -a -c -e -o c_cp.o -f /usb/Shell-0.1/c_cp.cpp -a -c -e -o /home/mellw/Shell-0.1/o_files/base_tools.o -f

// cmkS -o /home/mellw/Shell-0.1/o_files/Shell.o -f /usb/Shell-0.1/Shell.cpp -a -c -Wall -Werror -march=skylake -mtune=skylake -stdlib=libc++ -e \
-o /home/mellw/Shell-0.1/o_files/c_cp.o -f /usb/Shell-0.1/c_cp.cpp -a -c -e \
-o /home/mellw/Shell-0.1/o_files/base_tools.o -f /usb/Shell-0.1/base_tools.cpp -a -c -Wall -Werror -march=skylake -mtune=skylake -stdlib=libc++ -e

// Define a function for the threads
void processArg(const std::string& options, const std::string& files, const std::string& outputFile) {
    auto startTime = std::chrono::high_resolution_clock::now(); // Record the start time
    std::string cmd = "clang++ -o" + outputFile + " " + files + " " + options;
    std::cout << "Processing task: " << cmd << std::endl;
    std::system(cmd.c_str());
    auto endTime = std::chrono::high_resolution_clock::now(); // Record the end time
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    std::cout << cmd << duration << "\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [-o output] [-c config] arg1 arg2 arg3 ..." << std::endl;
        return 1;
    }


    std::vector<std::thread> threads;                       // Create a vector to store thread objects
    std::string files;
    std::string outputFile;
    std::string options;

    // Flags to track if -o or -c has been encountered
    bool f = false;
    bool o = false;
    bool e = false;
    bool a = false;

    // Iterate through command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-f") { f = true;                        // Set the flag to process output arguments
        } else if (arg == "f-") { f = false;     // Set the flag to process config arguments
        } else if (arg == "-o") { o = true;     // Set the flag to process config arguments
        } else if (arg == "-a") { a = true;                 // Set the flag to process config arguments
        } else if (arg == "-e") {
            threads.emplace_back(processArg, options, files, outputFile);
            files = "";
            outputFile = "";
            options = "";
        } else if (f) {
            if (arg == "-e") {
                f = false; // Reset the flag
            } else if (!files.empty()) {                    //Append each subsequent argument after -f to the fileArguments string
                files += " ";                               // Add space separator if not the first argument
            }
            files += arg;
        } else if (o) {
            outputFile += arg;
            o = false; // Reset the flag
        } else if (a) {
            if (arg == "-e") {
                f = false; // Reset the flag
            } else if (!files.empty()) {                    //Append each subsequent argument after -f to the fileArguments string
                files += " ";                               // Add space separator if not the first argument
            }
            options += arg;
            a = false; // Reset the flag
        } else {

        }
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
