// /* See LICENSE file for copyright and license details. */
// #include "util.hpp"
//
// void
// die(const char *fmt, ...)
// {
// 	va_list ap;
//
// 	va_start(ap, fmt);
// 	vfprintf(stderr, fmt, ap);
// 	va_end(ap);
//
// 	if (fmt[0] && fmt[strlen(fmt)-1] == ':') {
// 		fputc(' ', stderr);
// 		perror(NULL);
// 	} else {
// 		fputc('\n', stderr);
// 	}
//
// 	exit(1);
// }
//
// void *
// ecalloc(size_t nmemb, size_t size)
// {
// 	void *p;
//
// 	if (!(p = calloc(nmemb, size)))
// 		die("calloc:");
// 	return p;
// }


#include "util.hpp"

[[noreturn]] void die(const std::string &msg) {
    std::cerr << msg << std::endl;
    throw std::runtime_error(msg);
}

void *
ecalloc(size_t nmemb, size_t size)
{
	void *p;

	if (!(p = calloc(nmemb, size)))
		die("calloc:");
	return p;
}

std::unique_ptr<char[]> Uecalloc(size_t nmemb, size_t size) {
    try {
        std::unique_ptr<char[]> p(new char[nmemb * size]);
        memset(p.get(), 0, nmemb * size);
        return p;
    } catch (const std::bad_alloc& e) {
        die("Memory allocation failed: " + std::string(e.what()));
    }
}
