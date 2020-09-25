"""
Very simple HTTP server in python (Updated for Python 3.7)
Usage:
    ./PRETTYGOOD.py -h
    ./PRETTYGOOD.py -l localhost -p 8000
Send a GET request:
    curl http://localhost:8000
Send a HEAD request:
    curl -I http://localhost:8000
Send a POST request:
    curl -d "foo=bar&bin=baz" http://localhost:8000
"""
import argparse
from http.server import HTTPServer, BaseHTTPRequestHandler

#imports the python encryption library
import monocypher
import unittest
import base64

import struct

#encryption globals
KEY_LEN = 32
NONCE_LEN = 24
MAC_LEN = 16
PRIV_KEY, PUB_KEY = monocypher.generate_key_exchange_key_pair()
SHARED_KEY = ""

assert(len(PRIV_KEY) == 32)
assert(len(PUB_KEY) == 32)

def encrypyt_wrapper(plain_msg):
    nonce = bytes(random.randint(0, 256, NONCE_LEN, dtype=np.uint8))
    mac, c = monocypher.lock(SHARED_KEY, nonce, msg)
    #sender pub key [unnessecary], nonce, mac, len of cyber text, cipher text
    struct.pack("{}c{}c{}cLs".format(KEY_len,NONCE_LEN,MAC_LEN),PUB_KEY,nonce,mac,len(c),c)

def decrypt_wrapper(enc_msg):
    their_key, nonce, mac, clen, enc_msg = struct.unpack("{}c{}c{}cLs".format(KEY_len,NONCE_LEN,MAC_LEN))
    msg = monocypher.unlock(SHARED_KEY, nonce, mac, enc_msg)
    print msg

class TestTasks(unittest.TestCase):

    def test_get_task(self):
        random = np.random.RandomState(seed=1)


class S(BaseHTTPRequestHandler):

    def _set_headers(self):
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()

    def _html(self, message):
        """This just generates an HTML document that includes `message`
        in the body. Override, or re-write this do do more interesting stuff.
        """
        content = message
        return content.encode("utf8")  # NOTE: must return a bytes object!

    #this provides the public key so the client can derive the shared session key
    def do_get_key(self):
        print("GET key, sending ", base64.b64encode(PUB_KEY))
        self.wfile.write(base64.b64encode(PUB_KEY)) #sends servers public key

    #this sets the remote clients public key and allows for the shared session key
    def do_post_key(self,key):
        print("POST b64 key: ",key)
        their_key = bytes(base64.b64decode(key))
        print(len(their_key), their_key)
        print("POST decoded key: ",their_key)
        SHARED_KEY = monocypher.key_exchange(PRIV_KEY,their_key)
        print("shared key: ",SHARED_KEY.hex())

    #this provides a task or the client
    def do_get_task(self):
        print("GET task")

    #this gets a response based off of a task
    def do_post_task_resp(self):
        print("POST task: ",resp)


    #routing for get requests
    def do_GET(self):
        self._set_headers()
        if self.path == '/key':
            self.do_get_key()
        elif self.path == '/task':
            self.do_get_task()
        else:
            self.wfile.write(self._html("hi!"))

    def do_HEAD(self):
        self._set_headers()

    #routing for post requests
    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length)
        print (post_data)
        self._set_headers()
        if self.path == '/key':
            self.do_post_key(post_data.strip())
        elif self.path == '/task':
            self.do_post_task_resp()
        else:
            self.wfile.write(self._html("POST!"))

#starts the process
def run(server_class=HTTPServer, handler_class=S, addr="localhost", port=8000):
    server_address = (addr, port)
    httpd = server_class(server_address, handler_class)
    print("priv key:", PRIV_KEY,"pub key:", PUB_KEY)

    print(f"Starting httpd server on {addr}:{port}")
    httpd.serve_forever()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run a simple HTTP server")
    parser.add_argument(
        "-l",
        "--listen",
        default="localhost",
        help="Specify the IP address on which the server listens",
    )
    parser.add_argument(
        "-p",
        "--port",
        type=int,
        default=8000,
        help="Specify the port on which the server listens",
    )
    args = parser.parse_args()
    run(addr=args.listen, port=args.port)