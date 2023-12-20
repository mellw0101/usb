#include <cstdlib>

#include "/usb/Shell-0.1/base_tools.h"
#include "/usb/Shell-0.1/export_from_file.h"

int main() {
    tools::cmkdir(get_vars::get_HOME_var() + "/.proton");
    tools::cddir(get_vars::get_HOME_var() + "/.proton");
    system("yay -G proton");
    tools::cddir("proton");
    std::string PKGBUILD;
    tools::export_from_file("PKGBUILD", PKGBUILD);

    std::string find;
    size_t pos;

    while (true) {
        find = "-O2 -march=nocona -mtune=core-avx2";
        pos = PKGBUILD.find(find);
        if (pos != std::string::npos) {
            PKGBUILD.replace(pos, find.length(), "-O3 march=native");
        }
        else {
            break;
        }
    }

    find = "opt-level=2 -C target-cpu=nocona";
    pos = PKGBUILD.find(find);
    if (pos != std::string::npos) {
        PKGBUILD.replace(pos, find.length(), "opt-level=3 -C target-cpu=native");
    }

    find = "WINEESYNC=0";
    pos = PKGBUILD.find(find);
    if (pos != std::string::npos) {
        PKGBUILD.replace(pos, find.length(), "WINEESYNC=1");
    }

    find = "WINEFSYNC=0";
    pos = PKGBUILD.find(find);
    if (pos != std::string::npos) {
        PKGBUILD.replace(pos, find.length(), "WINEFSYNC=1");
    }

    tools::rm_f("PKGBUILD");
    tools::create_file("PKGBUILD");
    tools::inputF(PKGBUILD, "PKGBUILD");

    system("makepkg -si");

    return 0;
}
