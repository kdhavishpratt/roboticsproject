"""Build the guide page: embed the images into guide.src.html and write guide.html.

    cd docs/guide && python3 build.py

The published copy lives at https://claude.ai/artifact/RWXzkgmmDWiwHpDqywjX2H
"""
import base64
from pathlib import Path

here = Path(__file__).parent
s = (here / 'guide.src.html').read_text()


def data(path, mime):
    return 'data:%s;base64,%s' % (mime, base64.b64encode((here / path).read_bytes()).decode())


for key, path, mime in [
    ('__IMG10__', 'img/aihand_10.png', 'image/png'),
    ('__IMG9__', 'img/aihand_9.png', 'image/png'),
    ('__G1__', 'img/glow/g1.jpg', 'image/jpeg'),
    ('__G5__', 'img/glow/g5.jpg', 'image/jpeg'),
    ('__G8__', 'img/glow/g8.jpg', 'image/jpeg'),
    ('__G3__', 'img/glow/g3.jpg', 'image/jpeg'),
]:
    s = s.replace(key, data(path, mime))

(here / 'guide.html').write_text(s)
print('built guide.html,', len(s), 'bytes')
