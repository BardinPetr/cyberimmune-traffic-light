from flask import Flask, request

app = Flask(__name__)


@app.post("/")
def test():
    data = request.json
    print(data)
    return {"test": 324, "res": data}

@app.get("/")
def testw():
    return {"test": 324, "res": 2}
