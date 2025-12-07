
# Configuration
## For Dockerfile
```dockerfile
# Add timezone
ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
```
```dockerfile
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
# Add user
ARG NEW_USER
RUN useradd -ms /bin/bash $NEW_USER
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
