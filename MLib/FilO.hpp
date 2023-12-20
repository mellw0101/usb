# ifndef FILO_HPP
# define FILO_HPP

// #include <string>
#include <vector>

#include "String.hpp"

# ifdef FILO_HPP

# include <fcntl.h>
# include <unistd.h>

# endif

namespace mlib {
    class FilO {
        public:
            FilO(const char* filename) : m_fd(-1) {
                m_fd = open(filename, O_RDWR | O_CREAT, 0666); // Use appropriate flags and mode
            }
            std::string readFile() {
                if (!isOpen()) {
                    return ""; // Or handle the error as appropriate
                }

                std::string contents;
                std::vector<char> buffer(1024, 0); // Buffer for reading the file
                ssize_t bytesRead = 0;

                while ((bytesRead = read(m_fd, &buffer[0], buffer.size())) > 0) {
                    contents.append(buffer.begin(), buffer.begin() + bytesRead);
                }

                // Reset the file descriptor's position to the beginning of the file
                lseek(m_fd, 0, SEEK_SET);

                return contents;
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
            ~FilO() {
                closeFile();
            }
        private:
            int m_fd;
    };
}

# endif                                                     // FILO_HPP
