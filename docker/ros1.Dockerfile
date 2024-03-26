FROM osrf/ros:noetic-desktop-focal
# Add user and timezone
ENV USER=$(whoami)
ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
# Update & Upgrade
RUN apt-get update \
    && apt-get upgrade -y
# Add Toolchain PPA
RUN apt-get install -y \
    software-properties-common \
    && add-apt-repository -y ppa:ubuntu-toolchain-r/test
# Install GCC-11
RUN apt-get install -y \
    gcc-11 \
    g++-11 \
    && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 100 \
    --slave /usr/bin/g++ g++ /usr/bin/g++-11 \
    --slave /usr/bin/gcov gcov /usr/bin/gcov-11
# Install tools
RUN apt-get update \
    && apt-get install -y \
    curl \
    wget \
    git \
    neovim \
    tree \
    sudo \
    unzip\
    bash-completion
RUN rm -rf /var/lib/apt/lists/*
# Add user
ARG NEW_USER
RUN useradd -ms /usr/bin/bash $NEW_USER
RUN usermod -aG sudo $NEW_USER
RUN passwd -d $NEW_USER
USER $NEW_USER

RUN echo '. /opt/ros/noetic/setup.bash' >> ~/.bashrc

WORKDIR /home/$NEW_USER
