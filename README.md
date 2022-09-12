# Docker Images

## For build
```bash
docker build --no-cache -t leonard2000/name:tag .
```

## With docker compose
```bash
docker-compose build --no-cache
docker-compose -p name up -d  
```

## For win10 with X-server
```docker
ENV DISPLAY=host.docker.internal:0.0
```
