FROM node:latest

ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Update & Upgrade
RUN apt-get update \
    && apt-get upgrade -y

# Install tools
RUN apt-get install -y \
    wget \
    git \
    vim \
    tree \
    bash-completion \
    sudo

RUN rm -rf /var/lib/apt/lists/*

ENV WORKSPACE_USER /home/node
ENV USER=node
RUN echo "node:node" | chpasswd && adduser node sudo

RUN git clone https://github.com/MrLeonardPak/workspace-setup.git \
    $WORKSPACE_USER/workspace-setup

RUN $WORKSPACE_USER/workspace-setup/setup-docker.bash

USER node
