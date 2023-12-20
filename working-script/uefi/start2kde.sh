dd if=/dev/zero of=/swapfile bs=1M count=3072 status=progress
chmod 0600 /swapfile
mkswap -U clear /swapfile
swapon /swapfile
echo '/swapfile none swap defaults 0 0' >> /etc/fstab
free -m
useradd -m -g users -G wheel -s /bin/bash mellw
passwd mellw
sudo cp /sudoers.tmp /etc/sudoers
pacman -Syu
yes y|pacman -S nvidia
pacman -S pulseaudio pulseaudio-alsa xorg xorg-xinit xorg-server
sh /kde.sh
sh /Bashsetup/Bashsetup/bashsetup.sh
sh /Bashsetup/bashsetup.sh
yes y|pacman -S bpytop
systemctl enable sshd.service
systemctl start sshd.service

