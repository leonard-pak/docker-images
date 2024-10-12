FROM gcc:latest
# Add user and timezone
ENV USER=$(whoami)
ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
# Update & Upgrade
RUN apt-get update \
  && apt-get upgrade -y \
  && rm -rf /var/lib/apt/lists/*
# Install tools
RUN apt-get update \
  && apt-get install -y \
  curl \
  wget \
  git \
  neovim \
  tree \
  sudo \
  unzip \
  bash-completion \
  cmake \
  gdb \
  && rm -rf /var/lib/apt/lists/*
# Fix gdb
RUN mv /usr/bin/gdb /usr/bin/gdborig
RUN echo '#!/bin/sh\nsudo gdborig $@' > /usr/bin/gdb
RUN chmod 0755 /usr/bin/gdb
# Install fish
RUN echo 'deb http://download.opensuse.org/repositories/shells:/fish:/release:/3/Debian_12/ /' | tee /etc/apt/sources.list.d/shells:fish:release:3.list
RUN curl -fsSL https://download.opensuse.org/repositories/shells:fish:release:3/Debian_12/Release.key | gpg --dearmor | tee /etc/apt/trusted.gpg.d/shells_fish_release_3.gpg > /dev/null
RUN apt-get update \
  && apt-get install fish -y \
  && rm -rf /var/lib/apt/lists/*
SHELL ["fish", "--command"]
RUN chsh -s /usr/bin/fish
# Add user
ARG NEW_USER
RUN useradd -ms /usr/bin/fish $NEW_USER
RUN usermod -aG sudo $NEW_USER
RUN passwd -d $NEW_USER
USER $NEW_USER
# Install fisher
RUN curl -sL https://raw.githubusercontent.com/jorgebucaran/fisher/main/functions/fisher.fish | source && fisher install jorgebucaran/fisher

WORKDIR /home/$NEW_USER
ENTRYPOINT ["/usr/bin/fish"]
