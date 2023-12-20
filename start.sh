cp /conf/mirrorlist /etc/pacman.d/mirrorlist
cp /conf/pacman.conf /etc/pacman.conf
cp /conf/nanorc /etc/nanorc
cp /conf/locale.gen /etc
locale-gen

echo "LANG=en_US.UTF-8" > /etc/locale.conf
ln -sf /usr/share/zoneinfo/Europe/Stockholm /etc/localtime
hwclock --systohc --utc
echo archPC > /etc/hostname
cp /conf/hosts /etc
yes y | pacman -Syyu

yes y |pacman -S networkmanager

systemctl enable NetworkManager

yes y |pacman -S grub efibootmgr

Bootvar=$(cat /boot.txt)
mount --mkdir /dev/$Bootvar /boot/efi
grub-install --target=x86_64-efi --bootloader-id=GRUB --efi-directory=/boot/efi --removable

grub-mkconfig -o /boot/grub/grub.cfg
cp /conf/grub /etc/default/grub

grub-mkconfig -o /boot/grub/grub.cfg

