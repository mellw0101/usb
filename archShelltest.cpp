#include <algorithm>                                        // for sorting
#include <array>
#include <atomic>
#include <cerrno>
#include <chrono>
#include <complex>
#include <condition_variable>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/misc.h>
#include <cryptopp/sha.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <curses.h>
#include <errno.h>
#include <ext2fs/ext2fs.h>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <functional>
#include <future>
#include <getopt.h>
#include <iomanip>
#include <iostream>
#include <iostream>                                         // Include the header for std::cout
#include <iterator>
#include <mutex>
#include <ncurses.h>
#include <numeric>
#include <openssl/sha.h>
#include <ostream>
#include <pthread.h>
#include <queue>
#include <readline/history.h>
#include <readline/keymaps.h>
#include <readline/readline.h>
#include <readline/rlstdc.h>
#include <readline/rltypedefs.h>
#include <readline/tilde.h>
#include <regex>
#include <sched.h>
#include <set>
#include <spawn.h>
#include <sstream>
#include <sstream>                                          // Include the header for std::ostringstream
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/mount.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include <vector>
#include <zlib.h>

// sudo g++ /usb/archShelltest.cpp -time -lz -std=c++20 -O2 -o /bin/Shell -lreadline -lncurses
// sudo mpicxx /usb/archShelltest.cpp -xHOST -fopenmp -xCORE-AVX2 -time -lz -std=c++20 -O3 -unroll -march=native -mtune=skylake -o /bin/Shell -lreadline -lncurses
// sudo clang++ -O3 -march=x86-64-v3 -o /bin/Shell -stdlib=libc++ -lz -lncurses -lreadline /usb/archShelltest.cpp


// g++ /usb/archShelltest.cpp -time -lz -std=c++20 -O2 -o test -lreadline
namespace SourceCodeLinks {
    // bash source code
    // git clone git://git.savannah.gnu.org/bash.git
}

namespace fs = std::filesystem;
namespace mellwscommands {
    namespace base_tools {
        void set_env_var() {
            const char* pathEnv = std::getenv("PATH");
            if (pathEnv != nullptr) {
                setenv("PATH", pathEnv, 1);
            }
            const char* displayEnv = std::getenv("DISPLAY");
            if (displayEnv != nullptr) {
                setenv("DISPLAY", displayEnv, 1);
            }
            const char* XAUTHORITYenv = std::getenv("XAUTHORITY");
            if (XAUTHORITYenv != nullptr) {
                setenv("XAUTHORITY", "/tmp/xauth_tcvJLb", 1);
            }
        }
        namespace get_vars {
            std::string get_PATH_var() {
                const char* env_var = std::getenv("PATH");
                if (env_var == nullptr) {
                    // Handle the case where PATH is not set or is empty
                    return "";
                }
                return std::string(env_var);
            }
            std::string get_XAUTHORITY_var() {
                const char* env_var = std::getenv("XAUTHORITY");
                if (env_var == nullptr) {
                    // Handle the case where PATH is not set or is empty
                    return "";
                }
                return std::string(env_var);
            }
            std::string get_USER_var() {
                const char* env_var = std::getenv("USER");
                if (env_var == nullptr) {
                    // Handle the case where PATH is not set or is empty
                    return "";
                }
                return std::string(env_var);
            }
            std::string get_DISPLAY_var() {
                const char* env_var = std::getenv("DISPLAY");
                if (env_var == nullptr) {
                    // Handle the case where PATH is not set or is empty
                    return "";
                }
                return std::string(env_var);
            }
            std::string get_HOME_var() {
                const char* env_var = std::getenv("HOME");
                if (env_var == nullptr) {
                    // Handle the case where PATH is not set or is empty
                    return "";
                }
                return std::string(env_var);
            }
            std::string get_TERM_var() {
                const char* env_var = std::getenv("TERM");
                if (env_var == nullptr) {
                    // Handle the case where PATH is not set or is empty
                    return "";
                }
                return std::string(env_var);
            }
            std::string get_COLORTERM_var() {
                const char* env_var = std::getenv("COLORTERM");
                if (env_var == nullptr) {
                    // Handle the case where PATH is not set or is empty
                    return "";
                }
                return std::string(env_var);
            }
            std::string get_XDG_CURRENT_DESKTOP_var() {
                const char* env_var = std::getenv("XDG_CURRENT_DESKTOP");
                if (env_var == nullptr) {
                    // Handle the case where PATH is not set or is empty
                    return "";
                }
                return std::string(env_var);
            }
            std::string get_DPCPP_HOME_var() {
                const char* env_var = std::getenv("DPCPP_HOME");
                if (env_var == nullptr) {
                    // Handle the case where PATH is not set or is empty
                    return "";
                }
                return std::string(env_var);
            }
            std::string get_DBUS_SESSION_BUS_ADDRESS_var() {
                const char* env_var = std::getenv("DBUS_SESSION_BUS_ADDRESS");
                if (env_var == nullptr) {
                    // Handle the case where PATH is not set or is empty
                    return "";
                }
                return std::string(env_var);
            }
            std::string get_XDG_CONFIG_DIRS_var() {
                const char* env_var = std::getenv("XDG_CONFIG_DIRS");
                if (env_var == nullptr) {
                    // Handle the case where PATH is not set or is empty
                    return "";
                }
                return std::string(env_var);
            }
            std::string get_XDG_SEAT_var() {
                const char* env_var = std::getenv("XDG_SEAT");
                if (env_var == nullptr) {
                    // Handle the case where PATH is not set or is empty
                    return "";
                }
                return std::string(env_var);
            }
            std::string get_XDG_SESSION_CLASS_var() {
                const char* env_var = std::getenv("XDG_SESSION_CLASS");
                if (env_var == nullptr) {
                    // Handle the case where PATH is not set or is empty
                    return "";
                }
                return std::string(env_var);
            }
            std::string get_XDG_RUNTIME_DIR_var() {
                const char* env_var = std::getenv("XDG_RUNTIME_DIR");
                if (env_var == nullptr) {
                    // Handle the case where PATH is not set or is empty
                    return "";
                }
                return std::string(env_var);
            }
        }
        std::vector<std::string> splitString(const std::string& input, char delimiter) {
            std::vector<std::string> tokens;
            size_t start = 0;
            size_t end = input.find(delimiter);
            while (end != std::string::npos) {
                tokens.push_back(input.substr(start, end - start));
                start = end + 1;
                end = input.find(delimiter, start);
            }
            tokens.push_back(input.substr(start, end));
            return tokens;
        }
        std::string search_in_PATH(const std::string& phrase) {
                std::string pathEnv = get_vars::get_PATH_var();

                std::vector<std::string> paths = splitString(pathEnv, ':');
                std::string fullPath;

                bool found = false;
                for (const std::string& path : paths) {
                    // Check if the phrase exists in the current path
                    fullPath = path + "/" + phrase;
                    if (access(fullPath.c_str(), F_OK) == 0) {
                        // std::cout << "Found in path: " << path << std::endl;
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    return "Command not found in any PATH directory.";
                }

                return fullPath;
        }
        void cddir(const std::string& full_PATH_to_dir) {   // move active directory to target
            if (fs::exists(full_PATH_to_dir) && fs::is_directory(full_PATH_to_dir)) {
                if (chdir(full_PATH_to_dir.c_str()) != 0) {
                    std::cerr << "cddir: Failed to change directory.\n";
                }
            } else if (fs::exists(full_PATH_to_dir)) {
                std::cerr <<  "cddir'" + full_PATH_to_dir + "' is a file.\n";
            } else {
                std::cerr <<  "cddir'" + full_PATH_to_dir + "' dosent exist.\n";
            }
        }
    }
    namespace memory_tools {
        bool clearCacheMemory() {
            std::ofstream dropCachesFile("/proc/sys/vm/drop_caches");
            if (!dropCachesFile.is_open()) {
                std::cerr << "Failed to open drop_caches file." << std::endl;
                return false;
            }
            dropCachesFile << "3";                          // Write "3" to the file to clear page cache, dentries, and inodes
            dropCachesFile.close();                         // Close the file
            std::cout << "Caches cleared successfully." << std::endl;
            return true;
        }
    }
    namespace benchmarks {
        namespace primes {
            void sieveOfEratosthenes(int n) {
                // Create a boolean array "isPrime[0..n]" and initialize all entries to true
                std::vector<bool> isPrime(n + 1, true);

                for (int p = 2; p * p <= n; p++) {
                    // If isPrime[p] is true, then it's a prime number
                    if (isPrime[p]) {
                        // Update all multiples of p
                        for (int i = p * p; i <= n; i += p) {
                            isPrime[i] = false;
                        }
                    }
                }

                // Print prime numbers
                for (int p = 2; p <= n; p++) {
                    if (isPrime[p]) {
                        std::cout << p << " ";
                    }
                }
            }
            int main(int n) {
                std::cout << "Prime numbers up to " << n << " are:" << std::endl;
                sieveOfEratosthenes(n);
                return 0;
            }
        }
    }
    namespace string_manipulation   {
        bool search_string(std::string& targetstring, const std::string& wordToFind) {
            // Create a regular expression pattern to match the whole word
            std::regex wordPattern("\\b" + wordToFind + "\\b");

            // Search for the whole word in the string
            std::smatch match;
            if (std::regex_search(targetstring, match, wordPattern)) {
                std::cout << "search_string: string '" << wordToFind << "' found at position " << match.position() << std::endl;
                return true;
            } else {
                std::cout << "search_string: Word '" << wordToFind << "' not found" << std::endl;
                return false;
            }
        }
        bool replace_string(std::string& targetstring, const std::string& wordToFind, const std::string& replacement) {
            // Create a regular expression pattern to match the whole word
            std::regex wordPattern("\\b" + wordToFind + "\\b");

            // Replace the word in the target string
            targetstring = std::regex_replace(targetstring, wordPattern, replacement);

            // Check if any replacement was made
            return targetstring != wordToFind; // Returns true if a replacement occurred
        }
    }
    namespace threds_tools {
        // simple function to get numCores
        int cpu_Cores(){
            unsigned int numCores = std::thread::hardware_concurrency();
            std::cout << "Number of CPU cores: " << numCores << std::endl;
            return numCores;
        }
        // Function to run Geekbench with a given program path
        int runGeekbench(const std::string& program) {
            char* const argv[] = {const_cast<char*>(program.c_str()), nullptr};
            char* const empty_env[] = {nullptr};

            pid_t pid;
            if (posix_spawn(&pid, program.c_str(), nullptr, nullptr, argv, empty_env) != 0) {
                return -1; // Error spawning process
            }

            int status;
            if (waitpid(pid, &status, 0) == -1) {
                return -2; // Error waiting for process
            }

            if (WIFEXITED(status)) {
                return WEXITSTATUS(status); // Return the exit status
            } else {
                return -3; // Process did not exit normally
            }
        }
        // Function to run Geekbench in parallel with all available threads
        void runGeekbenchInParallel(const std::string& program) {
            // Get the number of hardware threads available
            unsigned int numThreads = std::thread::hardware_concurrency();
            std::cout << "Number of CPU cores: " << numThreads << std::endl;

            std::vector<std::thread> threads;

            // Create and start threads to run Geekbench
            for (unsigned int i = 0; i < numThreads; ++i) {
                threads.push_back(std::thread([program]() {
                    int result = runGeekbench(program);
                    if (result == 0) {
                        std::cout << "Thread completed successfully." << std::endl;
                    } else {
                        std::cerr << "Thread failed with result: " << result << std::endl;
                    }
                }));
            }

            // Wait for all threads to finish
            for (auto& thread : threads) {
                thread.join();
            }

            std::cout << "All threads have completed running Geekbench." << std::endl;
        }
    }
    namespace exec_tools {
        bool runBinary(const std::string& binaryPath) {
            pid_t pid;
            char* const argv[] = { const_cast<char*>(binaryPath.c_str()), nullptr };
            posix_spawn_file_actions_t actions;
            posix_spawn_file_actions_init(&actions);
            posix_spawnattr_t attr;
            posix_spawnattr_init(&attr);

            int result = posix_spawn(&pid, binaryPath.c_str(), &actions, &attr, argv, nullptr);
            if (result == 0) {
                // Wait for the child process to finish
                int status;
                if (waitpid(pid, &status, 0) == -1) {
                    std::cerr << "Error waiting for the child process." << std::endl;
                    return false;
                }
                return true; // Execution succeeded
            } else {
                std::cerr << "Error spawning the child process." << std::endl;
                return false; // Execution failed
            }
        }
        int Binary_with_std_system(const std::string& binaryPath) {
            std::string command = binaryPath;                   // Construct the command to execute the binary
            int exitCode = std::system(command.c_str());        // Execute the binary using system() and capture the exit code
            return exitCode;
        }
        std::future<void> runBinaryAsync(const std::string& binaryPath) {
            return std::async(std::launch::async, [&binaryPath] {
                pid_t pid;
                char* const argv[] = { const_cast<char*>(binaryPath.c_str()), nullptr };

                if (posix_spawn(&pid, binaryPath.c_str(), nullptr, nullptr, argv, nullptr) != 0) {
                    // Handle the error, e.g., log an error message
                    std::cerr << "Error spawning child process." << std::endl;
                    return;
                }

                // Optionally, you can collect the exit status later
                int status;
                waitpid(pid, &status, 0);

                std::cout << "Child process completed." << std::endl;
            });
        }
        std::future<void> runBinaryAsync_no_error(const std::string& binaryPath) {
            return std::async(std::launch::async, [&binaryPath] {
                pid_t pid;
                char* const argv[] = { const_cast<char*>(binaryPath.c_str()), nullptr };

                if (posix_spawn(&pid, binaryPath.c_str(), nullptr, nullptr, argv, nullptr) == 0) {
                    // Wait for the child process to finish
                    int status;
                    waitpid(pid, &status, 0);
                }
            });
        }
        std::future<void> runBinaryAsync_no_error_max_prio(const std::string& binaryPath) {
            return std::async(std::launch::async, [&binaryPath] {
                pid_t pid;
                char* const argv[] = { const_cast<char*>(binaryPath.c_str()), nullptr };
                char* const envp[] = { nullptr };

                posix_spawn_file_actions_t actions;
                posix_spawnattr_t attr;
                posix_spawnattr_setschedpolicy(&attr, SCHED_FIFO);

                // Set CPU affinity to the maximum available CPU
                cpu_set_t cpuset;
                CPU_ZERO(&cpuset);
                CPU_SET(CPU_COUNT(&cpuset) - 1, &cpuset); // Set affinity to the last CPU core
                sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);

                // Set process priority to the maximum
                setpriority(PRIO_PROCESS, 0, -20); // Lower values indicate higher priority

                if (posix_spawn(&pid, binaryPath.c_str(), &actions, &attr, argv, envp) == 0) {
                    // Wait for the child process to finish
                    int status;
                    waitpid(pid, &status, 0);
                }
            });
        }
        std::future<void> runBinaryAsync_use_all_cores(const std::string& binaryPath) {
            return std::async(std::launch::async, [&binaryPath] {
                pid_t pid;
                char* const argv[] = { const_cast<char*>(binaryPath.c_str()), nullptr };
                char* const envp[] = { nullptr };

                posix_spawn_file_actions_t actions;
                posix_spawn_file_actions_init(&actions);
                posix_spawnattr_t attr;
                posix_spawnattr_init(&attr);

                // Get the number of available CPU cores
                int numCores = std::thread::hardware_concurrency();

                // Set CPU affinity for all available cores
                cpu_set_t cpuset;
                CPU_ZERO(&cpuset);
                for (int i = 0; i < numCores; ++i) {
                    CPU_SET(i, &cpuset);
                }
                sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);

                // Set process priority to the maximum
                setpriority(PRIO_PROCESS, 0, -20); // Lower values indicate higher priority

                if (posix_spawn(&pid, binaryPath.c_str(), &actions, &attr, argv, envp) == 0) {
                    // Wait for the child process to finish
                    int status;
                    waitpid(pid, &status, 0);
                }
            });
        }
    }
    namespace test {
        namespace find_PATH {
            std::vector<std::string> splitPath(const std::string& pathVar) {
                std::vector<std::string> paths;
                std::istringstream iss(pathVar);
                std::string path;
                while (std::getline(iss, path, ':')) {
                    paths.push_back(path);
                }
                return paths;
            }
            void removePrefixFromVector(std::vector<std::string>& vec, const std::string& prefix, const std::string& phrase) {
                size_t prefixLength = prefix.length();

                while (true) {
                    bool found = false;

                    for (auto it = vec.begin(); it != vec.end(); ++it) {
                        if (it->find(prefix + phrase) == 0) {
                            // Found a match, remove the prefix + phrase
                            it->erase(0, prefixLength + phrase.length());
                            found = true;
                        }
                    }

                    // If no more matches are found, exit the loop
                    if (!found) {
                        break;
                    }
                }
            }
            bool get_first_Path(const std::string& phrase) {
                std::string pathEnv = mellwscommands::base_tools::get_vars::get_PATH_var();

                std::vector<std::string> paths = mellwscommands::base_tools::splitString(pathEnv, ':');

                bool found = false;
                for (const std::string& path : paths) {
                    // Check if the phrase exists in the current path
                    std::string fullPath = path + "/" + phrase;
                    if (access(fullPath.c_str(), F_OK) == 0) {
                        std::cout << "Found in path: " << path << std::endl;
                        // You can remove the path from the search here
                        // Optionally, you can break the loop if you only want to find the first occurrence
                        // break;
                    }
                }

                if (!found) {
                    std::cout << "Phrase not found in any PATH directory." << std::endl;
                }

                return found;
            }
            void get_current_Path(const std::string& phrase) {
                char* pathEnv = std::getenv("PATH");
                if (pathEnv == nullptr) {
                    std::cerr << "PATH environment variable is not set." << std::endl;
                    return;
                }

                std::vector<std::string> paths = splitPath(pathEnv);

                for (const std::string& path : paths) {
                    // Check if the phrase exists in the current path
                    std::string fullPath = path + "/" + phrase;
                    if (access(fullPath.c_str(), F_OK) == 0) {
                        std::cout <<  fullPath << std::endl;
                    }
                }
            }
            void listOccurrencesInPath_old(const std::string& phrase) {
                char* pathEnv = std::getenv("PATH");
                if (pathEnv == nullptr) {
                    std::cerr << "PATH environment variable is not set." << std::endl;
                    return;
                }

                std::vector<std::string> paths = splitPath(pathEnv);
                std::set<std::string> foundPaths;

                for (const std::string& path : paths) {
                    // Check if the phrase exists in the current path
                    std::string fullPath = path + "/" + phrase;
                    if (access(fullPath.c_str(), F_OK) == 0) {
                        foundPaths.insert(path);
                    }
                }

                if (foundPaths.empty()) {
                    std::cout << "Phrase not found in any PATH directory." << std::endl;
                } else {
                    std::cout << "Found in the following PATH directories:" << std::endl;
                    for (const std::string& path : foundPaths) {
                        std::cout << path << std::endl;
                    }
                }
            }
            void listOccurrencesInPath(const std::string& phrase) {
                char* pathEnv = std::getenv("PATH");
                if (pathEnv == nullptr) {
                    std::cerr << "PATH environment variable is not set." << std::endl;
                    return;
                }

                std::vector<std::string> paths = splitPath(pathEnv);
                std::set<std::string> foundPaths;

                for (const std::string& path : paths) {
                    // Check if the phrase exists in the current path
                    std::string fullPath = path + "/" + phrase;
                    if (access(fullPath.c_str(), F_OK) == 0) {
                        foundPaths.insert(fullPath); // Store the full path, not just the directory
                    }
                }

                if (foundPaths.empty()) {
                    std::cout << "Phrase not found in any PATH directory." << std::endl;
                } else {
                    std::cout << "Found in the following PATH directories:" << std::endl;
                    for (const std::string& path : foundPaths) {
                        std::cout << path << std::endl;
                    }
                }
            }
            std::vector<std::string> findPaths(const std::string& phrase) {
                char* pathEnv = std::getenv("PATH");
                std::vector<std::string> foundPaths;

                if (pathEnv == nullptr) {
                    std::cerr << "PATH environment variable is not set." << std::endl;
                    return foundPaths;
                }

                std::vector<std::string> paths = splitPath(pathEnv);

                for (const std::string& path : paths) {
                    // Check if the phrase exists in the current path
                    std::string fullPath = path + "/" + phrase;
                    if (access(fullPath.c_str(), F_OK) == 0) {
                        foundPaths.push_back(fullPath);
                    }
                }

                return foundPaths;
            }
            void list(const std::string& phraseToFind) {
                std::vector<std::string> foundPaths = findPaths(phraseToFind);

                if (foundPaths.empty()) {
                    std::cout << "Phrase not found in any PATH directory." << std::endl;
                } else {
                    std::cout << "Found in the following paths:" << std::endl;
                    for (const std::string& path : foundPaths) {
                        std::cout << path << std::endl;
                    }
                }
                return;
            }
        }
        std::future<void> runBinaryAsync_with_env(const std::string& binaryPath) {
            return std::async(std::launch::async, [&binaryPath] {
                std::string XAUTHORITY = "XAUTHORITY=" + mellwscommands::base_tools::get_vars::get_XAUTHORITY_var();
                std::string PATH = "PATH=" + mellwscommands::base_tools::get_vars::get_PATH_var();
                std::string DISPLAY = "DISPLAY=" + mellwscommands::base_tools::get_vars::get_DISPLAY_var();
                std::string TERM = "TERM=" + mellwscommands::base_tools::get_vars::get_TERM_var();
                std::string COLORTERM = "COLORTERM=" + mellwscommands::base_tools::get_vars::get_COLORTERM_var();
                std::string HOME = "HOME=" + mellwscommands::base_tools::get_vars::get_HOME_var();
                std::string USER = "USER=" + mellwscommands::base_tools::get_vars::get_USER_var();
                std::string DBUS_SESSION_BUS_ADDRESS = "DBUS_SESSION_BUS_ADDRESS=" + mellwscommands::base_tools::get_vars::get_DBUS_SESSION_BUS_ADDRESS_var();
                std::string DPCPP_HOME = "DPCPP_HOME=" + mellwscommands::base_tools::get_vars::get_DPCPP_HOME_var();
                std::string XDG_CURRENT_DESKTOP = "XDG_CURRENT_DESKTOP=" + mellwscommands::base_tools::get_vars::get_XDG_CURRENT_DESKTOP_var();
                std::string XDG_SESSION_CLASS = "XDG_SESSION_CLASS=" + mellwscommands::base_tools::get_vars::get_XDG_SESSION_CLASS_var();
                std::string XDG_CONFIG_DIRS = "XDG_CONFIG_DIRS=" + mellwscommands::base_tools::get_vars::get_XDG_CONFIG_DIRS_var();
                std::string XDG_SEAT = "XDG_SEAT=" + mellwscommands::base_tools::get_vars::get_XDG_SEAT_var();
                std::string XDG_RUNTIME_DIR = "XDG_RUNTIME_DIR=" + mellwscommands::base_tools::get_vars::get_XDG_RUNTIME_DIR_var();
                // steam env vars
                std::string STEAM_RUNTIME = "STEAM_RUNTIME=debug";
                std::string RLIMIT_NICE_var = "RLIMIT_NICE=22";
                std::string STEAM_DEBUGGER = "STEAM_DEBUGGER=valgrind";


                pid_t pid;
                char* const argv[] = { const_cast<char*>(binaryPath.c_str()), nullptr };
                char* const envp[] = {
                    const_cast<char*>(STEAM_RUNTIME.c_str()),
                    const_cast<char*>("LD_LIBRARY_PATH=/usr/lib/steam:/usr/lib32/steam"),
                    const_cast<char*>(STEAM_DEBUGGER.c_str()),
                    const_cast<char*>(RLIMIT_NICE_var.c_str()),
                    const_cast<char*>(DBUS_SESSION_BUS_ADDRESS.c_str()),
                    const_cast<char*>(XDG_SEAT.c_str()),
                    const_cast<char*>(XDG_RUNTIME_DIR.c_str()),
                    const_cast<char*>(XDG_CURRENT_DESKTOP.c_str()),
                    const_cast<char*>(XDG_CONFIG_DIRS.c_str()),
                    const_cast<char*>(XDG_SESSION_CLASS.c_str()),
                    const_cast<char*>(XAUTHORITY.c_str()),
                    const_cast<char*>(USER.c_str()),
                    const_cast<char*>(DISPLAY.c_str()),
                    const_cast<char*>(HOME.c_str()),
                    nullptr
                };

                posix_spawnattr_t attr;
                posix_spawn_file_actions_t actions;

                posix_spawnattr_setschedpolicy(&attr, SCHED_FIFO);

                posix_spawn_file_actions_init(&actions);
                posix_spawnattr_init(&attr);


                int status;
                if (posix_spawn(&pid, binaryPath.c_str(), &actions, &attr, argv, envp) == 0) {
                    // Wait for the child process to finish
                    waitpid(pid, &status, 0);
                } else {
                    throw std::runtime_error("Error spawning the process: " + std::string(strerror(errno)));
                }
            });
        }
        std::future<void> runBinaryAsync_with_env_wargs(const std::string& binaryPath, const std::vector<std::string>& args) {
            return std::async(std::launch::async, [&binaryPath, &args] {
                std::string XAUTHORITY = "XAUTHORITY=" + mellwscommands::base_tools::get_vars::get_XAUTHORITY_var();
                std::string PATH = "PATH=" + mellwscommands::base_tools::get_vars::get_PATH_var();
                std::string DISPLAY = "DISPLAY=" + mellwscommands::base_tools::get_vars::get_DISPLAY_var();
                std::string TERM = "TERM=" + mellwscommands::base_tools::get_vars::get_TERM_var();
                std::string COLORTERM = "COLORTERM=" + mellwscommands::base_tools::get_vars::get_COLORTERM_var();
                std::string HOME = "HOME=" + mellwscommands::base_tools::get_vars::get_HOME_var();
                std::string USER = "USER=" + mellwscommands::base_tools::get_vars::get_USER_var();
                std::string DBUS_SESSION_BUS_ADDRESS = "DBUS_SESSION_BUS_ADDRESS=" + mellwscommands::base_tools::get_vars::get_DBUS_SESSION_BUS_ADDRESS_var();
                std::string DPCPP_HOME = "DPCPP_HOME=" + mellwscommands::base_tools::get_vars::get_DPCPP_HOME_var();
                std::string XDG_CURRENT_DESKTOP = "XDG_CURRENT_DESKTOP=" + mellwscommands::base_tools::get_vars::get_XDG_CURRENT_DESKTOP_var();
                std::string XDG_SESSION_CLASS = "XDG_SESSION_CLASS=" + mellwscommands::base_tools::get_vars::get_XDG_SESSION_CLASS_var();
                std::string XDG_CONFIG_DIRS = "XDG_CONFIG_DIRS=" + mellwscommands::base_tools::get_vars::get_XDG_CONFIG_DIRS_var();
                std::string XDG_SEAT = "XDG_SEAT=" + mellwscommands::base_tools::get_vars::get_XDG_SEAT_var();
                std::string XDG_RUNTIME_DIR = "XDG_RUNTIME_DIR=" + mellwscommands::base_tools::get_vars::get_XDG_RUNTIME_DIR_var();




                std::vector<char*> argv;
                for (const std::string& arg : args) {       // Build the command string
                    argv.push_back(const_cast<char*>(arg.c_str()));      // Add each argument to the argv vector
                }
                argv.push_back(nullptr);  // The last element must be nullptr

                pid_t pid;

                // Get the pointer to the argv vector
                char* const* argvPtr = argv.data();

                char* const envp[] = {
                    //const_cast<char*>("XDG_SESSION_PATH=/org/freedesktop/DisplayManager/Session0"),
                    //const_cast<char*>("XCURSOR_SIZE=24"),
                    //const_cast<char*>("XDG_SESSION_DESKTOP=KDE"),
                    //const_cast<char*>("XDG_SESSION_TYPE=x11"),
                    //const_cast<char*>("XDG_SEAT_PATH=/org/freedesktop/DisplayManager/Seat0"),
                    // const_cast<char*>("XCURSOR_THEME=breeze_cursors"),
                    // const_cast<char*>("XDG_SESSION_ID=1"),
                    const_cast<char*>(DPCPP_HOME.c_str()),
                    const_cast<char*>(DBUS_SESSION_BUS_ADDRESS.c_str()),
                    const_cast<char*>(COLORTERM.c_str()),
                    const_cast<char*>(TERM.c_str()),
                    const_cast<char*>(XDG_CONFIG_DIRS.c_str()),
                    const_cast<char*>(XDG_RUNTIME_DIR.c_str()),
                    const_cast<char*>(XDG_SEAT.c_str()),
                    const_cast<char*>(XDG_SESSION_CLASS.c_str()),
                    const_cast<char*>(XDG_CURRENT_DESKTOP.c_str()),
                    const_cast<char*>(XAUTHORITY.c_str()),
                    const_cast<char*>(USER.c_str()),
                    const_cast<char*>(DISPLAY.c_str()),
                    const_cast<char*>(HOME.c_str()),
                    const_cast<char*>(PATH.c_str()),
                    nullptr
                };

                static posix_spawn_file_actions_t actions;
                static posix_spawnattr_t attr;

                posix_spawn_file_actions_init(&actions);
                posix_spawnattr_init(&attr);


                int status;
                if (posix_spawn(&pid, binaryPath.c_str(), &actions, &attr, argvPtr, envp) == 0) {
                    // Wait for the child process to finish
                    waitpid(pid, &status, 0);
                } else {
                    throw std::runtime_error("Error spawning the process: " + std::string(strerror(errno)));
                }
            });
        }
        std::future<void> runBinaryAsync_with_parent_env_wargs(const std::string& binaryPath, const std::vector<std::string>& args) {
            return std::async(std::launch::async, [&binaryPath, &args] {

                std::vector<char*> argv;
                for (const std::string& arg : args) {       // Build the command string
                    argv.push_back(const_cast<char*>(arg.c_str()));      // Add each argument to the argv vector
                }
                argv.push_back(nullptr);  // The last element must be nullptr

                pid_t pid;

                // Get the pointer to the argv vector
                char* const* argvPtr = argv.data();

                char* const* envp = environ;

                static posix_spawn_file_actions_t actions;
                static posix_spawnattr_t attr;

                posix_spawn_file_actions_init(&actions);
                posix_spawnattr_init(&attr);


                int status;
                if (posix_spawn(&pid, binaryPath.c_str(), &actions, &attr, argvPtr, envp) == 0) {
                    // Wait for the child process to finish
                    waitpid(pid, &status, 0);
                } else {
                    throw std::runtime_error("Error spawning the process: " + std::string(strerror(errno)));
                }
            });
        }
    }
    void error_message(const std::string& program, const std::string& message) {
        std::cerr << program + ": ERROR: " << message << " (press enter to continue)";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    void check_active_directory() {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("Current working directory: %s\n", cwd);
        } else {
            perror("getcwd");
        }
    }
    namespace getCurrentDirectory_tools {
        std::string shortenPath(const std::string& rootDir, const std::vector<std::string>& lastComponents) {
            std::string result = rootDir;  // Initialize with the root directory

            // Append the last 5 components to the result
            for (const std::string& component : lastComponents) {
                result += "/" + component;
            }

            return result;
        }
        std::string getCurrentDirectory_test() {
            std::string result = "";
            fs::path currentPath = fs::current_path();

            // Get the components of the current path
            std::vector<std::string> pathComponents;
            for (const auto& component : currentPath) {
                pathComponents.push_back(component.string());
            }

            // Determine how many components to return (at most 6)
            size_t numComponents = std::min(pathComponents.size(), static_cast<size_t>(6));

            if (numComponents <= 6) {
                result = currentPath.string();
                return result;
            } else if (numComponents > 6) {
                // Get the root directory
                std::string rootDir = pathComponents.front();

                // Get the last 'numComponents' components
                std::vector<std::string> lastComponents;
                for (size_t i = pathComponents.size() - numComponents; i < pathComponents.size(); ++i) {
                    lastComponents.push_back(pathComponents[i]);
                }

                // Use the shortenPath function to construct the result
                result = shortenPath(rootDir, lastComponents);
                return result;
            } else {
                // Join the last 'numComponents' components to form the result
                for (size_t i = pathComponents.size() - numComponents; i < pathComponents.size(); ++i) {
                    result += "/" + pathComponents[i];
                }
                return result;
            }
        }
    }
    namespace pipe_functions {
        bool pipeCommands_simple(const std::string& command1, const std::string& command2) {
            int pipefd[2]; // File descriptors for the pipe

            // Create the pipe
            if (pipe(pipefd) == -1) {
                perror("pipe");
                return false;
            }

            // Fork a child process
            pid_t childPid = fork();

            if (childPid == -1) {
                perror("fork");
                return false;
            }

            if (childPid == 0) {
                // Child process (command1)
                close(pipefd[0]); // Close the read end of the pipe
                dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to the pipe
                close(pipefd[1]); // Close the write end of the pipe

                // Execute command1
                if (system(command1.c_str()) == -1) {
                    perror("system");
                    exit(1);
                }
                exit(0);
            } else {
                // Parent process (command2)
                close(pipefd[1]); // Close the write end of the pipe
                dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to the pipe
                close(pipefd[0]); // Close the read end of the pipe

                // Execute command2
                if (system(command2.c_str()) == -1) {
                    perror("system");
                    return false;
                }

                // Wait for the child process to finish (optional)
                int status;
                waitpid(childPid, &status, 0);
            }

            return true;
        }
        bool pipeCommands(const std::vector<std::string>& commands) {
            if (commands.empty()) {
                std::cerr << "No commands provided." << std::endl;
                return false;
            }

            std::vector<int> pipefds(commands.size() - 1);

            // Create pipes
            for (size_t i = 0; i < pipefds.size(); ++i) {
                if (pipe(pipefds.data() + i) == -1) {
                    perror("pipe");
                    return false;
                }
            }

                for (size_t i = 0; i < commands.size(); ++i) {
                    pid_t childPid = fork();

                    if (childPid == -1) {
                        perror("fork");
                        return false;
                    }

                    if (childPid == 0) {
                    // Child process

                    // Set up stdin
                    if (i > 0) {
                        dup2(pipefds[i - 1], STDIN_FILENO);
                        close(pipefds[i - 1]);
                    }

                    // Set up stdout
                    if (i < commands.size() - 1) {
                        dup2(pipefds[i], STDOUT_FILENO);
                        close(pipefds[i]);
                    }

                    // Close all remaining pipe file descriptors in the child
                    for (size_t j = 0; j < pipefds.size(); ++j) {
                        close(pipefds[j]);
                    }

                    // Execute the command
                    const char* command = commands[i].c_str();
                    execlp(command, command, nullptr);

                    // If execlp fails, print the error message and exit
                    perror("execlp");
                    exit(1);
                } else {
                    // Parent process
                    if (i > 0) {
                        close(pipefds[i - 1]);
                    }
                    int status;
                    waitpid(childPid, &status, 0);
                }
            }

            return true;
        }
    }
    std::string getCurrentDirectory() {
        char buffer[1024]; // Create a buffer to store the directory path

        if (getcwd(buffer, sizeof(buffer)) != NULL) {
            return std::string(buffer); // Convert the C-string to a C++ string
        } else {
            std::cerr << "Error getting current working directory: " << strerror(errno) << std::endl;
            return ""; // Return an empty string on error
        }
    }
    void input_to_file(const std::string& input, const std::string& directory, const std::string& FILEname) {
        std::string full_path = directory + "/" + FILEname;
        if (fs::exists(directory) && fs::is_directory(directory)) {
            std::cout << "input_to_file: Directory: '" << directory << "' is a directory\n";
            if (fs::exists(full_path) && fs::is_directory(full_path)) {
                error_message("input_to_file", "target file '" + FILEname + "' is a directory");
                return;
            }
            if (fs::exists(full_path)) {
                std::cout << "input_to_file: target file '" << full_path << "' exists\n";
                std::ofstream openFILE;
                openFILE.open(full_path, std::ios::app);
                if (openFILE.is_open()) {
                    openFILE << input << std::endl;
                    openFILE.close();
                    std::cout << "input_to_file: successfully appended '" << input << "' to '" << full_path << "'\n";
                } else {
                    error_message("input_to_file", "can't open '" + FILEname + "'");
                    return;
                }
            } else {
                error_message("input_to_file", "target file '" + full_path + "' does not exist");
                return;
            }
        } else if (fs::exists(directory)) {
            error_message("input_to_file", "'" + directory + "' is not a directory; it is a file");
            return;
        } else {
            error_message("input_to_file", "Directory '" + directory + "' does not exist");
            return;
        }
    }
    void inputF(const std::string& input, const std::string& full_path) {
        if (fs::exists(full_path) && fs::is_directory(full_path)) {
            error_message("input_to_file", "'" + full_path + "' is a directory not a file");
            return;
        } else if (fs::exists(full_path)) {
            std::cout << "input_to_file: target file '" << full_path << "' exists\n";
            std::ofstream openFILE;
            openFILE.open(full_path, std::ios::app);
            if (openFILE.is_open()) {
                openFILE << input;
                openFILE.close();
                std::cout << "input_to_file: successfully appended \n'" << input << "'\n to '" << full_path << "'\n\n";
            } else {
                error_message("input_to_file", "can't open '" + full_path + "'");
                return;
            }
        }  else {
            error_message("input_to_file", "target file '" + full_path + "' does not exist");
            return;
        }
    }
    void create_file(const std::string& full_PATH_to_file) {
        if (fs::exists(full_PATH_to_file)) {
            if (fs::is_directory(full_PATH_to_file)) {
                error_message("create_file", "target file '" + full_PATH_to_file + "' is a directory");
                return;
            } else {
                error_message("create_file", "target file '" + full_PATH_to_file + "' already exists");
                return;
            }
        } else {
            std::ofstream outFile;
            outFile.open(full_PATH_to_file, std::ios::out | std::ios::trunc);
            if (!outFile.is_open()) {
                error_message("create_file", "Failed to create a new file at target '" + full_PATH_to_file + "'");
                return;
            } else {
                outFile.close();
                std::cout << "create_file: created an empty file at '" << full_PATH_to_file << "'\n";
                return;
            }
        }
    }
    void export_from_file(const std::string& FILEname, std::string& evar) {
        if (fs::exists(FILEname)) {  // if target file exists
            if (!fs::is_directory(FILEname)) {  // if target file is not a directory
                if (!fs::is_empty(FILEname)) {  // if target file is not empty
                    std::ifstream inFile;
                    inFile.open(FILEname);
                    if (inFile.is_open()) {  // if function was able to open the file
                        std::cout << "export_from_file: successfully opened target file '" + FILEname + "'\n";
                        std::string var;
                        std::stringstream buffer;
                        while (getline(inFile, var)) {
                            buffer << var << '\n';
                        }
                        evar = buffer.str();
                        if (!evar.empty()) {
                            std::cout << "export_from_file: successfully read target file '" + FILEname + "'\n";
                        }
                        inFile.close();
                    } else {  // if function was unable to open the file
                        error_message("export_from_file", "can't open '" + FILEname + "'");
                    }
                } else {  // if target file is empty
                    error_message("export_from_file", "target file '" + FILEname + "' is empty");
                }
            } else {  // if target file is a directory
                error_message("export_from_file", "target file '" + FILEname + "' is a directory");
                return;
            }
        } else {  // if target file does not exist
            error_message("export_from_file", "target file '" + FILEname + "' does not exist");
        }
    }
    bool rm_f(const std::string& filePath) {
        if (std::remove(filePath.c_str()) == 0) {
            std::cout << "File '" << filePath << "' deleted successfully." << std::endl;
            return true;
        } else {
            std::cerr << "Error deleting file '" << filePath << "'" << std::endl;
            return false;
        }
    }

    void cmkdir(const std::string& full_PATH_to_dir){            // make dir if one does not exist at target
        if (!fs::exists(full_PATH_to_dir)) {            // check if dir already exxist
            try {                                               //trys to create directory
                fs::create_directory(full_PATH_to_dir); std::cout << "Directory created successfully" << std::endl;
            }
            catch (const fs::filesystem_error& e) {
                std::cerr << "ERROR creating directory" << e.what() << std::endl;
            }
        } else {
            std::cout << "Directory already exists." << std::endl;
        }
    }
    void cddir(const std::string& full_PATH_to_dir) {            // move active directory to target
        if (fs::exists(full_PATH_to_dir) && fs::is_directory(full_PATH_to_dir)) {
            std::cout << "cddir: Directory '" << full_PATH_to_dir << "' Exists\n";
            if (chdir(full_PATH_to_dir.c_str()) != 0) {
                error_message("cddir", "Failed to change directory.");
                return;
            } else if (chdir(full_PATH_to_dir.c_str()) == 0) {
                std::cout << "cddir: changed Directory to: '" << full_PATH_to_dir.c_str() << "'\n";
            }
        } else if (full_PATH_to_dir == "home"){
            return;
        } else if (fs::exists(full_PATH_to_dir)) {
            error_message("cddir", "'" + full_PATH_to_dir + "' is a file.");
            return;
        } else {
            error_message("cddir", "'" + full_PATH_to_dir + "' dosent exist.");
            return;
        }
    }
    void updatePrompt() {
        //std::string currentDir = getCurrentDirectory();
        std::string newPrompt = "Shell@(" + getCurrentDirectory() + ")> ";
        //std::string newPrompt = "Shell@(" + currentDir + ")> ";
        rl_set_prompt(newPrompt.c_str()); // Set the new prompt with the current directory
    }
    std::stringstream findCommand(const std::string& command) {
        std::array<char, 128> buffer;
        std::stringstream resultStream;

        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) {
            std::cerr << "Error executing the command." << std::endl;
            return resultStream; // Return an empty stringstream
        }

        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            resultStream << buffer.data(); // Append to the stringstream
        }
        pclose(pipe);
        return resultStream;
    }
    namespace c_cp_tools {
        bool calcCRC32(const std::string& filename, uint32_t& checksum) {
            std::ifstream file(filename, std::ios::binary);
            if (!file.is_open()) {
                std::cerr << "calcCRC32: Error: Cannot open FILE: '" << filename << "'\n";
                return false;
            }
            if (file.is_open()) {
                std::cout << "calcCRC32: SUCCESSFULLY Opened '" << filename << "'\n";
                uint32_t crc = 0;
                char buffer[1024];
                while (!file.eof()) {
                    file.read(buffer, sizeof(buffer));
                    crc = static_cast<uint32_t>(crc32(crc, reinterpret_cast<const Bytef*>(buffer), static_cast<uInt>(file.gcount())));
                    //crc = crc32(crc, reinterpret_cast<const Bytef*>(buffer), static_cast<uInt>(file.gcount()));
                }
                file.close();
                checksum = crc;
                std::cout << "calcCRC32: checksum for FILE: '" << filename << "' is '" << checksum << "'\n";
            }
            return true;
        }
        void Checksum(const std::string& sourcePath, const std::string& destinationPath) {
            uint32_t source_checksum;
            // returns true if calcCRC32 was able to calculate checksum for Source file and false if it couldent
            bool source = calcCRC32(sourcePath, source_checksum);
            if (source) {                                       // if source is true
                std::cout << "Checksum: source FILE: '" << sourcePath << "' has checksum '"<< source_checksum << "'\n";
            } else {                                            // if source is false
                std::cerr << "Failed to calculate checksum for FILE: '" << sourcePath << "'\n";
            }
            uint32_t destination_checksum;
            // returns true if calcCRC32 was able to calculate checksum for Destination and false if it couldent
            bool destination = calcCRC32(destinationPath, destination_checksum);
            if (destination) {                                  // if destination is true
                std::cout << "Checksum: destination FILE: '" << destinationPath << "' has checksum '"<< destination_checksum << "'\n";
            } else {                                            // if destination is false
                std::cerr << "Failed to calculate checksum for FILE: '" << destinationPath << "'\n";
            }
            if (source_checksum == destination_checksum) {      // if checksum of source and destination match print success message
                std::cout << "Checksum: Match!!\n";
            } else {                                            // if checksum does NOT match print error and wait for user to press enter to continue
                std::cerr << "Checksum: Dosent Match!! (press enter to continue): " << std::cin.get() << "\n";
            }
        }
        void copyFile(const std::string& sourcePath, const std::string& destinationPath) {
            // open source file
            std::ifstream source(sourcePath, std::ios::binary);

            // open destination file
            std::ofstream destination(destinationPath, std::ios::binary);

            if (!source) {                                      // if source file coult not be opened tell user and exit function
                std::cerr << "copyFile: Error opening source file: " << sourcePath << "' (press enter to continue): ";
                std::cin.ignore();

                while (std::cin.get() != '\n') {                // loop until enter is input by user
                    std::cerr << "copyFile: (press enter to continue)";
                    std::cin.ignore();                          // ignore input becuse it was not enter as enter would have ended the loop
                }
                return;
            }
            if (!destination) {                                 // if destination file could not be opened close function
                std::cerr << "Error opening destination file: " << destinationPath << std::endl;
                return;
            }

            // Use istreambuf_iterator to copy file
            destination << source.rdbuf();

            source.close();                                     // closes source file
            destination.close();                                // closes destination file

            Checksum(sourcePath, destinationPath);
        }
    }
    void c_cp(const std::string& sourcePath, const std::string& destinationPath, bool force = false) {
        using namespace c_cp_tools;
        if (fs::exists(sourcePath)) {
            std::cout << "c_cp: Source FILE: '" << sourcePath << "' Does exist\n";
            if (!fs::exists(destinationPath)) {
                std::cout << "c_cp: Destination FILE: '" << destinationPath << "' Does not exist" << std::endl;
                copyFile(sourcePath, destinationPath);
            } else {
                if (!force){
                    char answer;
                    std::cout << "c_cp: Destination FILE: '" << destinationPath << "' Does exist\nc_cp: do you want to overwrite FILE ?: ";
                    std::cin >> answer;
                    if (answer == 'Y' || answer == 'y') {
                        copyFile(sourcePath, destinationPath);
                    } else {
                        return;
                    }
                } else {
                    std::cout << "c_cp: Destination FILE: '" << destinationPath << "' Does exist\nc_cp: (FORCE MODE) enabled, will OWERWRITE: '" << destinationPath << "' with content of: '" << sourcePath << "'\n";
                    copyFile(sourcePath, destinationPath);
                }
            }
        } else {
            std::cerr << "c_cp: Source FILE: '" << sourcePath << "' Does not exist (press enter to continue): ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            return;
        }
    }
    void runProgram(const std::string& program) {
        if (fork() == 0) {
            // Child process
            execlp(program.c_str(), program.c_str(), nullptr);
            perror("execlp failed"); // Print an error message if execlp fails
            exit(1);
        } else {
            // Parent process
            wait(NULL); // Wait for the child process to finish
        }
    }
    void runProgramWargs(const std::string &program, const std::vector<std::string> &args) {
        std::vector<char *> argv;
        for (const std::string &arg : args) {
            argv.push_back(const_cast<char *>(arg.c_str()));
        }
        argv.push_back(nullptr);

        if (fork() == 0) {
            // Child process
            execvp(program.c_str(), argv.data());
            perror("execvp failed"); // Print an error message if execvp fails
            exit(1);
        } else {
            // Parent process
            wait(NULL); // Wait for the child process to finish
        }
    }
    void runProgramWithInput(const std::string& program, const std::vector<std::string>& args, const std::string& input) {
        std::vector<char*> argv;
        for (const std::string& arg : args) {
            argv.push_back(const_cast<char*>(arg.c_str()));
        }
        argv.push_back(nullptr);

        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("pipe failed");
            exit(1);
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork failed");
            exit(1);
        }

        if (pid == 0) {
            // Child process
            close(pipefd[1]);  // Close the write end of the pipe

            // Redirect stdin to read from the pipe
            if (dup2(pipefd[0], STDIN_FILENO) == -1) {
                perror("dup2 failed");
                exit(1);
            }

            close(pipefd[0]);  // Close the read end of the pipe

            // Execute the program
            execvp(program.c_str(), argv.data());
            perror("execvp failed");
            exit(1);
        } else {
            // Parent process
            close(pipefd[0]);  // Close the read end of the pipe

            // Write the input to the child process's stdin through the pipe
            if (write(pipefd[1], input.c_str(), input.length()) == -1) {
                perror("write failed");
                exit(1);
            }

            close(pipefd[1]);  // Close the write end of the pipe

            // Wait for the child process to finish
            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status)) {
                int exitStatus = WEXITSTATUS(status);
                std::cout << "Child process exited with status: " << exitStatus << std::endl;
            }
        }
    }
    std::uintmax_t getFileSize(const std::string& filePath) {
        try {
            // Use std::filesystem to get the file size
            return fs::file_size(filePath);
        } catch (const fs::filesystem_error& e) {
            // Handle any filesystem-related errors here
            std::cerr << "Filesystem error: " << e.what() << std::endl;
            return 0; // Return 0 to indicate an error
        }
    }
    namespace c_ls {
        void listCurrentDirectory_old() {
            // Get the current directory path as a string
            std::string currentDir = fs::current_path().string();

            std::cout << "Contents of current directory (" << currentDir << "):" << std::endl;

            try {
                for (const auto& entry : fs::directory_iterator(currentDir)) {

                    std::cout << entry.path().filename() << std::endl; // Print the filename or directory name
                }
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
        void listDirectory(const std::string& full_PATH_to_dir) {
            std::cout << "Contents of directory ( " << full_PATH_to_dir << " ):\n";

            try {
                // Create a vector to store directory entries
                std::vector<fs::directory_entry> entries;

                for (const auto& entry : fs::directory_iterator(full_PATH_to_dir)) {
                    // Store the directory entries in the vector
                    entries.push_back(entry);
                }

                // Sort the vector alphabetically by filename
                std::sort(entries.begin(), entries.end(), [](const fs::directory_entry& a, const fs::directory_entry& b) {
                    return a.path().filename() < b.path().filename();
                });

                // Print the sorted contents
                for (const auto& entry : entries) {
                    std::cout << entry.path().filename() << std::endl; // Print the filename or directory name
                }
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }
    namespace edit_main {
        int edit(const std::string& full_PATH_to_file) {
            std::string tmp_string;
            export_from_file(full_PATH_to_file, tmp_string);

            initscr(); // Initialize the ncurses library
            raw();     // Disable line buffering
            keypad(stdscr, TRUE); // Enable special keys (e.g., arrow keys)
            noecho();  // Don't display typed characters

            WINDOW* inputWin = newwin(1, tmp_string.size(), 0, 0); // 1 row, width based on file content, at row 0, column 0

            mvwprintw(inputWin, 0, 0, "%s", tmp_string.c_str());
            wrefresh(inputWin);

            int ch;
            int cursorPosition = 0; // Initialize cursor position

            while ((ch = getch()) != '\n') {
                switch (ch) {
                    case KEY_LEFT:
                        if (cursorPosition > 0)
                            cursorPosition--;
                        break;
                    case KEY_RIGHT:
                        if (cursorPosition < tmp_string.size())
                            cursorPosition++;
                        break;
                    case KEY_BACKSPACE:
                        if (cursorPosition > 0) {
                            tmp_string.erase(cursorPosition - 1, 1);
                            cursorPosition--;
                        }
                        break;
                    default:
                        tmp_string.insert(cursorPosition, 1, ch);
                        cursorPosition++;
                        break;
                }

                // Clear and refresh the input window
                werase(inputWin);
                mvwprintw(inputWin, 0, 0, "%s", tmp_string.c_str());
                wmove(inputWin, 0, cursorPosition);
                wrefresh(inputWin);
            }

            // Update the file with the edited content if needed

            endwin();
            return 0;
        }

    }
    namespace startgui_xterm {
    }
    namespace run {
        char runExecutable_6(const std::string& program, const std::vector<std::string>& args) {
            // Initialize file actions
            posix_spawn_file_actions_t file_actions;
            if (posix_spawn_file_actions_init(&file_actions) != 0) {
                return -1; // Error initializing file actions
            }

            struct FileActionsRAII {
                posix_spawn_file_actions_t* fa;
                FileActionsRAII(posix_spawn_file_actions_t* fa) : fa(fa) {}
                ~FileActionsRAII() { posix_spawn_file_actions_destroy(fa); }
            } fileActionsRAII(&file_actions);

            // Create argv
            std::vector<const char*> argv(args.size() + 2, nullptr);
            argv[0] = program.c_str();
            for (size_t i = 0; i < args.size(); ++i) {
                argv[i + 1] = args[i].c_str();
            }

            pid_t pid;
            if (posix_spawn(&pid, program.c_str(), &file_actions, NULL, const_cast<char* const*>(argv.data()), environ) != 0) {
                return -2; // Error spawning process
            }

            int status;
            if (waitpid(pid, &status, 0) == -1) {
                return -3; // Error waiting for process
            }

            if (WIFEXITED(status)) {
                return static_cast<char>(WEXITSTATUS(status)); // Return the exit status as a char
            } else {
                return -4; // Process did not exit normally
            }
        }
        int program_start_wargs(const std::string& program, const std::vector<std::string>& args) {
            char result = runExecutable_6(program, args);
            if (result == -1) {
                std::cerr << "Error initializing file actions." << std::endl;
            } else if (result == -2) {
                std::cerr << "Error spawning process." << std::endl;
            } else if (result == -3) {
                std::cerr << "Error waiting for process." << std::endl;
            } else if (result == -4) {
                std::cerr << "Process did not exit normally." << std::endl;
            } else {
                std::cout << "Process exited with status: " << static_cast<int>(result) << std::endl;
            }
            return result;
        }
        char runExecutable(const std::string& program) {
            char* empty_env[] = {nullptr};
            pid_t pid;
            if (posix_spawn(&pid, program.c_str(), nullptr, nullptr, empty_env, environ) != 0) {
                return -1; // Error spawning process
            }

            int status;
            if (waitpid(pid, &status, 0) == -1) {
                return -2; // Error waiting for process
            }

            if (WIFEXITED(status)) {
                return static_cast<char>(WEXITSTATUS(status)); // Return the exit status as a char
            } else {
                return -3; // Process did not exit normally
            }
        }
        int program_start(const std::string& program) {

            char result = runExecutable(program);

            if (result == -1) {
                std::cerr << "Error spawning process." << std::endl;
            } else if (result == -2) {
                std::cerr << "Error waiting for process." << std::endl;
            } else if (result == -3) {
                std::cerr << "Process did not exit normally." << std::endl;
            } else {
                std::cout << "Process exited with status: " << static_cast<int>(result) << std::endl;
            }

            return result;
        }
        int runExecutable_7(const std::string& program, const std::vector<std::string>& args) {
            posix_spawn_file_actions_t file_actions;
            posix_spawn_file_actions_init(&file_actions);

            std::vector<const char*> argv;
            argv.reserve(args.size() + 2); // Reserve space to avoid reallocations
            argv.push_back(program.c_str());
            for (const auto& arg : args) {
                argv.push_back(arg.c_str());
            }
            argv.push_back(nullptr);

            pid_t pid;
            if (posix_spawn(&pid, program.c_str(), &file_actions, NULL, const_cast<char* const*>(argv.data()), environ) != 0) {
                perror("posix_spawn");
                return -1;
            }

            int status;
            if (waitpid(pid, &status, 0) == -1) {
                perror("waitpid");
                return -2;
            }

            if (WIFEXITED(status)) {
                return WEXITSTATUS(status);
            } else {
                return -3;
            }
        }
        void fullRun(const std::string& program) {
            if (execl(program.c_str(), program.c_str(), nullptr) == -1) {
                perror("execl");
                std::exit(EXIT_FAILURE);
            }
        }
        void runParallel(const std::string& program) {
            // Get the number of hardware threads available
            unsigned int numThreads = std::thread::hardware_concurrency();
            std::cout << "Number of CPU cores: " << numThreads << std::endl;

            std::vector<std::thread> threads;

            // Atomic flag to track if any thread failed
            std::atomic<bool> hasFailed(false);

            // Create and start threads to run Geekbench
            for (unsigned int i = 0; i < numThreads; ++i) {
                threads.push_back(std::thread([&program, &hasFailed]() {
                    int result = program_start(program);
                    if (result != 0) {
                        std::cerr << "Thread failed with result: " << result << std::endl;
                        hasFailed.store(true);
                    }
                }));
            }

            // Wait for all threads to finish
            for (auto& thread : threads) {
                thread.join();
            }

            if (!hasFailed.load()) {
                std::cout << "All threads have completed running successfully." << std::endl;
            } else {
                std::cerr << "One or more threads failed while running '" << program << "'." << std::endl;
            }
        }
    }
    namespace outputfrompipe {
        std::string executeCommandAndGetOutput(const std::string& command) {
            // Create a pipe to capture the command's output
            int pipefd[2];
            if (pipe(pipefd) == -1) {
                perror("pipe");
                return "";
            }

            pid_t childPid = fork();

            if (childPid == -1) {
                perror("fork");
                return "";
            }

            if (childPid == 0) {
                // Child process

                // Close the write end of the pipe, as it's not needed in the child
                close(pipefd[1]);

                // Redirect the standard output to the write end of the pipe
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]);

                // Execute the command
                const char* cmd = command.c_str();
                execlp(cmd, cmd, nullptr);

                // If execlp fails, print an error message and exit
                perror("execlp");
                exit(1);
            } else {
                // Parent process

                // Close the read end of the pipe, as it's not needed in the parent
                close(pipefd[0]);

                // Create a buffer to read the command's output
                constexpr size_t bufferSize = 4096;
                char buffer[bufferSize];
                std::string output = "";

                // Read the command's output and capture it into the 'output' string
                ssize_t bytesRead;
                while ((bytesRead = read(pipefd[1], buffer, bufferSize)) > 0) {
                    output += std::string(buffer, bytesRead);
                }

                // Close the write end of the pipe
                close(pipefd[1]);

                // Wait for the child process to finish
                int status;
                waitpid(childPid, &status, 0);

                return output;
            }
        }
        std::string executeCommand(const std::string& command) {          // uses pipes to search for string in output of executed command
            std::string result; std::array<char, 128> buffer;

            FILE* pipe = popen(command.c_str(), "r");
            if (!pipe) {
                std::cerr << "Error executing the command." << std::endl;
                return "";
            }

            while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
                result += buffer.data();
            }
            pclose(pipe); return result;
        }
        std::string regex_escape(const std::string& input) {
            // Characters to escape: \ ^ $ . | ? * + ( ) [ ] { }
            std::string specialChars = "\\^$.|?*+()[]{}";
            std::string escapedString;

            for (char c : input) {
                if (specialChars.find(c) != std::string::npos) {
                    escapedString += '\\'; // Add a backslash to escape the character
                }
                escapedString += c;
            }

            return escapedString;
        }
        bool search_string_all(std::string& targetstring, const std::string& stringToFind) {
            // Create a regular expression pattern to match the string globally
            std::string escapedString = regex_escape(stringToFind);
            std::regex stringPattern("(" + escapedString + ")");

            std::vector<std::string> lines;
            size_t start = 0;


            // Split the target string into lines
            while (start < targetstring.length()) {
                size_t end = targetstring.find('\n', start);
                if (end == std::string::npos) {
                    end = targetstring.length();
                }
                lines.push_back(targetstring.substr(start, end - start));
                start = end + 1;
            }

            bool found = false;

            // Process each line
            for (const std::string& line : lines) {
                // Search for the string in the line
                if (std::regex_search(line, stringPattern)) {
                    // Print the line with highlighting
                    std::string highlightedLine = std::regex_replace(line, stringPattern, "\033[1;31m$1\033[0m");
                    std::cout << highlightedLine << std::endl;
                    found = true;
                }
            }

            return found;
        }
    }
    namespace c_html {
        int main_html() {

            return 0;
        }
    }
    namespace calculate_tools {
        double add(double a, double b) {                    // Function to add two numbers
            return a + b;
        }
        double subtract(double a, double b) {               // Function to subtract two numbers
            return a - b;
        }
        double multiply(double a, double b) {               // Function to multiply two numbers
            return a * b;
        }
        double divide(double a, double b) {                 // Function to divide two numbers (checks for division by zero)
            if (b == 0) {
                std::cerr << "Error: You have violated the law, math will not accsetpt devision by '0' STOP NOW!!" << std::endl;
                return 0; // You can handle this error differently if needed
            }
            return a / b;
        }
        double solveLinearEquation(double a, double b) {    // Function to solve linear equations ax + b = 0
            if (a == 0) {
                if (b == 0) {
                    std::cout << "Infinite solutions (any x will work)." << std::endl;
                } else {
                    std::cout << "No solution (contradiction)." << std::endl;
                }
                return NAN; // Not-a-Number to indicate no valid solution
            } else {
                double x = -b / a;
                std::cout << "Solution: x = " << x << std::endl;
                return x;
            }
        }
    }
}
class ThreadPool {
public:
    ThreadPool(size_t numThreads) {
        workers.reserve(numThreads);
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this] { return stop || !tasks.empty(); });
                        if (stop && tasks.empty()) {
                            return;
                        }
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }


    template <class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>> {
        using return_type = std::invoke_result_t<F, Args...>;
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        std::future<return_type> result = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (stop) {
                throw std::runtime_error("enqueue on stopped ThreadPool");
            }
            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
        return result;
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread& worker : workers) {
            worker.join();
        }
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop = false;
};
namespace run {
    int run_exec_with_return_code(const std::string& program) {
        // Implement the task you want to run here
        // For running an executable, you can use the runExecutable function
        char result = mellwscommands::run::runExecutable(program);

        if (result == 0) {
            std::cout << "Task completed successfully." << std::endl;
        } else {
            std::cerr << "Task failed with result: " << static_cast<int>(result) << std::endl;
        }

        return static_cast<int>(result); // Convert char result to int for consistency
    }
    void async_binary(const std::string& binaryPath) {
        std::future<int> result = std::async(std::launch::async, mellwscommands::exec_tools::Binary_with_std_system, binaryPath);
        int exitCode = result.get();                        // Wait for the binary execution to complete and get the exit code
        if (exitCode == 0) {
            std::cout << "Binary executed successfully." << std::endl;
        } else {
            std::cerr << "Binary execution failed with exit code " << exitCode << std::endl;
        }
    }
}


// Global variables for command history
std::vector<std::string> commandHistory;
int historyIndex = -1;

void loadCommandHistory() {                                 // Function to load command history from a file
    std::ifstream historyFile("/home/mellw/.ShellHistory");
    if (historyFile.is_open()) {
        std::string line;
        while (std::getline(historyFile, line)) {
            commandHistory.push_back(line);
            add_history(line.c_str());
        }
        historyFile.close();
    }
}
void saveCommandHistory() {                                 // Function to save command history to a file
    std::ofstream historyFile("/home/mellw/.ShellHistory");
    if (historyFile.is_open()) {
        for (const std::string& command : commandHistory) {
            historyFile << command << std::endl;
        }
        historyFile.close();
    }
}
void addToHistory(const std::string& command) {             // Function to add a command to history
    commandHistory.push_back(command);
    historyIndex = commandHistory.size() - 1;
}
void addToHistory(const std::vector<std::string> &args) {
    //std::string result = std::accumulate(args.begin(), args.end(), std::string(""));
     std::string result = std::accumulate(args.begin(), args.end(), std::string(""),
                                         [](const std::string& a, const std::string& b) {
                                             return a + " " + b;
                                         });



    commandHistory.push_back(result);
    historyIndex = commandHistory.size() - 1;
}
void displayHistory() {
    std::cout << "Command History:" << std::endl;
    for (const std::string& cmd : commandHistory) {
        std::cout << cmd << "\n";
    }
}
void executeStoredCommand(const std::string& command);
void executeCommand(const std::vector<std::string> &args) {
    if (args[0] == "history") {
        displayHistory();
        return;
    } else if (args[0] == "!!") {
        if (historyIndex >= 0 && historyIndex < static_cast<int>(commandHistory.size())) {
            executeStoredCommand(commandHistory[historyIndex]);
        }
    } else if (args[0][0] == '!' && args[0].size() > 1) {
        int commandNumber = std::stoi(args[0].substr(1));
        if (commandNumber >= 1 && commandNumber <= static_cast<int>(commandHistory.size())) {
            executeStoredCommand(commandHistory[commandNumber - 1]);
        }
    } else if (args[0] == "create_file") {                  // mellws create_file function
        if (args.size() == 2) {
            mellwscommands::create_file(args[1]);
        } else {
            std::cout << "Usage: create_file <filename>" << std::endl;
        }
    } else if (args[0] == "export_from_file") {             // mellws export_from_file function
        // Defines content and then makes sure that content is an empty string
        std::string content; content = "";
        std::string PName= "export_from_file";

        if (args.size() == 6) {
            if (args[2] == "find") {
                if (args[4] == "rw") {
                    std::string backup_file = "/tmp/tmp_backup";
                    uint32_t original = 0;
                    uint32_t backup = 0;
                    mellwscommands::rm_f(backup_file);
                    mellwscommands::c_cp_tools::calcCRC32(args[1], original);
                    mellwscommands::export_from_file(args[1], content);
                    if (content == "") {
                        return;
                    } else {
                        mellwscommands::create_file(backup_file);
                        mellwscommands::inputF(content, backup_file);
                        mellwscommands::c_cp_tools::calcCRC32(backup_file, backup);
                        if (original == backup) {
                            std::cout << content << "\n";
                            std::string wordToFind = args[3];
                            if (mellwscommands::string_manipulation::search_string(content, wordToFind)) {
                                std::string replacement = args[5];
                                if (mellwscommands::string_manipulation::replace_string(content, wordToFind, replacement)) {
                                    std::cout << "String Replacement successful: \n'\n" << content << std::endl;
                                    if (mellwscommands::rm_f(args[1])) {
                                        mellwscommands::create_file(args[1]);
                                        mellwscommands::inputF(content, args[1]);
                                        mellwscommands::rm_f(backup_file);
                                        return;
                                    }
                                } else {
                                    mellwscommands::error_message("replace_string", "cannot replace '" + wordToFind + "' with '" + replacement + "'");
                                    return;
                                }
                            } else if (!mellwscommands::string_manipulation::search_string(content, wordToFind)) {
                                mellwscommands::error_message(PName, "Can Not FIND: '" + wordToFind + "' in file '" + args[1] + "'");
                                return;
                            }
                        } else {                                // if checksums dosent match
                            mellwscommands::error_message("Checksum", "dont Match");
                            return;
                        }
                    }
                }
            } else {
                std::cout << PName << ": usage: export_from_file </name/to/path> <find> <string_to_find> do not start string with /\n";
                return;
            }
        }
        if (args.size() == 4) {
            if (args[2] == "find") {
                mellwscommands::export_from_file(args[1], content);
                if (content == "") {
                    return;
                } else {
                    std::cout << "\nFile content = '( \n\n" << content << "\n)'End of FILE!!!\n";
                    std::string wordToFind = args[3];
                    mellwscommands::string_manipulation::search_string(content, wordToFind);
                }
            } else {
                std::cout << PName << ": usage: export_from_file </name/to/path> <find> <string_to_find> do not start string with /\n";
                return;
            }
        }
        if (args.size() == 2) {
            mellwscommands::export_from_file(args[1], content);
            if (args[1] == "find") {
                return;
            } else {
                std::cout << "\nFile content = '( \n\n" << content << "\n)'End of FILE!!!\n";
                return;
            }
        } else if (args.size() < 2){
            std::cout << "Usage: export_from_file <filename>" << std::endl;
            return;
        }
    } else if (args[0] == "search_string") {                // mellws search_string command
        if (args.size() == 3) {
            std::string targetstring = args[1];
            std::string wordToFind = args[2];
            mellwscommands::string_manipulation::search_string(targetstring, wordToFind);
        } else {
            std::cout << "Usage: search_string <targetstring> <wordToFind>" << std::endl;
        }
    } else if (args[0] == "inputF") {                       // mellws own inputF function to append input to target
        if (args.size() == 3) {
            std::string input = args[1];
            std::string full_path = args[2];
            mellwscommands::inputF(input, full_path);
        } else {
            std::cout << "Usage: inputF <input> <full_path_to_target_file>" << std::endl;
        }
    } else if (args[0] == "run") {
        if (args.size() == 2) {
            try {
            auto asyncTask = mellwscommands::test::runBinaryAsync_with_env(args[1]);
            asyncTask.get(); // Wait for the async task to finish
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
        }
    } else if (args[0] == "test") {
        std::cout << mellwscommands::base_tools::search_in_PATH(args[1]) << "\n";
    } else if (args[0] == "og") {                           // run commands with execvp
        std::vector<std::string> newArgs(args.begin() + 1, args.end());

        std::vector<char*> argv;
            for (const std::string& arg : newArgs) {
                argv.push_back(const_cast<char*>(arg.c_str()));
            }
            argv.push_back(nullptr);  // Null-terminate the array

            // Fork a child process
            pid_t pid = fork();

            if (pid == -1) {
                // Forking failed
                perror("fork");
            } else if (pid == 0) {
                // This is the child process
                // Execute the command
                if (execvp(argv[0], argv.data()) == -1) {
                    if (errno == ENOENT) {
                    std::cerr << "Shell: '" << args[0] << "' no such file or directory\n";
                } else {
                    perror("execvp");
                }
                exit(EXIT_FAILURE);
                }
            } else {
                // This is the parent process
                // Wait for the child to finish
                int status;
                if (waitpid(pid, &status, 0) == -1) {
                    perror("waitpid");
                }
            }
    } else if (args[0] == "cd") {                           // cd to move change active directory
        if (args.size() == 1) {
            std::string home = mellwscommands::base_tools::get_vars::get_HOME_var();
            mellwscommands::base_tools::cddir(home);
        } else if (args.size() == 2) {
            std::string full_PATH_to_dir = args[1];
            mellwscommands::base_tools::cddir(full_PATH_to_dir);
        } else if (args.size() == 3) {
            std::string full_PATH_to_dir = args[1] + " " + args[2];
            mellwscommands::base_tools::cddir(full_PATH_to_dir);
        } else {
            std::cerr << "Usage: cd <full_path_to_target_dir>\n";
        }
    } else if (args[0] == "cp") {
        if (args.size() == 4) {
            if (args[3] == "true") {
                mellwscommands::c_cp(args[1], args[2], true);
                return;
            }
        }
        if (args.size() == 3) {
            mellwscommands::c_cp(args[1], args[2], false);
        } else {
            std::cout << "Usage: cp <source> <destination> <true> To force overwrite" << std::endl;
        }
    } else if (args[0] == "ls") {
        if (args.size() == 3) {
            std::string directoryName = args[1] + " " + args[2];
            mellwscommands::c_ls::listDirectory(directoryName);
            return;
        } else if (args.size() == 2) {
            std::string command = args[0] + " \"" + args[1] + "\"";
            mellwscommands::c_ls::listDirectory(args[1]);
            return;
        } else if (args.size() == 1) {
            std::string currentDir = fs::current_path().string(); // Get the current directory path as a string
            mellwscommands::c_ls::listDirectory(currentDir);
            return;
        } else {
            std::cout << "Usage: c_ls <destination>\n";
            std::cout << "Usage: c_ls <destination>\n";
            std::cout << "Usage: c_ls <destination>\n";
            return;
        }
    } else if (args[0] == "fullRun") {
        if (args.size() == 2) {
            mellwscommands::run::fullRun(args[1]);
        } else {
            std::cout << "usage: fullRun <full_path_to_target>\n";
        }
    } else if (args[0] == "math") {
        if (args.size() == 4) {
            const char* str1 = args[2].c_str();
            double result1 = strtod(str1, NULL);
            const char* str2 = args[3].c_str();
            double result2 = strtod(str2, NULL);
            if (args[1] == "add") {
                double result = mellwscommands::calculate_tools::add(result1, result2);
                std::cout << "math: '" << result1 << "' + '" << result2 << "' = '" << result << "'\n";
                return;
            } else if (args[1] == "sub") {
                double result = mellwscommands::calculate_tools::subtract(result1, result2);
                std::cout << "math: '" << result1 << "' - '" << result2 << "' = '" << result << "'\n";
                return;
            } else if (args[1] == "div") {
                double result = mellwscommands::calculate_tools::divide(result1, result2);
                std::cout << "math: '" << result1 << "' / '" << result2 << "' = '" << result << "'\n";
                return;
            } else if (args[1] == "multi") {
                double result = mellwscommands::calculate_tools::multiply(result1, result2);
                std::cout << "math: '" << result1 << "' * '" << result2 << "' = '" << result << "'\n";
                return;
            } else {
                std::cout << "usage: math <add, sub, div, multi> <num1> <num2>\n";
                return;
            }
        } else {
            std::cout << "usage: math <add, sub, div, multi> <num1> <num2>\n";
            return;
        }
    } else if ((args[0] == "nano" || args[0] == "sudo") && args[1] == "nano") {
        return;
    } else if (args[0] == "file" && (args[1] == "size" || args[1] == "info")) {
        if (args[1] == "size") {
            std::string filePath = args[2];
            std::uintmax_t file_size = mellwscommands::getFileSize(filePath);
            if (file_size != 0) {
                double MB = static_cast<double>(file_size) / (1024 * 1024);
                std::cout << "file size: is '" << file_size << "' bytes = '" << MB << "', megabytes\n";
                return;
            } else {
                mellwscommands::error_message("file size", "Failed in retreving '" + filePath + "' check spelling and premmisions\n");
                return;
            }
        } else if (args[1] == "info") {
            return;
        } else {
            std::cout << "usage: 'file' <'size', 'info'> <full_PATH_to_file>\n";
            return;
        }
    } else if (args[0] == "edit") {
        if (args.size() == 2) {
            if (fs::exists(args[1]) && !fs::is_directory(args[1])) {
                int result = mellwscommands::edit_main::edit(args[1]);
                if (result != 0) {
                    mellwscommands::error_message("edit", "'edit' did not exit correctly");
                    return;
                } else {
                    return;
                }
            } else if (fs::exists(args[1])) {
                std::cout << "edit: target file: '" << args[1] << "' is a directory\n";
                return;
            } else {
                std::cout << "edit: target file: '" << args[1] << "' does not exist\n";
                return;
            }
        } else {
            std::cout << "usage: 'edit' <full_PATH_to_file>\n";
            return;
        }
    } else if (args[0] == "check_active_directory") {
        mellwscommands::check_active_directory();
    } else if (args[0] == "PATH") {
        std::cout << mellwscommands::base_tools::get_vars::get_PATH_var() << "\n";
    } else {
        std::string binaryPath = mellwscommands::base_tools::search_in_PATH(args[0]);
        // std::vector<std::string> newArgs(args.begin() + 1, args.end());


        try {
            auto asyncTask = mellwscommands::test::runBinaryAsync_with_env_wargs(binaryPath, args);
            asyncTask.get(); // Wait for the async task to finish
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;


            std::vector<char*> argv;
            for (const std::string& arg : args) {
                argv.push_back(const_cast<char*>(arg.c_str()));
            }
            argv.push_back(nullptr);  // Null-terminate the array

            // Fork a child process
            pid_t pid = fork();

            if (pid == -1) {
                // Forking failed
                perror("fork");
            } else if (pid == 0) {
                // This is the child process
                // Execute the command
                if (execvp(argv[0], argv.data()) == -1) {
                    if (errno == ENOENT) {
                    std::cerr << "Shell: '" << args[0] << "' no such file or directory\n";
                } else {
                    perror("execvp");
                }
                exit(EXIT_FAILURE);
                }
            } else {
                // This is the parent process
                // Wait for the child to finish
                int status;
                if (waitpid(pid, &status, 0) == -1) {
                    perror("waitpid");
                }
            }
        }
    return;
    }
}


void executeStoredCommand(const std::string& command) {
    // Implement the execution of stored commands here
    std::vector<std::string> storedArgs;
    std::istringstream iss(command);
    std::string token;
    while (iss >> token) {
        storedArgs.push_back(token);
    }

    executeCommand(storedArgs);

}
void executeAndColorize(const std::string& command) {
    FILE* fp = popen(command.c_str(), "r");
    if (fp == nullptr) {
        perror("popen");
        return;
    }

    char buffer[128];
    std::string colorizedOutput;

    // Define ANSI escape codes for colors
    const std::string blueColor = "\x1B[34m";
    const std::string greenColor = "\x1B[32m";
    const std::string resetColor = "\x1B[0m";

    while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
        std::string line(buffer);

        // Check if the line represents a directory (you can customize this check)
        if (line.find('/') != std::string::npos) {
            line = blueColor + line;
        } else if (line.find('*') != std::string::npos) {
            line = greenColor + line;
        }

        colorizedOutput += line;
    }

    pclose(fp);

    // Print the colorized output
    std::cout << colorizedOutput << resetColor;
}
int main() {
    // mellwscommands::base_tools::set_env_var();
    setenv("PATH", mellwscommands::base_tools::get_vars::get_PATH_var().c_str(), 1);
    //  ThreadPool pool(4);
    rl_initialize();
    rl_set_screen_size(1200, 600);
    rl_extend_line_buffer(1200);
    rl_prep_terminal(4);
    using_history();                                        // Enable command history
    loadCommandHistory();

    while (true) {                                          // main shell loop
        char* userInput = nullptr;
        std::string Prompt = "Shell@(" + mellwscommands::getCurrentDirectory_tools::getCurrentDirectory_test() + ")> ";
        const std::string Prompt_colur = "\x1B[34mShell@\x1B[0m(\x1B[32m" + mellwscommands::getCurrentDirectory_tools::getCurrentDirectory_test() + "\x1B[0m)> ";
        const char* hello = Prompt.c_str();

        std::cout << std::flush;
        rl_readline_name = hello;
        userInput = readline(rl_readline_name);

        if (userInput == nullptr) {                         // User pressed Ctrl+D or encountered an error (e.g., end of input)
            break;
        } else if (userInput != nullptr) {                  // Trim leading and trailing whitespace from userInput
            std::string trimmedInput = userInput;
            trimmedInput.erase(trimmedInput.find_last_not_of(" \t\n") + 1);
            if (!trimmedInput.empty()) {                    // Check if the trimmed input is not empty
                add_history(userInput);
                addToHistory(userInput);
                saveCommandHistory();
            }
        }
        std::string inputStr(userInput); free(userInput);

        if (inputStr.find('|') != std::string::npos) {      // Check if the input contains a pipe character '|'
            add_history(inputStr.c_str());
            std::vector<std::string> commands;
            std::istringstream inputStream(inputStr);
            std::string command;
            while (std::getline(inputStream, command, '|')) {
                // Trim leading and trailing whitespace from each command
                size_t first = command.find_first_not_of(" \t\n");
                size_t last = command.find_last_not_of(" \t\n");
                if (first != std::string::npos && last != std::string::npos) {
                    commands.push_back(command.substr(first, last - first + 1));
                }
            }

            std::stringstream com;
            for (const std::string& cmd : commands) {
               com << cmd << " ";
            }

            std::vector<std::string> args;
            std::istringstream iss(com.str());
            // char delimiter = ' ';
            std::string token;
            while (iss >> token) {
                args.push_back(token);
            }

            if (args.size() == 3) {
                // running first command in pipe and saving output tto string output
                std::string output = mellwscommands::outputfrompipe::executeCommand(args[0]);

                if (args[1] == "find") {
                    mellwscommands::outputfrompipe::search_string_all(output, args[2]);
                } else {
                    mellwscommands::error_message("usage pipes", "<command> | <find> <letter or string to find");
                }
            } else if (args.size() == 4){
                std::string output = mellwscommands::outputfrompipe::executeCommand(args[0] + " " + args[1]);
                if (args[2] == "find") {
                    mellwscommands::outputfrompipe::search_string_all(output, args[3]);

                } else {
                    mellwscommands::error_message("usage pipes", "<command> | <find> <letter or string to find");
                }
            }
        } else {                                            // No pipe detected, split the input into tokens as before
            std::vector<std::string> args;
            std::istringstream iss(inputStr);
            std::string token;
            while (iss >> token) {
                args.push_back(token);
            }
            if (!args.empty()) {                            // check that args are not empty
                if (args[0] == "exit") {                    // Exit the shell if userInput is exit
                    saveCommandHistory();                   // save commandHistory to file before exeting
                    break;                                  // break the mail prompt loop
                } else {                                    // Execute the command
                    executeCommand(args);
                }
            }
        }
    }
    return 0;
}
