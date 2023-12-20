pacman -S xfce4 lightdm lightdm-gtk-greeter
echo "exec startxfce4" > ~/.xinitrc
systemctl enable lightdm
