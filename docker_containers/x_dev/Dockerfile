FROM ubuntu:latest

# Install necessary packages
RUN apt-get update && apt-get install -y 
#    xorg \ 
#    libxcb1-dev \
#    libxcb-util0-dev \
#    libxcb-randr0-dev \
#    libxcb-keysyms1-dev

# Set up a user (replace 'user' with your username)
RUN useradd -ms /bin/bash user
USER user

# Copy your window manager binary into the container (replace 'your_wm_binary' with the actual binary)
COPY /bin/mwm /home/user/

# Set the entry point to start your window manager
#ENTRYPOINT ["/home/user/mwm"]
