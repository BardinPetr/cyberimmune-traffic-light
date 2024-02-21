from typing import Tuple

from flask import Flask, request
from dataclasses import dataclass, field

app = Flask(__name__)


@dataclass
class Mode:
    r: bool = False
    y: bool = False
    g: bool = False
    blink: bool = False


@dataclass
class Trafficlight:
    tlid: int
    is_maintenance: bool = False
    is_manual: bool = False
    current_mode: Tuple[Mode, Mode] = field(default=Mode)


trafficlights = {
    111: Trafficlight(111)
}


@app.post("/status")
def push_status():
    data = request.json
    tl = trafficlights[data['id']]
    tl.is_maintenance = data['is_maintenance']
    tl.is_manual = data['is_manual']
    tl.current_mode = [Mode(**i) for i in data['modes']]
    print(trafficlights)
    return {"status": "ok"}


@app.get("/status")
def get_status():
    return {"status": "ok"}


@app.post("/error")
def push_error():
    data = request.json
