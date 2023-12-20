u=$(getent passwd 1000 | cut -d: -f1)
yes y |sudo pacman -S git
yes y |sudo pacman -S base-devel
cd /opt
sudo git clone https://aur.archlinux.org/yay-git.git
sudo chown -R $u:wheel ./yay-git
cd yay-git
yes y | sudo -u $u makepkg -si
