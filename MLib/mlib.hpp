#ifndef MLIB_HPP
#define MLIB_HPP

#ifdef UNUSED_LIBS

    #include <cmath>
    #include <cstdlib>
    #include <cstring>
    #include <fcntl.h>   // For open
    #include <functional>
    #include <initializer_list>
    #include <iostream>
    #include <map>
    #include <memory>
    #include <sstream>
    #include <stdexcept>
    #include <string>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <unistd.h>  // For close
    #include <utility>
    #include <variant>
    #include <vector>

#endif // UNUSED_LIBS
#include <fstream>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <cstdio>
#include <string.h>
#include <cstring>

#ifndef QtLib
#define QrLib


    #include <QDebug>
    #include "QLayout"
    #include "qlogging.h"
    #include "qnamespace.h"
    #include "qobject.h"
    #include "qpoint.h"
    #include "qpropertyanimation.h"
    #include "qpushbutton.h"
    #include "qsize.h"
    #include "qsizepolicy.h"
    #include "qstring.h"
    #include "qvariant.h"
    #include "qvariantanimation.h"
    #include "qwidget.h"
    #include "qwindowdefs.h"
    // #include <KWindowSystem>
    #include <QGuiApplication>
    #include <QObject>
    #include <QSequentialAnimationGroup>
    #include <QSettings>
    #include <QSize>
    #include <QString>
    #include <QtCore/QObject>
    #include <QtWidgets/QApplication>
    #include <QtWidgets/QMenu>
    // #include <kwindowinfo.h>
    #include <wayland-client.h>
    // #include <wayland-xdg-shell-client-protocol.h>
    #include <QtWaylandCompositor/QWaylandCompositor>
    #include <QtWaylandCompositor/QWaylandCompositorExtensionTemplate>
    #include <QtWaylandCompositor/QWaylandCompositorExtension>

    // // #include <QtGui/qtguiglobal.h>
    // // #include <QtWaylandClient/qtwaylandclient-config.h>
    // // #include <qtwaylandclient-config_p.h>
    // // #include <qtwaylandclientexports.h>
    // // #include <QtWaylandClient/qtwaylandclientexports.h>
    // // #include <QWaylandCompositor>
    // // #include <QtWaylandClient/QtWaylandClientDepends>
    // // #include "qtwaylandclientversion.h"
    // // #include "qwaylandclientextension.h"
    // // #include <QtWaylandClient>
    // // #include <QWaylandCompositor>
    // // #include <QtWaylandClient>
    // // #include <QtWaylandClient/private/qwayland-wayland.h>
    // // #include <qgenericunixservices_p.h>
    // // #include <qwaylandwindowmanagerintegration_p.h>

#endif
namespace mlib {
    #ifndef DEBUG
    // #define DEBUG
        class Debug {
            bool verboseLogging;  // Example member variable
            enum mode {
                Qt,
                Cout,
            };
            public:
                Debug() :verboseLogging() {}

                template<typename T, typename... Args>
                void debug(const T& first, const Args&... args) {
                    QStringList list;
                    debugImpl(list, first, args...);
                }
                enum COLOR {
                    PURPLE,
                    BLUE,
                    YELLOW,
                    RESET_COLOR
                };

                void variableValue(const QString functionName, const QString variableName, const QString variableValue) {
                    QString line = functionName + ": " + variableName + ": " + variableValue;
                    std::string line_string = line.toStdString();
                    const char* line_char = line_string.c_str();
                    qDebug("%s", line_char);
                }
                void output(const QString& string) {
                    QString line = string;
                    std::string line_string = line.toStdString();
                    const char* line_char = line_string.c_str();
                    qDebug("%s", line_char);
                }
                QString QStringList_to_QString(const QStringList list) {
                    if (list.empty())
                        return QString();
                    QString result;
                    result += "(";
                    for (int i = 0; i <= list.size() - 1; i += 1) {
                        if (i == list.size() - 1)
                            result += list[i];
                        else
                            result += list[i] + ", ";
                    }
                    result += ")";
                    return result;
                }
                QString blue(const QString& string) {
                    return "\e[34m" + string + "\e[0m";
                }
                std::string constCharToStdString(char*const& string) {
                    std::string stdString = string;
                    return stdString;
                }
                // Convert various types to QString
                QString toQString(const QString     &s)     { return s; }
                QString toQString(const std::string &s)     { return QString::fromStdString(s); }
                QString toQString(const QStringList &s)     { return QStringList_to_QString(s); }
                QString toQString(const char* s) {
                    QString string(s);
                    if (string == "\\func") {
                        string.replace("\\func", "\e[35m");
                        return string;
                    }
                    return QString(s);
                }
                QString toQString(COLOR config) {
                    switch (config) {
                        case PURPLE:
                            return "\e[35m";
                        case BLUE:
                            return "\e[34m";
                        case YELLOW:
                            return "\e[33m";
                        case RESET_COLOR:
                            return "\e[0m";
                    }
                    return QString();
                }
                QString toQString(const QPoint &s) {
                    return "(" + QString::number(s.x()) + ", " + QString::number(s.y()) + ")";
                }
                QString toQString(const QSize &s) {
                    return "(" + QString::number(s.width()) + ", " + QString::number(s.height()) + ")";
                }
                QString toQString(const std::vector<std::pair<double, double>> &s) {
                    QString result = QString();
                    result += "{ ";
                    for (const auto&n : s) {
                        result += "(" + QString::number(n.first) + ", " + QString::number(n.second) + ") ";
                    }
                    result += "}";
                    return result;
                }
                QString toQString(const bool &s) {
                    if (s) {
                        return "(TRUE)";
                    }
                    return "(FALSE)";
                }
                template<typename T>
                QString toQString(const T& value) { return QString::number(value); }
                void debugPrint(const QStringList& list);

                template<typename T, typename... Args>          // Recursive function to collect arguments
                void debugImpl(QStringList& list, const T& first, const Args&... args) {
                    list.append(toQString(first));
                    if constexpr (sizeof...(args) == 0) {
                        // End of recursion, call debugPrint
                        debugPrint(list);
                    } else {
                        // Continue recursion
                        debugImpl(list, args...);
                    }
                }

                class DebugCompositor {
                    public:



                        // Additional methods for different composition styles or formats
                    };
        };
    #endif
    #ifndef FILO
    #define FILO
        class FilO {

        };
    #endif
    #ifndef OPENCLOSEFILES
    #define OPENCLOSEFILES
        class OpenCloseFiles {
            public:
                // Constructor
                OpenCloseFiles(const std::string& filename) : m_filename(filename) {}
                void openFile() {                               // Open file
                    m_filestream.open(m_filename, std::ios::in | std::ios::out);
                }
                void closeFile() {                              // Close file
                    if (m_filestream.is_open()) {
                        m_filestream.close();
                    }
                }
                bool isOpen() const {                           // Check if file is open
                    return m_filestream.is_open();
                }

            private:
                std::string m_filename;
                std::fstream m_filestream;
        };
    #endif
    #ifndef FILEHANDLER
    #define FILEHANDLER
        class FileHandler {
            public:
                FileHandler(const char* filename) : m_fd(-1) {
                    m_fd = open(filename, O_RDWR | O_CREAT, 0666); // Use appropriate flags and mode
                }

                bool isOpen() const {
                    return m_fd != -1;
                }

                void closeFile() {
                    if (m_fd != -1) {
                        close(m_fd);
                        m_fd = -1;
                    }
                }

                ~FileHandler() {
                    closeFile();
                }

            private:
                int m_fd;
        };
    #endif
    #ifdef STRING
    // #define STRING
        class String {
            private:
                char* data;
                int length;
            public:
                String(const char* str = "") {                  // Default constructor
                    length = strlen(str);
                    data = new char[length + 1];
                    strcpy(data, str);
                }
                template<typename T>
                String(T input) {                               // Template constructor
                    char* str = ToMLibString(input);
                    length = strlen(str);
                    data = new char[length + 1];
                    strcpy(data, str);
                    delete[] str; // Delete the temporary string
                }
                ~String() {
                    delete[] data;
                }
                // Friend function for overloading the << operator
                friend std::ostream& operator<<(std::ostream& os, const String& str) {
                    os << str.data;
                    return os;
                }


                template<typename T>
                static char* ToMLibString(T input) {
                }
                template<>
                char* ToMLibString(int number) {
                    bool isNegative = number < 0;
                    int length = static_cast<int>(log10(std::abs(number))) + 1 + (isNegative ? 1 : 0);

                    if (number == 0) { // Handle 0 explicitly
                        length = 1;
                    }

                    char* str = new char[length + 1]; // +1 for null terminator
                    str[length] = '\0'; // Set null terminator

                    int tempNumber = std::abs(number);
                    for (int i = length - 1; i >= (isNegative ? 1 : 0); i--) {
                        str[i] = (tempNumber % 10) + '0'; // Convert digit to char
                        tempNumber /= 10;
                    }

                    if (isNegative) {
                        str[0] = '-';
                    }

                    return str;
                }
                class Number {
                    public:
                        static double log10(double number) {
                            if (number <= 0) {
                                return -1; // log10 is not defined for non-positive numbers
                            }

                            double count = 0;
                            while (number >= 10.0) {
                                number /= 10.0;
                                count++;
                            }
                            while (number < 1.0 && number > 0) {
                                number *= 10.0;
                                count--;
                            }

                            // Basic approximation for the fractional part
                            if (number > 0) {
                                number -= 1.0; // Get the fractional part
                                double fractional = 0.0;
                                double term = number;
                                double denominator = 1.0;
                                for (int i = 1; i <= 10; ++i) { // You can increase the number of iterations for more accuracy
                                    fractional += term / denominator;
                                    term *= -number;
                                    denominator += 1.0;
                                }
                                count += fractional / 2.302585092994046; // Convert ln to log10
                            }

                            return count;
                        }
                        static int abs(const int &number) {
                            return (number < 0) ? -number : number;
                        }
                };
        };
    #endif
    #ifdef VECTOR
    // #define VECTOR
        class Vector {
            private:
                String* array;
                int capacity;
                int currentSize;

                void resize() {
                    capacity *= 2;
                    String* temp = new String[capacity];
                    for (int i = 0; i < currentSize; i++) {
                        temp[i] = array[i];
                    }
                    delete[] array;
                    array = temp;
                }

            public:
                Vector() : capacity(10), currentSize(0) {
                    array = new String[capacity];
                }

                ~Vector() {
                    delete[] array;
                }

                // Template add method
                template<typename T>
                void add(const T& item) {
                    if (currentSize == capacity) {
                        resize();
                    }
                    array[currentSize++] = String(item); // Converts item to String
                }

                // More functions like get, size, etc., can be added.
        };
    #endif
}


#endif // MLIB_HPP
