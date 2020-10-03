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

from data_utils import *

#imports the python encryption library
import monocypher
import unittest
import base64

#imports the task queue that will be used by the test server
import threading, queue

q_task = queue.Queue()
q_resp = queue.Queue()


#these are the task numbers!
ECHO = 0
TIMEOUT = 1
EXIT = 2

#encryption globals
KEY_LEN = 32
NONCE_LEN = 24
MAC_LEN = 16
PRIV_KEY, PUB_KEY = monocypher.generate_key_exchange_key_pair()
SHARED_KEY = ""

assert(len(PRIV_KEY) == 32)
assert(len(PUB_KEY) == 32)

###############################################################################
# Task Server
###############################################################################

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
        global SHARED_KEY
        print("POST b64 key: ",key)
        their_key = bytes(base64.b64decode(key))
        print(len(their_key), their_key)
        print("POST decoded key: ",their_key)
        SHARED_KEY = bytes(monocypher.key_exchange(PRIV_KEY,their_key))
        
        print("shared key: ",SHARED_KEY.hex())

    #this provides a task or the client
    def do_get_task(self):

        global SHARED_KEY
        print("GET task")
        item = q_task.get()
        q_task.task_done()
        print ("got item: ",item)
        enc = encrypyt_wrapper(SHARED_KEY, PUB_KEY, item.serialize_task())
        print("enc bytes: ",enc.hex())
        b64 = base64.b64encode(enc)
        print("sending {} bytes: {}".format(len(b64),b64))
        self.wfile.write(b64) #sends servers public key

    #this gets a response based off of a task
    def do_post_task_resp(self,resp):

        global SHARED_KEY

        print("POST task: ",resp.hex())
        ctext = bytes(base64.b64decode(resp))
        print (ctext.hex())
        dec = decrypt_wrapper(SHARED_KEY,ctext)
        print ("decrypted bin",dec)
        #get the task num to check type of response
        task_num = struct.unpack("H",dec[:2])
        task_num = task_num[0]
        #check the type of response and enqueue a resp object
        if (ECHO == task_num):
            print("recieved ECHO resposne")
            task = TaskEcho("")
            task.deserialize_response(dec)
            q_resp.put(task)
        elif (TIMEOUT == task_num):
            print("recieved TIMEOUT resposne")
            task = TaskTimeout()
            task.deserialize_response(dec)
            q_resp.put(task)
        elif (EXIT == task_num):
            print("recieved EXIT resposne")
            task = TaskExit()
            task.deserialize_response(dec)
            q_resp.put(task)
        else:
            print("unknown response!")    
        self.wfile.write(self._html("POST!"))

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
        print (self.path,post_data)
        self._set_headers()
        if self.path == '/key':
            self.do_post_key(post_data.strip())
        elif self.path == '/task':
            print("RECV RESP")
            self.do_post_task_resp(post_data.strip())
        else:
            self.wfile.write(self._html("POST!"))

#starts the process
def run(server_class=HTTPServer, handler_class=S, addr="localhost", port=8000):
    server_address = (addr, port)
    httpd = server_class(server_address, handler_class)
    print("priv key:", PRIV_KEY,"pub key:", PUB_KEY)

    print(f"Starting httpd server on {addr}:{port}")
    httpd.serve_forever()

###############################################################################
# Task Server tests
###############################################################################

#LOL this is a terrible way to do this

class TestSPITESTORE(unittest.TestCase):
    def test_echo(self):
        print("starting echo test")
        my_echo_string = "testing my echo string" 
        q_task.put(TaskEcho(my_echo_string))
        print("awaiting echo test")
        task_resp = q_resp.get()
        q_resp.task_done()
        num, res = task_resp.return_res()
        self.assertEqual(ECHO,num)
        self.assertEqual(my_echo_string,res)
    
    def test_timeout(self):
        print("echo")

    def test_exit(self):
        print("echo")


###############################################################################
# Main
###############################################################################

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
    http_server_thread = threading.Thread(name='http_server',
                                          target = run,
                                          )#args=(args.listen,args.port))
    http_server_thread.start()                    

    # turn-on the worker thread
    unittest.main()