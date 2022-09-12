FROM ubuntu:latest

ENV USER=$(whoami)

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
