FROM python:3.10-alpine
# Install tenserboard
RUN pip install tensorboard
# Run tenserboard
ENTRYPOINT [ "tensorboard","--logdir=/logdir", "--host", "0.0.0.0"]
