#include <fstream>
#include <iostream>
#include <string>

class
FileHandler
{
    public:
        FileHandler(const std::string &filename) : m_filename(filename) {}
        bool
        append(const std::string &text)
        {
            std::ofstream file(m_filename, std::ios::app); // Open in append mode

            if
            (
                !file.is_open()
            )
            {
                return false; // Return false if file couldn't be opened
            }
            file << text; // Append text
            file.close();
            return true;
        }

        bool
        open()
        {
            std::ofstream file(m_filename, std::ios::app); // Open in append mode
            bool isOpen = file.is_open();
            file.close();
            return isOpen;
        }

	private:
		std::string m_filename;
};

#include <sstream>
#include <string>
#include <iostream>

class Log {
public:
    static void
    INFO(const std::string& message)
	{
        logMessage("INFO: " + message);
    }

    template<typename T, typename... Args>
    static void
    INFO(const T& first, Args... args)
	{
        logMessage("INFO: " + toString(first, args...));
    }

    static void
    WARNING(const std::string& message)
    {
        logMessage("WARNING: " + message);
    }

    template<typename T, typename... Args>
    static void
    WARNING(const T& first, Args... args)
    {
        logMessage("WARNING: " + toString(first, args...));
    }

    static void
    ERROR(const std::string& message)
    {
        logMessage("ERROR: " + message);
    }

    template<typename T, typename... Args>
    static void
    ERROR(const T& first, Args... args)
    {
        logMessage("ERROR: " + toString(first, args...));
    }

private:
    // Static function for conversion
    template<typename T>
    static std::string
    toString(const T& arg)
    {
        std::ostringstream stream;
        stream << arg;
        return stream.str();
    }

    template<typename T, typename... Args>
    static std::string
    toString(const T& first, Args... args)
    {
        return toString(first) + " " + toString(args...);
    }

    static void
    logMessage(const std::string& message)
    {
        FileHandler file("/home/mellw/log");

        if
        (file.open())
        {
            file.append(message + "\n");
        }
    }
};
