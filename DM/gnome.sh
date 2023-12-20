echo "exec gnome-session" > ~/.xinitrc
sudo pacman -S gnome
sudo pacman -S gnome-tweaks sddm
sudo systemctl enable NetworkManager
sudo systemctl start NetworkManager
sudo systemctl enable sddm.service
