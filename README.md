# Docker Images

## Run Dockerfile
```bash
docker build -f docker/***.Dockerfile .
docker build -f docker/***.Dockerfile --build-arg NEW_USER=leonard -t *** .
```

# Run container with GUI support
```bash
docker run -it -v /tmp/.X11-unix:/tmp/.X11-unix -v /mnt/wslg:/mnt/wslg \
                      -e DISPLAY=$DISPLAY -e WAYLAND_DISPLAY=$WAYLAND_DISPLAY \
                      -e XDG_RUNTIME_DIR=$XDG_RUNTIME_DIR -e PULSE_SERVER=$PULSE_SERVER ***
```

## Run docker compose
```bash
docker compose up
```
