yes u |mkfs.fat -F32 /dev/sda1
yes y |mkfs.ext4 /dev/sda2
mount /dev/sda2 /mnt
cp /usb/start.sh /mnt
cp /usb/start2.sh /mnt
cp /usb/start2kde.sh /mnt
cp /usb/start3.sh /mnt
cp /usb/sudoers.tmp /mnt
cp /usb/locale.gen /mnt
cp /usb/hosts /mnt
cp /usb/kde.sh /mnt
cp -r /usb/Bashsetup /mnt
cp /usb/pacman.conf /mnt
cp /usb/nanorc /mnt
cd /usb/yayinstall.sh /mnt

printf '1\ny\ny\y' |pacstrap -i /mnt base linux linux-firmware sudo nano
genfstab -U -p /mnt >> /mnt/etc/fstab
echo 'TYPE arch-chroot /mnt /bin/bash THEN TYPE bash /start.sh'

