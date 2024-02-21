#!/usr/bin/python

import time

from flask import Flask, request, abort, send_from_directory
from dataclasses import asdict

from models import Trafficlight, Mode, M
from commands import *
from utils import *

app = Flask(__name__, static_url_path='')

items = {
    111: Trafficlight()
}


#######
# TL endpoints

@app.post("/status")
def push_status():
    data = request.json
    if data['id'] not in items:
        abort(404)
    tl = items[data['id']]

    tl.is_maintenance = data['is_maintenance']
    tl.is_manual = data['is_manual']
    tl.current_mode = [Mode(**i) for i in data['modes']]

    print(f"Update: {tl.current_mode}")
    return {"status": "ok"}


@app.post("/error")
def push_error():
    data = request.json
    if data['id'] not in items:
        abort(404)
    tl = items[data['id']]
    tl.errors.append(data)
    tl.errors[-1]['time'] = int(time.time())
    return {"status": "ok"}


@app.get("/remote/<int:uid>")
def pull_mode(uid: int):
    if uid not in items:
        abort(404)
    tasks = items[uid].commandQueue
    if not len(tasks):
        return 'no data', 204
    return asdict(tasks.pop(0))


#######
# User endpoints

@app.route('/')
def index():
    return send_from_directory('static', 'index.html')


@app.get("/status/<int:uid>")
def get(uid: int):
    if uid not in items:
        abort(404)
    tl = items[uid]
    return {
        "maintenance": tl.is_maintenance,
        "manual": tl.is_manual,
        "modes": [print_mode(i) for i in tl.current_mode],
        "errors": print_errors(tl.errors)
    }


@app.post("/maintenance/<int:uid>/<int:state>")
def maintenance(uid, state):
    if uid not in items:
        abort(404)
    items[uid].commandQueue.append(MaintenanceCommand(bool(state)))
    return "OK"


@app.post("/manual/<int:uid>/<int:state>")
def manual(uid, state):
    if uid not in items:
        abort(404)
    items[uid].commandQueue.append(ManualCommand(bool(state)))
    return "OK"


@app.post("/state/<int:uid>/<state0>/<state1>")
def state(uid, state0, state1):
    if uid not in items:
        abort(404)
    items[uid].commandQueue.append(SetModeCommand([M(state0), M(state1)]))
    return "OK"


@app.post("/prog/<int:uid>")
def prog(uid):
    if uid not in items:
        abort(404)
    text = request.json['code']
    text = text.strip().split(';')
    text = [i.strip().split('+') for i in text]
    items[uid].commandQueue.append(ProgramCommand(
        [ProgramStep((M(i), M(j)), 1000) for i, j in text]
    ))
    return "OK"


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
