
![alt text](https://gitlab.com/ouzb64ty/supstream/-/raw/main/Supstream/images/supstream-logo.png)

Gstreamer-based stream supervisor using YAML

**Project is under development, looking forward to getting help to enrich the project.**

To better understand, look at the _samples_ directory

Trello : https://trello.com/b/gAa0tKvO/supstream

![alt text](https://gitlab.com/ouzb64ty/supstream/-/raw/main/Supstream/images/screenshot.png)

# Install

## Compilation

### Ubuntu 21.10

1. You need to install gstreamer before (https://gstreamer.freedesktop.org/documentation/installing/index.html?gi-language=c)
2. type `apt install libyaml-dev && apt-install libczmq-dev && libcjson-dev`
3. type `git clone https://gitlab.com/ouzb64ty/supstream.git supstream && cd supstream && make`

## Docker

This project contains a Dockerfile for easy development mode based on latest Ubuntu.

- Automatic dependancies installations
- Gstreamer compilation using meson and ninja
- Gstreamer plugins compilations
- Vim and Zsh configuration

## Building

```bash
docker build . -t docker-gstreamer-compiler
```

## Running

```bash
xhost +
docker run --ipc=host -v=/tmp:/tmp --rm -it --device /dev/video0 --net host -e DISPLAY=$DISPLAY --device /dev/snd supstream
```

/tmp/tmp to getting visual pipeline graph

# Running

Just type `./bin/supstream samples/matroska_video_0.yaml`

```yaml
pipelines:

    matroska_video_0:

        init_state: play
        type_exec: sync

        elements:
            souphttpsrc:
                element: souphttpsrc
                properties:
                    location: https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm
                element_link: matroskademux
            matroskademux:
                element: matroskademux
                pad_link:
                    pad_video:
                        from: video_0
                        to:
                            name: vp8dec
                            pad: sink
            vp8dec:
                element: vp8dec
                element_link: videoconvert
            videoconvert:
                element: videoconvert
                element_link: autovideosink
            autovideosink:
                element: autovideosink
```
This configuration allows you to easily display a WEBM video from an MKV demuxer

# ZeroMQ API

This textual documentation describes how _supstream_ externally talk with IPC ZeroMQ

- codes :
    - 0 success
    - 1 failure

The default ZMQ uri for IPC/TCP sharing is _ipc:///tmp/supstream.pipe_, you can update your own ZMQ path with _zmq_path_ property.

## Requests

Mini pygame client available in https://gitlab.com/ouzb64ty/supstream-gateway

### Play

Update state to PLAYING for a specific pipeline or rather all

```json
{
    command: "play",
    pipelines: ["matroska-video_0"]
}
```

### Pause

Update state to PAUSED for a specific pipeline or rather all

```json
{
    command: "pause",
    pipelines: ["matroska-video_0"]
}
```

### Version

```json
{
    command: "version"
}
```

### Exit

This request exit a supstream process

```json
{
    command: "exit"
}
```

### Show

Used to get general pipelines configuration, saved in cache

```json
{
    command: "show"
}
```

# Autor

tpoac <t.poac@cyim.com>
