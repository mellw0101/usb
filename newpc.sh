cp /usb/conf/mirrorlist /etc/pacman.d/mirrorlist && cp /usb/conf/pacman.conf.start /etc/pacman.conf && clear && lsblk && echo

echo "What is the name of the drive you want to install arch on" && echo && read -p '  >>  ' Xvar && clear

echo "Do you want to partition the drive? y/N" && echo && read -p '  >>  ' p

	if [ $p = y ]; then cfdisk /dev/$Xvar; fi

Bvar="$Xvar"1 && Rvar="$Xvar"2 && Hvar="$Xvar"3

yes y |mkfs.fat -F32 /dev/$Bvar && yes y |mkfs.ext4 -b 4096 /dev/$Rvar && mount /dev/$Rvar /mnt && yes y |mkfs.ext4 -b 4096 /dev/$Hvar && mkdir /mnt/home && mount /dev/$Hvar /mnt/home && echo $Bvar > /mnt/boot.txt && clear && lsblk

read -p 'Do you want to add another drive y/N ' Dvar

	if [ $Dvar = y ]; then

		read -p '/dev/' Dpvar

		read -p 'Do you want to FORMAT the drive y/N ' Fvar

		if [ $Fvar = y ]; then yes y |mkfs.ext4 -b 4096 /dev/$Dpvar; fi

		mkdir /mnt/home/D && mount /dev/$Dpvar /mnt/home/D && clear && lsblk

			read -p 'Do you want to add one more drive y/N ' Kvar

				if [ $Kvar = y ]; then

					read -p '/dev/' Lvar

					read -p 'Do you want to FORMAT the drive y/N ' Fvar

					if [ $Fvar = y ]; then yes y |mkfs.ext4 -b 4096 /dev/$Lvar; fi

					mkdir /mnt/home/F && mount /dev/$Lvar /mnt/home/F && clear && lsblk

					read -p 'Do you want to add one more drive y/N ' Tvar

						if [ $Tvar = y ]; then

							read -p '/dev/' Xvar

							read -p 'Do you want to FORMAT the drive y/N ' Fvar

                                	        	if [ $Fvar = y ]; then yes y |mkfs.ext4 -b 4096 /dev/$Xvar; fi

							mkdir /mnt/home/K && mount /dev/$Xvar /mnt/home/K

						fi

				fi

	fi

/usb/cp/DM && /usb/cp/setup && /usb/cp/conf && /usb/cp/installers && /usb/cp/working-script

printf '1\ny\ny\y' |pacstrap -i /mnt base linux linux-firmware sudo nano iw
genfstab -U -p /mnt >> /mnt/etc/fstab
arch-chroot /mnt /bin/bash <<"EOT"
sh start.sh
EOT
arch-chroot /mnt /bin/passwd
umount -R /mnt
#shutdown now
