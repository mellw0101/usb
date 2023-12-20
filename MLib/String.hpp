#ifndef STRING_HPP
#define STRING_HPP

#ifdef STRING_HPP

#include <cstring>
#include <math.h>
#include <ostream>

#endif

namespace mlib {
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
}
#endif // STRING_HPP
