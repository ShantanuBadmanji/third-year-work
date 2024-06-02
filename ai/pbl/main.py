from typing import Union

from fastapi import FastAPI, Request
from fastapi.responses import HTMLResponse
from fastapi.staticfiles import StaticFiles
from fastapi.templating import Jinja2Templates

from nlp import get_emotion, maxEmotion

app = FastAPI()


app.mount("/static", StaticFiles(directory="static"), name="static")


templates = Jinja2Templates(directory="templates")


@app.get("/", response_class=HTMLResponse)
async def read_root(request: Request):
    return templates.TemplateResponse("index.html", {"request": request})


@app.post("/sentences")
async def read_item(request: Request, q: Union[str, None] = None):
    body = await request.json()
    print(body)
    max_emotion, emotions = maxEmotion(get_emotion(body["sentence"]))
    return {"emotions": emotions, "maxEmotion": max_emotion}
