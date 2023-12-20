echo "exec startkde" > ~/.xinitrc
sudo pacman -S plasma-meta
yes y |sudo pacman -S dolphin
yes y |sudo pacman -S yakuake
sudo systemctl enable NetworkManager
sudo systemctl start NetworkManager
sudo systemctl enable sddm.service
