# Docker Images

## Run Dockerfile
```bash
docker build -f docker/***.Dockerfile .
```

## Run docker compose
```bash
docker compose up
```

# Configuration
## For Dockerfile
```dockerfile
# Add user and timezone
ENV USER=$(whoami)
ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
```
```dockerfile
# Install tools
RUN apt-get update \
  && apt-get install -y \
  wget \
  git \
  neovim \
  tree \
  curl \
  && rm -rf /var/lib/apt/lists/*
```
```dockerfile
# Install workspace
RUN git clone https://github.com/MrLeonardPak/workspace-setup.git $HOME/workspace-setup
RUN $HOME/workspace-setup/setup-docker.bash
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
