![alt text](https://gitlab.com/ouzb64ty/supstream/-/raw/main/images/supstream-logo.png)

*Gstreamer-based stream supervisor using YAML*

📞 [Discord support](https://discord.gg/ARQPKGdvrc) 24h/24 - 7j/7

# 🇺🇸 Introduction (in US)

Supstream is an open-source project for deployment and development of dynamic streaming pipelines. This project uses the YAML format to configure different pipelines for easy use between different parts of an architecture. It is a "dev-ops" tool for setting up streaming pipelines, saving a significant amount of development time. Supstream allows you to avoid changing versions of Gstreamer in a secure and containerized context under Linux. The particularity of Supstream is to centralize a set of streams, which is not negligible in the case of a microservice architecture that may require supervision, monitoring and in-depth management of streams. Supstream integrates a REST API (APIGateway) in Python/uvicorn using APIFast, for the moment it is a POC. In general, this project is a "Virtual Machine" performing a set of operations supposed to standardize the implementation of audio & video streaming architecture. At the moment, no open-source project like this one exists. On the other hand, there is another open-source project slightly similar to Supstream called "RidgeRun/gstd-1.x" for Gstreamer Daemon, developed by the company RidgeRun which supports Gstreamer. It should be noted that this project integrates incoming pipelines in the CLI format of Gstreamer, "gst-launch", on the contrary, Supstream uses directly the low level functions of the Gstreamer library allowing to extend and improve natively the control on each of them.

The Supstream executable is as fast as the "gst-launch" command, in this case Supstream uses YAML and launches a ZeroMQ listening server (IPC or TCP/IP) for supervision and monitoring. Technically, the parsing of this project is relatively well developed from a lexical, syntactic and semantic analysis using abstract binary trees for all parts related to interpretation, for example configurations. In particular, hash tables are integrated in this project to allow sharing of some Gstreamer data.

In general, Supstream for SUPervisor STREAM, integrates :
- Caps management
- Setting properties
- Pads/Elements (un)link
- 30 pre-configured samples
- Pipeline supervision by priority order
- Supervision of pipelines by execution type (synchronized/threaded)
- Logs
- Pipeline state initialization
- Generation of graphs to visualize the pipelines launched by Gstreamer
- Management of pipelines through schedulers and delays
- Timezone setting
- A docker that launches a Gstreamer compilation via Cerbero and Ninja, only in a development environment for the moment
- Auto restart of pipelines
- A DOM loaded in memory and updated at each modifications
- Enable or disable IPC API

The real-time API ([ouzb64ty/mxsupstream](https://gitlab.com/ouzb64ty/mxsupstream)), allows to *(🚧 Under development)* :
- Real-time control states of pipelines (play, pause, null and ready)
- Obtaining the version
- Obtaining the current "DOM" containing the elements, pipelines, configuration...
- Unlink/link elements
- Creating and deleting elements
- Set pads and elements properties
- Set caps
- Exit Supstream

Prefer to use the docker container for the moment, it can take a few hours because it compiles all the Gstreamer sources from their sources.
Currently, the project is under development. To better understand the interest of this project, you can take a look at the _samples_ directory from this repository.
A [Trello](https://trello.com/b/gAa0tKvO/supstream) is available, it contains all the current tasks, i.e. those under development.

Prefer to use the [ouzb64ty/supstream-deploy](https://gitlab.com/ouzb64ty/supstream-deploy) repository for a simple docker installation.

# 🇫🇷 Introduction (in French)

Supstream est un projet open-source de déploiement et de développement de pipelines de streaming dynamique. Ce projet utilise le format YAML pour configurer les différentes pipelines pour un usage aisé entres plusieurs parties d'une architecture. Il est un outil «dev-ops» pour la mise en place de pipelines de streaming, épargnant un temps de développement conséquent, Supstream permet de s'affranchir du changement de version de Gstreamer dans un contexte sécurisé et conteneurisé sous Linux. La particularité de Supstream est de pouvoir centraliser un ensemble de flux, ce qui n’est pas négligeable dans le cas d'une architecture microservice pouvant nécéssiter d‘une supervision, d’un monitoring et d’une gestion approfondie des streams. Supstream intégre une API REST (APIGateway) en Python/uvicorn utilisant APIFast, pour le moment c'est un POC. En général, ce projet est une «Virtual Machine» éxécutant un ensemble d’opérations censées uniformiser la mise en place d’architecture de streaming audio & vidéo. A l’heure actuelle, aucuns projets open-source comme celui ci n’existe. En revanche, il existe un autre projet open-source légèrement similaire à Supstream nommé «RidgeRun/gstd-1.x» pour Gstreamer Daemon, développé par la société RidgeRun qui fait du support Gstreamer. Il faut savoir que ce projet intégre des pipelines entrantes sous le format CLI de Gstreamer, «gst-launch», à contrario, Supstream utilise directement les fonctions bas niveau de la librairie Gstreamer permettant d’élargir et d’améliorer nativement le contrôle sur chaqu’unes d’elles.

L’éxécutable Supstream est aussi rapide que la commande «gst-launch», en l’occurrence Supstream utilise du YAML et lance un serveur d’écoute ZeroMQ (IPC ou TCP/IP) pour la supervision et le monitoring. Techniquement, le parsing de ce projet est relativement bien développé à partir d'une analyse lexicale, syntaxique et sémantique en utilisant des arbres binaires abstraits pour toutes les parties en rapport avec l’interprétation, par exemple des configurations. Des tables de hash sont notamment intégrées dans ce projet pour permettre un partage de certaines données Gstreamer.

De maniére générale, **Supstream pour SUPervisor STREAM**, intégre :
- La gestion des caps
- L'attribution des propriétés
- Le linkage des pads et des éléments
- Un repertoire de 30 samples pré-configurés
- une supervision des pipelines par ordre de priorité
- une supervision des pipelines par type d'éxécution (synchronisé/threadé)
- la sauvegarde des logs dans un fichier
- l'initialisation de l'état des pipelines
- la génération des graphs pour visualiser les pipelines lancées par Gstreamer
- la gestion des pipelines à travers des schedulers et des delays
- l'édition de la timezone
- un docker qui lance une compilation Gstreamer via Cerbero et Ninja, uniquement dans un environnement de développement pour le moment
- l'auto restart des pipelines
- Un "DOM" chargé en mémoire et mis à jour lors de chaques modifications
- l'activation d'une API IPC pour modifier en temps-réel les pipelines

L'API temps-réel ([ouzb64ty/mxsupstream](https://gitlab.com/ouzb64ty/mxsupstream)), permet *(🚧 En cours de développement)* :
- Le control des états en temps-réel des pipelines (play, pause, null et ready)
- L'obtention de la version
- L'obtention de l'actuel "DOM" contenant les éléments, les pipelines, la configuration..
- Le relinkage des éléments
- La création et la suppression des éléments
- La modification des propriétés d'éléments et de pads
- La modification des caps
- La fermeture du programme Supstream

Préférez utiliser le conteneur docker pour le moment, cela peut durer quelques heures car il compile toutes les sources Gstreamer depuis leurs sources.
Actuellement, le projet est en cours de développement. Pour mieux comprendre l'intérêt de ce projet, vous pouvez jeter un œil sur le dossier _samples_ du répertoire.
Un [Trello](https://trello.com/b/gAa0tKvO/supstream) est accessible, il contient l'ensembles des tâches courantes, c.à.d, en cours de développement.

Préférez utiliser le répertoire [ouzb64ty/supstream-deploy](https://gitlab.com/ouzb64ty/supstream-deploy) pour une installation simple via un conteneur docker.

# 🇫🇷 Installation (in french)

⚠️ Il est préférable de builder le projet depuis son Docker ou d'installer la dernière version de Gstreamer, Supstream est censé s'affranchir du problème de version en installant automatiquement l'une des dernières version de Gstreamer.

## Compilation
### Ubuntu 21.10

1. Vous devez installer Gstreamer via Cerbero (https://gstreamer.freedesktop.org/documentation/installing/index.html?gi-language=c)
2. `apt install libyaml-dev && apt-install libczmq-dev && libcjson-dev`
3. `git clone https://gitlab.com/ouzb64ty/supstream.git supstream && cd supstream/Supstream && mkdir build && cd build && cmake .. && make`

## Docker

Ce projet contient un Dockerfile pour un mode de développement simple temporairement basé sur un conteneur ubuntu:latest.

- Installation des dépendances Gstreamer
- Compilation Gstreamer avec meson et ninja
- Compilation des plugins Gstreamer
- Configuration VIM et ZSH
- Installation des dépendances de l'API Gateway

1. Builder le Docker Supstream

```bash
docker build . -t docker-gstreamer-compiler
```

2. Faire un export display

```bash
xhost +
```

3. Lancer un docker Supstream

```bash
docker run --ipc=host -v=/tmp:/tmp --rm -it --device /dev/video0 --net host -e DISPLAY=$DISPLAY --device /dev/snd supstream
```

Pour information, _--device /dev/video0_ permet l'utilisation de l'élément v4l2src pour la webcam, tandis que _--device /dev/snd_ permet l'utilisation de l'audio.

### Exemples

- La commande :
```bash
./bin/supstream -f samples/matroska_video_0.yaml
```

- Basé sur la configuration YAML:
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

Cette configuration vous permet d'afficher uniquement la vidéo du conteneur MKV à partir de l'élément demuxer matroskademux.

- La commande:
```bash
./bin/supstream -f samples/mosaic.yaml
```

- Basé sur la configuration YAML:
```yaml
##############
# GST LAUNCH #
##############

# gst-launch-1.0 -e videomixer name=mix \                                               
# sink_0::xpos=0 sink_0::ypos=0 \
# sink_1::xpos=10 sink_1::ypos=10 \
# sink_2::xpos=320 sink_2::ypos=10 \
# sink_3::xpos=10 sink_3::ypos=180 \
# sink_4::xpos=320 sink_4::ypos=180 \
# ! videoconvert ! xvimagesink \
# videotestsrc pattern=snow ! video/x-raw,width=650,height=370 ! mix.sink_0 \
# videotestsrc ! videoconvert ! videoscale ! video/x-raw,width=320,height=180 ! mix.sink_1 \
# videotestsrc ! videoconvert ! videoscale ! video/x-raw,width=320,height=180 ! mix.sink_2 \
# videotestsrc ! videoconvert ! videoscale ! video/x-raw,width=320,height=180 ! mix.sink_3 \
# videotestsrc ! videoconvert ! videoscale ! video/x-raw,width=320,height=180 ! mix.sink_4

##########
# CONFIG #
##########

supstream:
    zmq_path: "tcp://0.0.0.0:5557"
    timezone: "Europe/Paris"

#############
# PIPELINES #
#############

pipelines:

    ##################
    # VIDEO PIPELINE #
    ##################

    mosaic_video:

        init_state: play
        type_exec: sync

        elements:

            #########
            # MIXER #
            #########

            videomixer:

                element: videomixer
                element_link: videoconvert
                pad_props:

                    ##############
                    # 4/4 MOSAIC #
                    ##############

                    panel:
                        pad: sink_0
                        properties:
                            xpos: 0
                            ypos: 0

                    top_left:
                        pad: sink_1
                        properties:
                            xpos: 10
                            ypos: 10

                    top_right:
                        pad: sink_2
                        properties:
                            xpos: 320
                            ypos: 10

                    bottom_left:
                        pad: sink_3
                        properties:
                            xpos: 10
                            ypos: 180

                    bottom_right:
                        pad: sink_4
                        properties:
                            xpos: 320
                            ypos: 180


            videoconvert:

                element: videoconvert
                element_link: xvimagesink

            xvimagesink:
                
                element: xvimagesink

            #########
            # PANEL #
            #########

            videotestsrc_panel:

                element: videotestsrc
                properties:
                    pattern: 1
                caps:
                    media_type: video/x-raw
                    width: 650
                    height: 370
                element_link: videoconvert_panel

            videoconvert_panel:

                element: videoconvert
                pad_link:

                    link_panel:
                        from: src
                        to:
                            name: videomixer
                            pad: sink_0



            ################
            # TOP LEFT 1/4 #
            ################
            
            videotestsrc_top_left:

                element: videotestsrc
                properties:
                    pattern: 0
                element_link: videoconvert_top_left

            videoconvert_top_left:

                element: videoconvert
                element_link: videoscale_top_left
                caps:
                    media_type: video/x-raw
                    width: 320
                    height: 180
                    framerate: "30000/1001"

            videoscale_top_left:

                element: videoscale
                pad_link:

                    link_top_left:
                        from: src
                        to:
                            name: videomixer
                            pad: sink_1


            #################
            # TOP RIGHT 2/4 #
            #################

            videotestsrc_top_right:

                element: videotestsrc
                properties:
                    pattern: 0
                element_link: videoconvert_top_right

            videoconvert_top_right:

                element: videoconvert
                element_link: videoscale_top_right
                caps:
                    media_type: video/x-raw
                    width: 320
                    height: 180

            videoscale_top_right:

                element: videoscale
                pad_link:

                    link_top_right:
                        from: src
                        to:
                            name: videomixer
                            pad: sink_2


            ###################
            # BOTTOM LEFT 3/4 #
            ###################

            videotestsrc_bottom_left:

                element: videotestsrc
                element_link: videoconvert_bottom_left
                properties:
                    pattern: 0

            videoconvert_bottom_left:

                element: videoconvert
                element_link: videoscale_bottom_left
                caps:
                    media_type: video/x-raw
                    width: 320
                    height: 180

            videoscale_bottom_left:

                element: videoscale
                pad_link:

                    link_bottom_left:
                        from: src
                        to:
                            name: videomixer
                            pad: sink_3


            ####################
            # BOTTOM RIGHT 4/4 #
            ####################

            videotestsrc_bottom_right:

                element: videotestsrc
                element_link: videoconvert_bottom_right
                properties:
                    pattern: 0

            videoconvert_bottom_right:

                element: videoconvert
                element_link: videoscale_bottom_right
                caps:
                    media_type: video/x-raw
                    width: 320
                    height: 180

            videoscale_bottom_right:

                element: videoscale
                pad_link:

                    link_bottom_right:
                        from: src
                        to:
                            name: videomixer
                            pad: sink_4

```

Cette configuration vous permet d'afficher facilement une mosaïque.

- La commande:

```bash
./bin/supstream -f samples/gstdarknet_yolov4.yaml
```

- Basé sur cette configutation YAML:
```yaml
supstream:
  zmq_path: "tcp://0.0.0.0:5557"
  timezone: "Europe/Paris"

pipelines:
    
  road_detection:

    init_state: play
    type_exec: thread
    set_delay: "2022-04-19 12:17:20"

    elements:
      
      filesrc:
        element: filesrc
        properties:
          location: /tmp/test.mp4
        element_link: qtdemux

      qtdemux:
        element: qtdemux
        caps:
          media_type: video/x-h264
        pad_link:
          pad_video_3:
            from: video_0
            to:
              name: h264parse
              pad: sink

      h264parse:
        element: h264parse
        element_link: avdec_h264

      avdec_h264:
        element: avdec_h264
        element_link: videoconvert

      videoconvert:
        element: videoconvert
        element_link: darknetinfer
    
      darknetinfer:
        element: darknetinfer
        properties:
          config: /tmp/yolov4.cfg
          weights: /tmp/yolov4.weights
        element_link: darknetrender

      darknetrender:
        element: darknetrender
        properties:
          labels: /tmp/coco.names
        element_link: videoconvert3

      videoconvert3:
        element: videoconvert
        element_link: autovideosink

      autovideosink:
        element: autovideosink
```

Cette configuration permet d'appliquer un algorithme de détection d'objets à partir d'un flux vidéo en utilisant **darknet YOLOv4** à partir d'un délai déterminé soit **2022-04-19 12:17:20**.

# 🇺🇸 ZeroMQ API Documentation (in US)

This textual documentation describes how _supstream_ externally talk with IPC ZeroMQ

- codes :
    - 0 success
    - 1 failure

The default ZMQ uri for IPC/TCP sharing is _ipc:///tmp/supstream.pipe_, you can update your own ZMQ path with _zmq_path_ property.

### Play

Update state to PLAYING for a specific pipeline or rather all

```json
{
    "command": "play",
    "pipelines": ["matroska-video_0"]
}
```

### Pause

Update state to PAUSED for a specific pipeline or rather all

```json
{
    "command": "pause",
    "pipelines": ["matroska-video_0"]
}
```

### Ready

Update state to READY for a specific pipeline or rather all

```json
{
    "command": "ready",
    "pipelines": ["matroska-video_0"]
}
```

### Null

Update state to NULL for a specific pipeline or rather all

```json
{
    "command": "null",
    "pipelines": ["matroska-video_0"]
}
```

### Version

```json
{
    "command": "version"
}
```

### Exit

This request exit a supstream process

```json
{
    "command": "exit"
}
```

### Show

Used to get general pipelines configuration, saved in cache

```json
{
    "command": "show"
}
```

### Unlink element

```json
{
    "command": "unlink_element",
    "pipeline": "matroska-video_0",
    "src": "videoconvert",
    "sink": "autovideosink"
}
```

### Link element

```json
{
    "command": "link_element",
    "pipeline": "matroska-video_0",
    "src": "videoconvert",
    "sink": "autovideosink"
}
```

### Create element

```json
{
    "command": "create_element",
    "pipeline": "matroska-video_0",
    "element": {
    }
}
```

### Set properties

```json
{
    "command": "set_properties",
    "pipeline": "matroska-video_0",
    "element": "videoconvert",
    "properties": {
    }
}
```

### Set pad properties

```json
{
    "command": "set_properties",
    "pipeline": "matroska-video_0",
    "element": "videoconvert",
    "pad": "video_0",
    "properties": {
    }
}
```

### Set caps

```json
{
    "command": "set_caps",
    "pipeline": "matroska-video_0",
    "element": "videoconvert",
    "caps": {
    }
}
```

# Resources

- Gitlab project repository : https://gitlab.com/ouzb64ty/supstream
- Github project repository (mirror) : https://github.com/theophpo/supstream
- Old gitlab repository : https://gitlab.com/ouzb64ty/supstream-old-repository
- Gstreamer website : https://gstreamer.freedesktop.org/
- Trello task's : https://trello.com/b/gAa0tKvO/supstream
- RidgeRun/gstd-1.x on github : https://github.com/RidgeRun/gstd-1.x
- CYIM website : https://www.cyim.com/fr

# Autor

tpoac <t.poac@cyim.com>

