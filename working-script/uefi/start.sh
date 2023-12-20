cp /pacman.conf /etc/pacman.conf
cp /nanorc /etc/nanorc
cp /locale.gen /etc
locale-gen
echo "LANG=en_US.UTF-8" > /etc/locale.conf
ln -sf /usr/share/zoneinfo/Europe/Stockholm /etc/localtime
hwclock --systohc --utc
echo archPC > /etc/hostname
cp /hosts /etc
yes y |pacman -S yakuake git
yes y |pacman -S networkmanager
systemctl enable NetworkManager
passwd
yes y |pacman -S grub efibootmgr
mount --mkdir /dev/sda1 /boot/efi
grub-install --target=x86_64-efi --bootloader-id=GRUB --efi-directory=/boot/efi --removable
grub-mkconfig -o /boot/grub/grub.cfg
echo 'TYPE exit THEN umount -R /mnt THEN reboot'





