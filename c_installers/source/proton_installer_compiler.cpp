#include <cstdlib>

int main() {

    system("clang++ /usb/Shell-0.1/base_tools.cpp /usb/Shell-0.1/string_manipulation.cpp /usb/Shell-0.1/export_from_file.cpp /usb/Shell-0.1/c_cp.cpp proton_installer.cpp -o proton_installer -stdlib=libc++ -L/usr/lib -l:libc++.a -L/usr/lib -l:libc++abi.a -L/usr/avr/13.2.0 -l:libgcc.a");

    return 0;
}

