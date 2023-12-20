#dd if=/dev/zero of=/swapfile bs=1M count=3072 status=progress
#chmod 0600 /swapfile
#mkswap -U clear /swapfile
#swapon /swapfile
#echo '/swapfile none swap defaults 0 0' >> /etc/fstab
#free -m
clear
useradd -m -g users -G wheel -s /bin/bash mellw
passwd mellw
sudo cp /conf/sudoers.tmp /etc/sudoers
pacman -Syu
clear

read -p 'nvidia? y/N ' d

	if [ $d = y ]; then

	yes y|pacman -S nvidia

	fi
clear
pacman -S pulseaudio pulseaudio-alsa xorg xorg-xinit xorg-server
clear

u=$(getent passwd 1000 | cut -d: -f1)

sudo -u $u sh /yayinstall.sh
clear

echo "1 = kde-plasma"
echo "2 = Gnome"
echo "3 = hyprland"
echo "4 = kde-minimal"
echo "5 = lxde"
echo "6 = xfce"

read -p 'What DE do you want? ' Vvar

	if [ $Vvar = 1 ]; then

		sh /DM/kde.sh

	fi

	if [ $Vvar = 2 ]; then

		sh /DM/gnome.sh

	fi

	if [ $Vvar = 3 ]; then

		sh /DM/hyprland.sh

	fi

	if [ $Vvar = 4 ]; then

		sh /DM/kdem.sh

	fi

	if [ $Vvar = 5 ]; then

		/DM/lxdm

	fi

	if [ $Vvar = 6 ]; then

		sh /DM/xfce.sh

	fi

sh /conf/Bashsetup/bashsetup.sh
systemctl enable sshd.service
systemctl start sshd.service
/endmvfix

clear
/usefull
clear
balooctl disable
clear
sudo -u $u balooctl disable
reboot
