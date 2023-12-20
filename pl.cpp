#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>

class ThreadPool {
public:
    ThreadPool(size_t numThreads) : stop(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            threads.emplace_back([this] {
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

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread &thread : threads) {
            thread.join();
        }
    }

    template<typename F>
    void enqueue(F&& func) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.emplace(std::forward<F>(func));
        }
        condition.notify_one();
    }

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
};

int runTask(const std::string& program) {
    // Implement the task you want to run here
    // For running an executable, you can use the runExecutable function
    // For demonstration, we'll simulate a task by sleeping for a while.
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0; // Simulated success
}

int main() {
    // Create a thread pool with the number of available CPU cores
    unsigned int numThreads = std::thread::hardware_concurrency();
    ThreadPool pool(numThreads);

    // Function to run the specified program
    auto runProgram = [&pool]() {
        int result = runTask("your_program_name_here"); // Replace with the actual program name
        if (result == 0) {
            std::cout << "Thread completed successfully." << std::endl;
        } else {
            std::cerr << "Thread failed with result: " << result << std::endl;
        }
    };

    // Enqueue the program execution tasks to the thread pool
    for (unsigned int i = 0; i < numThreads; ++i) {
        pool.enqueue(runProgram);
    }

    // Sleep to allow threads to finish (you can implement more sophisticated synchronization)
    std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}
