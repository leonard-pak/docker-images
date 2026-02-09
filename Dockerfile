FROM ubuntu:latest

ARG NEW_USER
ARG PROJECT_NAME

# Add timezone
ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Install tools
RUN apt-get update && apt-get install -y \
    curl \
    wget \
    iputils-ping \
    git \
    vim \
    unzip\
    tree \
    bash-completion && \
    rm -rf /var/lib/apt/lists/*

# Add user
RUN useradd -ms /bin/bash $NEW_USER
RUN usermod -aG sudo $NEW_USER
RUN passwd -d $NEW_USER
USER $NEW_USER

# Prepare bash
RUN echo "# Color bash invitation" >> ~/.bashrc && \
    echo "C_GREEN='\[\033[1;32m\]'" >> ~/.bashrc && \
    echo "C_CYAN='\[\033[1;36m\]'" >> ~/.bashrc && \
    echo "C_RED='\[\033[1;31m\]'" >> ~/.bashrc && \
    echo "C_NC='\[\033[0m\]'" >> ~/.bashrc && \
    echo "export PS1=\"\${C_CYAN}\u@\${C_RED}[DOCKER]\${C_GREEN}:\w\${C_NC}\\\$ \"" >> ~/.bashrc

# Prepare ssh
RUN mkdir -p ~/.ssh

WORKDIR /$PROJECT_NAME
CMD ["tail", "-f", "/dev/null"]