FROM archlinux:latest

# Install necessary packages
RUN pacman -Syyu --noconfirm \
    && pacman -S --noconfirm    xterm \
                                qemu-full \ 
                                virt-manager \
                                wget

RUN systemctl enable libvirtd.service

COPY qemu_run /bin/

# Set the display environment variable
ENV DISPLAY=$DISPLAY

# Start X server and xterm in the container
CMD ["xterm"]
