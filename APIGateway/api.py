import zmq
import json
from fastapi import FastAPI, Request

# Config

app = FastAPI()
context = zmq.Context()
ZMQ_IPC_PATH = "tcp://127.0.0.1:5557" # Use TCP in tmp
print("Connecting to supstream ZeroMQ server...")
socket = context.socket(zmq.REQ)
socket.connect(ZMQ_IPC_PATH)

# API

@app.get("/v1/version")
async def version():

    request_version = """{
        "command": "version"
    }"""


    print("> Supstream version requested")
    socket.send_string(request_version)
    message = socket.recv()
    load_json = json.loads(message)

    return load_json

@app.get("/v1/pause")
async def pause(pipeline: str = None):

    if pipeline is None:
        request_pause = """{
            "command": "pause"
        }"""
    else:
        request_pause = """{
            "command": "pause",
            "pipeline": \"""" + pipeline + """\"
        }"""

    print("> Pause")
    socket.send_string(request_pause)
    message = socket.recv()
    load_json = json.loads(message)

    return load_json

@app.get("/v1/play")
async def play(pipeline: str = None):

    if pipeline is None:
        request_play = """{
            "command": "play"
        }"""
    else:
        request_play = """{
            "command": "play",
            "pipeline": \"""" + pipeline + """\"
        }"""

    print("> Play")
    socket.send_string(request_play)
    message = socket.recv()
    load_json = json.loads(message)

    return load_json

@app.get("/v1/ready")
async def ready(pipeline: str = None):

    if pipeline is None:
        request_ready = """{
            "command": "ready"
        }"""
    else:
        request_ready = """{
            "command": "ready",
            "pipeline": \"""" + pipeline + """\"
        }"""

    print("> Ready")
    socket.send_string(request_ready)
    message = socket.recv()
    load_json = json.loads(message)

    return load_json

@app.get("/v1/null")
async def null(pipeline: str = None):

    if pipeline is None:
        request_null = """{
            "command": "null"
        }"""
    else:
        request_null = """{
            "command": "null",
            "pipeline": \"""" + pipeline + """\"
        }"""

    print("> Null")
    socket.send_string(request_null)
    message = socket.recv()
    load_json = json.loads(message)

    return load_json

@app.get("/v1/link_elements")
async def link_elements(pipeline: str = None, src: str = None, sink: str = None):

    if not pipeline or not src or not sink:
        return None;

    request_link_elements = """{
        "command": "link_elements",
        "pipeline": \"""" + pipeline + """\",
        "src": \"""" + src + """\",
        "sink": \"""" + sink + """\"
    }"""


    print("> Link elements")
    socket.send_string(request_link_elements)
    message = socket.recv()
    load_json = json.loads(message)

    return load_json

@app.get("/v1/unlink_elements")
async def unlink_elements(pipeline: str = None, src: str = None, sink: str = None):

    if not pipeline or not src or not sink:
        return None;

    request_unlink_elements = """{
        "command": "unlink_elements",
        "pipeline": \"""" + pipeline + """\",
        "src": \"""" + src + """\",
        "sink": \"""" + sink + """\"
    }"""


    print("> Unlink elements")
    socket.send_string(request_unlink_elements)
    message = socket.recv()
    load_json = json.loads(message)

    return load_json

@app.get("/v1/set_properties")
async def set_properties():

    request_set_properties = """{
        "command": "set_properties",
        "pipeline": "matroska_video_0",
        "element": "souphttpsrc",
        "properties": {
            "location": "https://dl8.webmfiles.org/big-buck-bunny_trailer.webm"
        }
    }"""


    print("> Set properties")
    socket.send_string(request_set_properties)
    message = socket.recv()
    load_json = json.loads(message)

    return load_json

@app.get("/v1/set_pad_properties")
async def set_pad_properties():

    request_set_pad_properties = """{
        "command": "set_pad_properties",
        "pipeline": "videotestsrc_videobox_compositor",
        "element": "compositor",
        "pad_props": "sink_1",
        "properties": {
            "alpha": 0.1
        }
    }"""


    print("> Set pad properties")
    socket.send_string(request_set_pad_properties)
    message = socket.recv()
    load_json = json.loads(message)

    return load_json

@app.get("/v1/remove_element")
async def set_remove_element():

    request_remove_element = """{
        "command": "remove_element",
        "pipeline": "matroska_video_0",
        "element": "autovideosink"
    }"""


    print("> Remove element")
    socket.send_string(request_remove_element)
    message = socket.recv()
    load_json = json.loads(message)

    return load_json



@app.get("/v1/show")
async def show():

    request_show = """{
        "command": "show"
    }"""


    print("> Show all")
    socket.send_string(request_show)
    message = socket.recv()
    load_json = json.loads(message)

    return load_json

@app.get("/v1/exit")
async def exit():

    request_exit = """{
        "command": "exit"
    }"""


    print("> Exit")
    socket.send_string(request_exit)
    message = socket.recv()
    load_json = json.loads(message)

    return load_json
