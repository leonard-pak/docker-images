FROM nvidia/cuda:12.2.0-base-ubuntu22.04
# Add user and timezone
ENV USER=$(whoami)
ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
# Install tools
RUN apt-get update \
  && apt-get install -y \
  wget \
  git \
  neovim \
  tree \
  curl \
  python3 \
  python3-pip \
  && rm -rf /var/lib/apt/lists/*
# Update pip
RUN python3 -m pip install --upgrade pip
# Fix github certificate error
RUN openssl s_client -showcerts -servername github.com -connect github.com:443 </dev/null 2>/dev/null | sed -n -e '/BEGIN\ CERTIFICATE/,/END\ CERTIFICATE/ p'  > github-com.pem
RUN cat github-com.pem | tee -a /etc/ssl/certs/ca-certificates.crt
# Install pytorch
RUN pip install torch==1.11.0+cu113 --extra-index-url https://download.pytorch.org/whl/cu113
# Install ml-agent
RUN git clone --depth 1 https://github.com/Unity-Technologies/ml-agents
RUN pip install -e /ml-agents/ml-agents-envs
RUN pip install -e /ml-agents/ml-agents
# Install workspace
RUN git clone https://github.com/MrLeonardPak/workspace-setup.git $HOME/workspace-setup
RUN $HOME/workspace-setup/setup-docker.bash
# Run tools
WORKDIR /ml-agents
ENTRYPOINT ["mlagents-learn"]
