
# HELP

## Images

- osrf/ros:humble-desktop-full

## Dockerfile

### C++

```dockerfile
ARG BOOST_VERSION

# Add Toolchain PPA
RUN apt-get update && apt-get install -y \
    software-properties-common && \
    add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
    rm -rf /var/lib/apt/lists/*
# Install latest GCC
RUN apt-get update && apt-get install -y \
    gcc-13 \
    g++-13 && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 100 \
    --slave /usr/bin/g++ g++ /usr/bin/g++-13 \
    --slave /usr/bin/gcov gcov /usr/bin/gcov-13 && \
    rm -rf /var/lib/apt/lists/*
# Install Boost. From https://leimao.github.io/blog/Boost-Docker/
RUN cd /tmp && \
    BOOST_VERSION_MOD=$(echo $BOOST_VERSION | tr . _) && \
    wget https://archives.boost.io/release/${BOOST_VERSION}/source/boost_${BOOST_VERSION_MOD}.tar.bz2 && \
    tar --bzip2 -xf boost_${BOOST_VERSION_MOD}.tar.bz2 && \
    cd boost_${BOOST_VERSION_MOD} && \
    ./bootstrap.sh --prefix=/usr/local && \
    ./b2 install && \
    rm -rf /tmp/*
# Install latest CLANG
RUN bash -c "$(curl -fsSL https://apt.llvm.org/llvm.sh)"
RUN apt-get update && apt-get install -y \
    clang-tidy-20 \
    clang-format-20 && \
    update-alternatives --install /usr/bin/clang clang /usr/bin/clang-20 100 \
    --slave /usr/bin/clangd clangd /usr/bin/clangd-20 \
    --slave /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-20 \
    --slave /usr/bin/clang-format clang-format /usr/bin/clang-format-20 && \
    rm -rf /var/lib/apt/lists/*
```

### Python

```dockerfile
# Install Ruff
RUN apt-get update && apt-get install -y \
    python3-pip && \
    rm -rf /var/lib/apt/lists/*
RUN pip install ruff
# Install python modules
RUN pip install click ipykernel ipython
```

## Docker Compose

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
