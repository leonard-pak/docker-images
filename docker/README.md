
# Configuration
## For Dockerfile
```dockerfile
# Add user and timezone
ENV USER=$(whoami)
ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
```
```dockerfile
# Update & Upgrade
RUN apt-get update \
  && apt-get upgrade -y \
  && rm -rf /var/lib/apt/lists/*
```
```dockerfile
# Install tools
RUN apt-get update \
  && apt-get install -y \
  curl \
  wget \
  unzip\
  git \
  neovim \
  tree \
  && rm -rf /var/lib/apt/lists/*
```
```dockerfile
# Install workspace
RUN git clone https://github.com/MrLeonardPak/workspace-setup.git $HOME/workspace-setup
RUN $HOME/workspace-setup/setup-docker.bash
```
```dockerfile
# Install fish
RUN 'deb http://download.opensuse.org/repositories/shells:/fish:/release:/3/Debian_12/ /' | tee /etc/apt/sources.list.d/shells:fish:release:3.list
RUN curl -fsSL https://download.opensuse.org/repositories/shells:fish:release:3/Debian_12/Release.key | gpg --dearmor | tee /etc/apt/trusted.gpg.d/shells_fish_release_3.gpg > /dev/null
RUN apt-get update \
  && apt-get install fish -y \
  && rm -rf /var/lib/apt/lists/*
SHELL ["fish", "--command"]
RUN chsh -s /usr/bin/fish
```
```dockerfile
# Add user
ARG NEW_USER
RUN useradd -ms /usr/local/bin/fish $NEW_USER
RUN usermod -aG sudo $NEW_USER
RUN passwd -d $NEW_USER
USER $NEW_USER
```
```dockerfile
# Add display if x-server used
ENV DISPLAY=host.docker.internal:0.0
```
## For docker compose
```yaml
    # Containter name
    container_name: ***
```
```yaml
    # Add build
    build:
      context: ./docker
      dockerfile: ***.Dockerfile
```
```yaml
    # Add interactive session
    tty: true
```
```yaml
    # Add gpu support
    deploy:
      resources:
        reservations:
          devices:
            - driver: nvidia
              count: all
              capabilities: [ gpu ]
```
```yaml
    # Config for wslg bridge
    environment:
      - DISPLAY=$DISPLAY
      - WAYLAND_DISPLAY=$WAYLAND_DISPLAY
      - XDG_RUNTIME_DIR=$XDG_RUNTIME_DIR
      - PULSE_SERVER=$PULSE_SERVER
    volumes:
      - /tmp/.X11-unix:/tmp/.X11-unix
      - /mnt/wslg:/mnt/wslg
```
