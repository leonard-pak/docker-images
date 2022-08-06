FROM ubuntu:latest

ENV USER=$(whoami)
ENV DISPLAY=host.docker.internal:0.0

# Install tools
RUN apt-get update \
    && apt-get upgrade -y \
    && apt-get install -y \
    wget \
    git \
    vim \
    && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/MrLeonardPak/my-workspace.git

RUN /my-workspace/setup-docker.sh

