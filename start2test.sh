#!/bin/bash

set -euo pipefail

# Function to prompt user for confirmation
function confirm_action {
    read -p "$1 (y/N): " response
    case "$response" in
        [yY][eE][sS]|[yY])
            return 0
            ;;
        *)
            return 1
            ;;
    esac
}

# Function to format a partition
function format_partition {
    local partition="$1"
    confirm_action "Do you want to format /dev/$partition?" && yes y | mkfs.ext4 -b 4096 "/dev/$partition"
}

# Function to create and format swap partition
function create_swap_partition {
    local swapfile="/swapfile"
    dd if=/dev/zero of="$swapfile" bs=1M count=3072 status=progress
    chmod 0600 "$swapfile"
    mkswap -U clear "$swapfile"
    swapon "$swapfile"
    echo "$swapfile none swap defaults 0 0" >> /etc/fstab
    free -m
}

# Function to install necessary packages
function install_packages {
    pacman -Syu
    yes y | pacman -S networkmanager grub efibootmgr
}

# Function to configure Grub
function configure_grub {
    local bootvar=$(cat /boot.txt)
    mkdir -p /boot/efi
    mount --mkdir "/dev/$bootvar" /boot/efi
    grub-install --target=x86_64-efi --bootloader-id=GRUB --efi-directory=/boot/efi --removable
    grub-mkconfig -o /boot/grub/grub.cfg
}

# Function to install desktop environment based on user choice
function install_desktop_environment {
    echo "1 = kde-plasma"
    echo "2 = Gnome"
    echo "3 = hyprland"
    echo "4 = kde-minimal"
    echo "5 = lxde"
    echo "6 = xfce"
    read -p 'What DE do you want? ' de_choice

    case "$de_choice" in
        1)
            sh /DM/kde.sh
            ;;
        2)
            sh /DM/gnome.sh
            ;;
        3)
            sh /DM/hyprland.sh
            ;;
        4)
            sh /DM/kdem.sh
            ;;
        5)
            sh /DM/lxdm
            ;;
        6)
            sh /DM/xfce.sh
            ;;
        *)
            echo "Invalid choice. No desktop environment will be installed."
            ;;
    esac
}

# Function to set up users and password
function setup_users {
    useradd -m -g users -G wheel -s /bin/bash mellw
    passwd mellw
    cp /conf/sudoers.tmp /etc/sudoers
}

# Function to disable Baloo indexing
function disable_baloo_indexing {
    balooctl disable
    sudo -u $u balooctl disable
}

# Function to start the installation process
function start_installation {
    echo "Arch Linux Installer"
    echo "-------------------"

    cp /conf/mirrorlist /etc/pacman.d/mirrorlist
    cp /conf/pacman.conf /etc/pacman.conf
    cp /conf/nanorc /etc/nanorc
    cp /conf/locale.gen /etc
    locale-gen

    echo "LANG=en_US.UTF-8" > /etc/locale.conf
    ln -sf /usr/share/zoneinfo/Europe/Stockholm /etc/localtime
    hwclock --systohc --utc
    echo "archPC" > /etc/hostname
    cp /conf/hosts /etc

    install_packages
    configure_grub

    read -p 'Do you want to install Nvidia drivers? (y/N) ' nvidia_choice
    if [ "$nvidia_choice" = "y" ]; then
        yes y | pacman -S nvidia
    fi

    yes y | pacman -S pulseaudio pulseaudio-alsa xorg xorg-xinit xorg-server

    u=$(getent passwd 1000 | cut -d: -f1)
    sudo -u $u sh /yayinstall.sh

    install_desktop_environment
    setup_users

    sh /conf/Bashsetup/bashsetup.sh

    systemctl enable sshd.service
    systemctl start sshd.service

    disable_baloo_indexing

    reboot
}

# Main script execution
start_installation
