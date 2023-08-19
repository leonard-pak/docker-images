FROM ros:foxy
# Add user and timezone
ENV USER=$(whoami)
ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
# Update & Upgrade
RUN apt-get update \
    && apt-get upgrade -y
# Install ROS packages
RUN apt-get install -y \
    ros-foxy-desktop
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
    wget \
    git \
    vim \
    tree \
    bash-completion
RUN rm -rf /var/lib/apt/lists/*
# Install workspace
RUN git clone https://github.com/MrLeonardPak/workspace-setup.git $HOME/workspace-setup
RUN $HOME/workspace-setup/setup-docker.bash
RUN $HOME/workspace-setup/setup-ros2.bash
