#!/usr/bin/env python3

import sys
from pathlib import Path
import os
import logging
import requests
import time 

logging.basicConfig(filename=os.environ['TOT_MOTION_LOG_PATH'], 
        level=logging.DEBUG,
        format='%(asctime)s %(message)s')

driver_ip = '127.0.0.1'
driver_port = 9020

FILEPATH_KEY = "filePath"
FILECONTENT_KEY = "fileContent"
EVAL_FILE = "eval-motion.csv"


def current_milli_time():
    return round(time.time() * 1000)

if __name__ == '__main__':
    timer_start = current_milli_time()

    message = 'On called for file: ' + sys.argv[1]
#     print(message)
#     logging.debug(message)

    r = requests.post(f"http://{driver_ip}:{driver_port}/store", json={FILEPATH_KEY: sys.argv[1]})

    timer_stop = current_milli_time()
    time_delta = timer_stop - timer_start

    eval_res = Path(Path.home(), EVAL_FILE)
    eval_res = eval_res.resolve()
    with eval_res.open('a') as f:
        movie_file = Path(sys.argv[1])
        f.write(f"{sys.argv[1]}, {movie_file.stat().st_size}, {time_delta}\n")

    logging.debug(r.status_code)
    logging.debug(r.text)

    # Clean up
    Path(os.environ['TOT_MOTION_LOG_PATH']).unlink()
