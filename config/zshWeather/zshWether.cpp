#include <complex>
#include <cstdlib>
#include <iostream>
#include <curl/curl.h>
#include <string>
#include <sstream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <vector>

class FileHandler {
	public:
	explicit FileHandler ( std::string filename ) : m_filename (std::move( filename )) {}
	
    bool
	overwrite ( const std::string &text ) 
    {
		std::ofstream file ( m_filename);// Open in append mode

		if ( ! file.is_open () ) 
        {
			return false;// Return false if file couldn't be opened
		}
		file << text;// Append text
		file.close ();
		return true;
	}

    bool
	append ( const std::string &text ) 
    {
		std::ofstream file ( m_filename, std::ios::app );// Open in append mode

		if ( ! file.is_open () ) 
        {
			return false;// Return false if file couldn't be opened
		}
		file << text;// Append text
		file.close ();
		return true;
	}

	bool
	open() 
    {
		std::ofstream file(m_filename, std::ios::app);// Open in append mode
		bool isOpen = file.is_open();
		file.close();
		return isOpen;
	}

	private:
	std::string m_filename;
};

class TIME {
	public:
	static std::string get() 
    {
		// Get the current time
		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(now);

		// Convert time_t to tm as local time
		std::tm buf{};
		localtime_r(&in_time_t, &buf);

		// Use stringstream to format the time
		std::ostringstream ss;
		ss << std::put_time(&buf, "%H:%M");
		return ss.str();
	}
};

static size_t 
WriteCallback(void *contents, size_t size, size_t nmemb, std::string *s) 
{
    size_t newLength = size * nmemb;
    try 
    {
        s->append((char*)contents, newLength);
        return newLength;
    }
    catch(std::bad_alloc &e) 
    {
        // handle memory problem
        return 0;
    }
}

std::string
getWeather(const std::string &url) 
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Enable following redirects
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if(res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            return "zshWeather: %F{red}ERROR%f: url-error";
        }
    }

    std::string result;
    std::istringstream iss(readBuffer);
    std::string word;

    while (iss >> word) 
    {
        // Check if the word is non-empty and not just a newline
        if (!word.empty() && word != "\n") 
        {
            
            result += word + " ";
        }
    }

    if (!result.empty())
    {
        result.erase(result.length() - 1);
    }

    std::cout << result << "\n";

    return result;
}

int
main(int argc, char* argv[]) 
{
    FileHandler file("/usb/config/weatherData");
    file.overwrite(getWeather("http://wttr.in/Stockholm?format=3"));
    return 0;
}
