#ifndef DOCKCONFIG_H
#define DOCKCONFIG_H

#include "debug.h"
#include "qsize.h"
#include <linux/limits.h>
#include <memory>
#include <string>
#include <map>
#include <functional>
#include <iostream>
#include <variant>
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <variant>
#include <string>
#include <functional>
#include <map>
#include <utility>


void initializeVars();

enum File {
    APP_ORDER
};
const std::string dockFile(File config);

enum Vars {
    BUTTON_SIZE,
    ICON_SIZE,
    BUTTON_ICON_DIFFRENCE,
    TOTAL_WIDTH,
    ZOOM_ANIM_DUR,
    BOUNCE_ANIM_DUR,
    BOUNCE_ICON_SIZE,
    SQUEEZE_NUM,
    SCALE_FACTOR,
    TOTAL_BUTTONS,
};

enum Float {
    FLOAT_SCALE_FACTOR
};
enum Calculate {
    SCALE_STEPS,
    TIME_PER_STEP,
    DIFF
};




void actionTwoFunction();

int num(Vars config);
void changeNum(Vars config, const int num);
QSize Qnum(Vars config);
double doubleNum(Float config);
double calculate(Calculate num);


using FunctionReturn = std::variant<int, std::string, double, bool>;
FunctionReturn varsNum(Vars config);

FunctionReturn dock();


FunctionReturn count();

// class Dock {

// };
enum values {
    SIZE,
    COUNT,
};

class Buttons {
public:
    enum class SET {
        SIZE,
        COUNT,
    };
    enum class GET {
        SIZE,
        COUNT,
    };



    static int get(values config) {
        if (config == SIZE) {
            return num(BUTTON_SIZE);
        }
        if (config == COUNT) {
            return num(BUTTON_SIZE);
        }
        return -1;
    }

    static void Set(values config, int value) {
        if (config == SIZE) {
            changeNum(BUTTON_SIZE, value);
        }
        if (config == COUNT) {
            changeNum(TOTAL_BUTTONS, value);
        }
    }

    static std::map<Buttons::GET, std::function<FunctionReturn()>> functionMap;
    static int dock(Buttons::GET action) {
    auto it = functionMap.find(action);
    if (it != functionMap.end()) {
        FunctionReturn result = it->second(); // Call the function
        if (auto intValue = std::get_if<int>(&result)) {
            return *intValue;
        }
    }
    return -1;
}
};
extern std::map<Buttons::GET, std::function<FunctionReturn()>> functionMap;
int dock(Buttons config);
void performAction(Buttons action);

enum ACTION {
    SAVE,
    READ,
    // ... other buttons ...
};
class Action {

public:
    virtual ~Action() {}
    virtual int NOW() = 0;
};

class SaveAction : public Action {
public:
    int NOW() override {
        // Size logic
        return 0/* size value */;
    }
};

class ReadAction : public Action {
public:
    int NOW() override {
        // Count logic
        return 0/* count value */;
    }
};

class Dock {
public:
    static std::unique_ptr<Action> SAVE(ACTION action) {
        switch (action) {
            case ACTION::SAVE:
                return std::make_unique<SaveAction>();
            case ACTION::READ:
                return std::make_unique<ReadAction>();
            // ... other cases ...
        }
        return nullptr;
    }
};
        // FilO(const FilO &) = default;
        // FilO(FilO &&) = default;
        // FilO &operator=(const FilO &) = default;
        // FilO &operator=(FilO &&) = default;
        // // Constructor is private to control creation through the `save` method
        // FilO() : file(std::make_unique<std::fstream>()) {}
                // OpenState(const std::string &openPath);



#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <fcntl.h>   // For open
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>  // For close
#include <iostream>

class FilO {
    private:
        std::string filePath;
        int fd;
        std::unique_ptr<std::fstream> file;


    public:
        // Default constructor
        FilO() : fd(-1), file(std::make_unique<std::fstream>())  {}
        FilO(const std::string &Path) : filePath(Path), fd(-1){
            filePath = Path;
            debug(Path);
            // open(Path);
        }
        // Open file with POSIX calls
        FilO& open(/*const char* filename, int flags*/) {
            fd = ::open(filePath.c_str(), 0, 0666);                // Using Linux system call
            return *this;
        }

        FilO& read(char* buffer, size_t size, ssize_t& bytesRead) {
            bytesRead = ::read(fd, buffer, size); // Using Linux system call
            return *this;
        }
        FilO& Open() {
            file->open(filePath, std::ios::in | std::ios::out);
            if (file->is_open()){
                debug("FILE IS OPEN");
            }
            return *this;
        }

        FilO& read(std::string& outContent) {
            if (file->is_open()) {
                *file >> outContent;
            }
            return *this;
        }

        FilO& write(const std::string& content) {
            if (file->is_open()) {
                *file << content;
            }
            return *this;
        }

        void Close() {
            if (file->is_open()) {
                file->close();
            }
        }

        FilO& write(const char* buffer, size_t size) {
            ::write(fd, buffer, size); // Using Linux system call
            return *this;
        }

        void setPath(const std::string &Path) {
            this->filePath = Path;
            debug(this->filePath);
        }
        void close() {
            if (fd != -1) {
                ::close(fd);
                fd = -1;
            }
        }
        std::string PATH(){
            return FilO::filePath;
        }
        ~FilO() {
            if (fd != -1)
                close();
        }
};


#endif // DOCKCONFIG_H

            // Clean up resources if needed
        // FilO* file(const std::string& filePath){
        //     this->path = filePath;
        //     return this;
        // }

        // Destructor or other methods as necessary
    // Static method to start the chain
    // static AppendState Append(const std::string& data) {
    //     return AppendState(data);
    // }
        // static FilO* Replace(const std::string& data) {
        //     return new FilO(data);
        // }
