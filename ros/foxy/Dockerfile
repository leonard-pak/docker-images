FROM ros:foxy

ENV USER=$(whoami)
ENV DISPLAY=host.docker.internal:0.0

ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Install ROS packages
RUN apt-get update \
    && apt-get install -y \
    ros-foxy-desktop \
    ros-foxy-moveit \
    && rm -rf /var/lib/apt/lists/*

# Add Toolchain PPA
RUN apt-get update \
    && apt-get install -y \
    software-properties-common \
    && rm -rf /var/lib/apt/lists/* \
    && add-apt-repository -y ppa:ubuntu-toolchain-r/test 

# Install GCC-11
RUN apt-get update \
    && apt-get install -y \
    gcc-11 \
    g++-11 \
    && rm -rf /var/lib/apt/lists/* \
    && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 100 \
    --slave /usr/bin/g++ g++ /usr/bin/g++-11 \
    --slave /usr/bin/gcov gcov /usr/bin/gcov-11

# Update & Upgrade
RUN apt-get update \
    && apt-get upgrade -y \
    && rm -rf /var/lib/apt/lists/*

# Install tools
RUN apt-get update \
    && apt-get install -y \
    wget \
    git \
    vim \
    tree \
    bash-completion \
    && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/MrLeonardPak/workspace-setup.git $HOME/workspace-setup
RUN $HOME/workspace-setup/setup-docker.bash

