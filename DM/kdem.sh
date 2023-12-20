echo "exec startkde" > ~/.xinitrc
sudo pacman -S plasma-desktop
yes y |sudo pacman -S plasma-pa dolphin dolphin-plugins konsole yakuake sddm kscreen plasma-wayland-session
#lib32-libva-vdpau-driver vala libva-vdpau-driver wayland-utils
sudo systemctl enable NetworkManager
sudo systemctl start NetworkManager
sudo systemctl enable sddm.service
