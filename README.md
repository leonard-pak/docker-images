# Docker Images

## Buld Dockerfile
```bash
docker build -f docker/***.Dockerfile .
docker build -f docker/***.Dockerfile --build-arg NEW_USER=leonard -t *** .
```

# Run deteached container with volume and host network
```bash
docker run -d -it -v /source:/dest \
              --network host \
              --name my-container ***
```

# Run container with GUI support
```bash
docker run -it -v /tmp/.X11-unix:/tmp/.X11-unix -v /mnt/wslg:/mnt/wslg \
                      -e DISPLAY=$DISPLAY -e WAYLAND_DISPLAY=$WAYLAND_DISPLAY \
                      -e XDG_RUNTIME_DIR=$XDG_RUNTIME_DIR -e PULSE_SERVER=$PULSE_SERVER --name my-container ***
```

## Run docker compose
```bash
docker compose up
```
