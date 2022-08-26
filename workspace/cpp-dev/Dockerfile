FROM gcc:latest

ENV USER=$(whoami)
ENV DISPLAY=host.docker.internal:0.0

ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

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

