FROM ros:noetic

# install ros packages
RUN apt-get update && apt-get install -y \
    ros-noetic-desktop-full=1.5.0-1* \
    ros-noetic-moveit \
    && rm -rf /var/lib/apt/lists/*

ENV DISPLAY=host.docker.internal:0.0
