#!/usr/bin/env python3
from http.server import BaseHTTPRequestHandler, HTTPServer
from urllib import parse
from pathlib import Path
import logging
import requests
import datetime
import json
import time
from sys import argv
import re
import socket


EVAL_PORT = 9010
DRIVER_API_PORT = 9020
FILEPATH_KEY = "filePath"
REPORT_INTERVAL = 10


# JSON field names
SIEVE_ENC_TIME = "encryption_time_sieve"
SYM_ENC_TIME = "encryption_time_symmetric"
UPLOAD_TIME = "upload_time"
DRIVER_EXEC_TIME = "driver_execution_time"


def get_hostname_short():
    return socket.gethostname().split('.', 1)[0]


class EvalDeviceHandler(BaseHTTPRequestHandler):
    def _set_response(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()


    def _set_json_response(self):
        self.send_response(200)
        self.send_header('Content-Type', 'application/json')
        self.end_headers()


    def eval_data_store(self, data_size, reps):
        stopwatch = []
        # Conduct evaluation
        data_file = Path(f'./tests/eval-{data_size}.txt').resolve()
        if not data_file.exists():
            logging.error("Data file not exist")

        for i in range(reps):
            if i % REPORT_INTERVAL == 0:
                logging.info(f"Progress {i}/{reps}")
            
            # encrypt
            start = datetime.datetime.now()
            enc_res = requests.post(f'http://localhost:{DRIVER_API_PORT}/store',
                                    json={
                                        FILEPATH_KEY: str(data_file)
                                    })
            end = datetime.datetime.now()
            elapsed = end - start
            elapsed_ms = elapsed.total_seconds() * 1000

            enc_res_json = enc_res.json()

            stopwatch.append({
                SIEVE_ENC_TIME: enc_res_json[SIEVE_ENC_TIME],
                SYM_ENC_TIME: enc_res_json[SYM_ENC_TIME],
                UPLOAD_TIME: enc_res_json[UPLOAD_TIME],
                DRIVER_EXEC_TIME: enc_res_json[DRIVER_EXEC_TIME],
                'total_time': elapsed_ms,
            })

            time.sleep(0.1)

        return stopwatch

    def eval_group_owners(self, group_size, data_size, reps):
        stopwatch = []

        # Conduct evaluation
        data_file = Path(f'./tests/eval-{data_size}.txt').resolve()
        if not data_file.exists():
            logging.error("Data file not exist")

        for i in range(reps):
            if i % REPORT_INTERVAL == 0:
                logging.info(f"Progress {i}/{reps}")
            
            # encrypt
            start = datetime.datetime.now()
            enc_res = requests.post(f'http://localhost:{DRIVER_API_PORT}/store',
                                    json={
                                        FILEPATH_KEY: str(data_file)
                                    })
            end = datetime.datetime.now()
            elapsed = end - start
            elapsed_ms = elapsed.total_seconds() * 1000

            enc_res_json = enc_res.json()

            stopwatch.append({
                SIEVE_ENC_TIME: enc_res_json[SIEVE_ENC_TIME],
                SYM_ENC_TIME: enc_res_json[SYM_ENC_TIME],
                UPLOAD_TIME: enc_res_json[UPLOAD_TIME],
                DRIVER_EXEC_TIME: enc_res_json[DRIVER_EXEC_TIME],
                'total_time': elapsed_ms,
            })

            time.sleep(5)

        return stopwatch


    def do_GET(self):
        logging.info("GET request,\nPath: %s\nHeaders:\n%s\n",
                     str(self.path), str(self.headers))

        parse_obj = parse.urlparse(self.path)
        params = parse.parse_qs(parse_obj.query)

        msg = f"GET request for {str(self.path)}"
        status = "failed"
        stopwatch = []

        try:
            script_url = params['scriptUrl'][0]
            sheet_id = params['spreadsheetId'][0]
            eval_type = "UNKNOWN"
            update_eval_sheet = params['record'][0]
            if update_eval_sheet == 'true':
                update_eval_sheet = True 
            else:
                update_eval_sheet = False

            if re.match(r"/data-store.*", parse_obj.path):
                logging.info("Data store endpoint")
                data_size = params['size'][0].upper()
                reps = int(params['reps'][0])

                eval_type = f"Data Store {data_size} * {reps}"
                stopwatch = self.eval_data_store(data_size, reps)
                status = "success"
            elif re.match(r"/group.*", parse_obj.path):
                logging.info("Group endpoint")
                group_size = int(params['groupSize'][0])
                data_size = params['size'][0].upper()
                reps = int(params['reps'][0])

                eval_type = f"Group size {group_size}, {data_size} *{reps}"
                stopwatch = self.eval_group_owners(group_size, data_size, reps)
                status = "success"
            else:
                logging.info("Unknown endpoint")
                self.send_response(400)
                return
        except:
            msg += "\nRequest handle error"

        # Upload result
        # Transform logging
        if update_eval_sheet:
            data_headers = []
            data_body = []
            for entry in stopwatch:
                data_headers = entry.keys()
                data_body.append(entry.values())

            data_headers = [str(x) for x in data_headers]
            payload = ','.join(data_headers)
            payload += '\n'
            for row in data_body:
                row = [str(x) for x in row]
                payload += ','.join(row)
                payload += '\n'

            upload_result_req = \
                requests.post(script_url,
                            params={
                                'timestamp': datetime.datetime.now()
                                .strftime("%Y.%m.%d. %H:%M:%S"),
                                "type": f"h:{get_hostname_short()},{eval_type}"[:69],
                                "id": sheet_id,
                            },
                            data=payload)

        res_json = {
            'status': status,
            'message': msg,
        }
        self._set_json_response()
        self.wfile.write(json.dumps(res_json).encode(encoding='utf_8'))


    def do_POST(self):
        # <--- Gets the size of data
        content_length = int(self.headers['Content-Length'])
        # <--- Gets the data itself
        post_data = self.rfile.read(content_length)
        logging.info("POST request,\nPath: %s\nHeaders:\n%s\n\nBody:\n%s\n",
                     str(self.path), str(self.headers), post_data.decode('utf-8'))

        self._set_response()
        self.wfile.write("POST request for {}".format(
            self.path).encode('utf-8'))


def run(server_class=HTTPServer, handler_class=EvalDeviceHandler, port=EVAL_PORT):
    logging.basicConfig(level=logging.INFO)
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    logging.info(f'Starting httpd at port: {port} ...\n')
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    httpd.server_close()
    logging.info('Stopping httpd...\n')


if __name__ == '__main__':
    if len(argv) == 2:
        run(port=int(argv[1]))
    else:
        run()
