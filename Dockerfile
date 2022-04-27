FROM ubuntu:latest

# Config
ARG DEBIAN_FRONTEND=noninteractive
ARG CACHE_DATE=2022-02-28

# Workspace
RUN mkdir app/
WORKDIR app/

# Install deps
RUN apt-get --assume-yes update
RUN apt-get --assume-yes install python3 \
    python3-distro \
    autotools-dev \
    automake \
    autoconf \
    libtool \
    g++ \
    autopoint \
    make \
    cmake \
    bison \
    flex \
    nasm \
    libxv-dev \
    libx11-dev \
    libx11-xcb-dev \
    libpulse-dev \
    python3-dev \
    gettext \
    build-essential \
    pkg-config \
    libxext-dev \
    libxi-dev \
    x11proto-record-dev \
    libxrender-dev \
    libgl1-mesa-dev \
    libxfixes-dev \
    libxdamage-dev \
    libxcomposite-dev \
    libasound2-dev \
    gperf \
    wget \
    libxtst-dev \
    libxrandr-dev \
    libglu1-mesa-dev \
    libegl1-mesa-dev \
    git \
    xutils-dev \
    intltool \
    ccache \
    python3-setuptools \
    libssl-dev \
    sudo \
    make \
    glib-networking \
    meson \
    libunwind-dev \
    ffmpeg \
    v4l2loopback-dkms \
    libmbedtls-dev \
    libavcodec-dev \
    libavdevice-dev \
    libavfilter-dev \
    libavformat-dev \
    libavutil-dev \
    libcurl4-openssl-dev \
    libfdk-aac-dev \
    libfontconfig-dev \
    libfreetype6-dev \
    libglvnd-dev \
    libjack-jackd2-dev \
    libjansson-dev \
    libluajit-5.1-dev \
    libqt5x11extras5-dev \
    libspeexdsp-dev \
    libswresample-dev \
    libswscale-dev \
    libudev-dev \
    libv4l-dev \
    libvlc-dev \
    libwayland-dev \
    libx264-dev \
    libxcb-shm0-dev \
    libxcb-xinerama0-dev \
    libxinerama-dev \
    qtbase5-dev \
    qtbase5-private-dev \
    libqt5svg5-dev \
    swig \
    libxcb-randr0-dev \
    libxcb-xfixes0-dev \
    libxcb1-dev \
    libxss-dev \
    qtwayland5 \
    libgles2-mesa \
    libgles2-mesa-dev \
    fish \
    libyaml-dev \
    libevent-dev \
    libczmq-dev \
    libcjson-dev \
    vim \
    curl \
    figlet \
    fbi \
    graphviz

# Env
ENV GSTVERSION gstreamer-1.0
ENV GST_DEBUG_DUMP_DOT_DIR=/app/pipelines
# (Don't use master/main tag name)
ENV GST_PLUGIN_PATH="/usr/local"
ENV GSTTAG 1.20.0

# Download cerbero
RUN git clone https://gitlab.freedesktop.org/gstreamer/cerbero.git \
    && cd cerbero \
    && git checkout ${GSTTAG} \
    && sudo ./cerbero-uninstalled bootstrap \
    && sudo ./cerbero-uninstalled package $GSTVERSION

# Install cerbero devel
RUN cd cerbero \
    && mkdir -p /share \
    && mkdir -p /include \
    && mkdir devel \
    && tar -xvf ${GSTVERSION}-linux-x86_64-${GSTTAG}-devel.tar.xz -C devel \
    && cp -R devel/bin/* /bin \
    && cp -R devel/include/* /include \
    && cp -R devel/lib/* /lib \
    && cp -R devel/share/* /share \
    && rm -rf ${GSTVERSION}-linux-x86_64-${GSTTAG}-devel.tar.xz \
    && rm -rf devel

# Install cerbero global
RUN cd cerbero \
    && mkdir -p /libexec \
    && mkdir global \
    && tar -xvf ${GSTVERSION}-linux-x86_64-${GSTTAG}.tar.xz -C global \
    && cp -R global/bin/* /bin \
    && cp -R global/etc/* /etc \
    && cp -R global/lib/* /lib \
    && cp -R global/libexec/* /libexec \
    && cp -R global/share/* /share \
    && rm -rf ${GSTVERSION}-linux-x86_64-${GSTTAG}.tar.xz \
    && rm -rf global

# Cerbero remove useless sources
RUN cd cerbero \
    && ls | grep -v build | xargs rm -rf \
    && cd build \
    && ls | grep -v dist | xargs rm -rf

# Download gst-template
RUN git clone https://gitlab.com/ouzb64ty/gstreamer-plugin.git gstreamer-plugin \
    && cd gstreamer-plugin \
    && meson builddir \
    && ninja -C builddir install

# Supstream
RUN git clone https://gitlab.com/ouzb64ty/supstream.git

# Dev part
RUN git clone --depth=1 https://github.com/amix/vimrc.git ~/.vim_runtime
RUN sh ~/.vim_runtime/install_awesome_vimrc.sh

# Gstreamer dotfiles path
RUN mkdir -p pipelines

# For main branch
RUN cd supstream \
    && git fetch origin main \
    && git checkout main

# Starter
CMD cd supstream \
    && cd Supstream \
    && git pull \
    && mkdir build \
    && cd build \
    && cmake .. \
    && make \
    && cd .. \
    && figlet "Supstream Docker Dev" \
    && fish
