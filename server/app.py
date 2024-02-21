from flask import Flask, request, abort
from dataclasses import asdict

from models import Trafficlight, Mode, M
from commands import *

app = Flask(__name__)

items = {
    111: Trafficlight()
}
items[111].commandQueue += [
    # ManualCommand(False),
    # SetModeCommand([M(''), M('')]),
    # ProgramCommand([
    #     ProgramStep((M('r'), M('r')), 3),
    #     ProgramStep((M('yb'), M('yb')), 3),
    #     ProgramStep((M('g'), M('r')), 3),
    # ])
]


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
    return {"status": "ok"}


@app.get("/remote/<int:uid>")
def pull_mode(uid: int):
    if uid not in items:
        abort(404)
    tasks = items[uid].commandQueue
    if not len(tasks):
        abort(404)
    return asdict(tasks.pop(0))


@app.get("/<int:uid>")
def get(uid: int):
    return items[uid]
