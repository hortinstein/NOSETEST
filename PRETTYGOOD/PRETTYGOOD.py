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


PRIV_KEY, PUB_KEY = monocypher.generate_key_exchange_key_pair()
        
class TestTasks(unittest.TestCase):

    def test_get_task(self):
        random = np.random.RandomState(seed=1)
        for i in range(10):
            length = random.randint(1, 4096)
            key = bytes(random.randint(0, 256, 32, dtype=np.uint8))
            nonce = bytes(random.randint(0, 256, 24, dtype=np.uint8))
            msg = bytes(random.randint(0, 256, length, dtype=np.uint8))
            mac, c = monocypher.lock(key, nonce, msg)
            msg2 = monocypher.unlock(key, nonce, mac, c)
            self.assertNotEqual(msg, c)
            self.assertEqual(msg, msg2)

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


    def do_get_key(self):
        print("GET key")

    def do_post_key(self):
        print("POST key")

    def do_get_task(self):
        print("GET task")

    def do_post_task(self):
        print("POST task")


    def do_GET(self):
        self._set_headers()
        if self.path == '/key':
            do_get_key(self)
        elif self.path == '/task':
            do_get_task(self)
        else:
            self.wfile.write(self._html("hi!"))

    def do_HEAD(self):
        self._set_headers()

    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length)
        print (post_data)
        self._set_headers()
        if self.path == '/key':
            do_post_key(self)
        elif self.path == '/task':
            do_post_task(self)
        else:
            self.wfile.write(self._html("POST!"))


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