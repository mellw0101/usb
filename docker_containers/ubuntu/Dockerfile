# # Use a base image with X11 support
# FROM ubuntu:20.04

# # Install necessary packages
# RUN apt-get update && \
#     DEBIAN_FRONTEND=noninteractive apt-get install -y \
#     xserver-xephyr \
#     xterm \
#     fluxbox \
#     && rm -rf /var/lib/apt/lists/*

# # Set environment variables
# ENV DISPLAY=:1

# # Create a non-root user
# RUN useradd -m -G audio,video user
# USER user
# WORKDIR /home/user

# # Start Xephyr and window manager
# CMD Xephyr $DISPLAY -ac -screen 1280x720 & \
#     sleep 1 && \
#     fluxbox

# FROM archlinux:latest

# # Install necessary packages
# RUN yes y | pacman -Syyu nano sudo xterm 

# # Set the display environment variable
# ENV DISPLAY=:0

# # Start X server
# CMD ["xterm"]


FROM ubuntu:latest

# Install necessary packages
RUN apt-get update && apt-get install -y \
    x11-apps \
    xterm 

# Set the display environment variable
ENV DISPLAY=:0

# Start X server
CMD ["xterm"]


# # Use the official Ubuntu base image
# FROM ubuntu:latest

# # Set environment variables to prevent interactive prompts during installation
# ENV DEBIAN_FRONTEND=noninteractive

# # Copy the preseed file to the container
# COPY preseed.cfg /preseed.cfg

# # Use the preseed file for keyboard configuration
# RUN debconf-set-selections /preseed.cfg
    
# RUN apt-get update

# RUN apt update

# RUN apt-get install -y \
#     xorg \ 
#     libxcb1-dev \
#     libxcb-util0-dev \
#     libxcb-randr0-dev \
#     libxcb-keysyms1-dev && \
#     apt-get clean && \
#     rm -rf /var/lib/apt/lists/*

# # Set the default command to run when the container starts
# CMD ["/bin/bash"]
