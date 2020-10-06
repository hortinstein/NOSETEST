import unittest
import monocypher
import random
import numpy as np
import unittest
import monocypher
import hashlib
import numpy as np
import binascii
import os
import json
import warnings

import struct 
#encryption globals
KEY_LEN = 32
NONCE_LEN = 24
MAC_LEN = 16

###############################################################################
# Task Wrappers
###############################################################################

#these are the task numbers!
ECHO = 0
TIMEOUT = 1
EXIT = 2

class TaskEcho():
    res =""
    # init method or constructor    
    def __init__(self, echo_string):   
        self.echo_string = echo_string
        #TODO test something here
        self.echo_string += '\0'

    def deserialize_response(self,res):
        #5 bytes is the size of the header
        self.task_num, size, self.res = struct.unpack("=HH{}s".format(len(res[4:])),res)
    
    def serialize_task(self):
        return struct.pack("=HH{}s".format(len(self.echo_string)),
                            ECHO,
                            len(self.echo_string),
                            self.echo_string.encode('utf-8'))
    
    def return_res(self):
        return ECHO, self.res

class TaskExit():
    res =""
    def deserialize_response(res):
        #5 bytes is the size of the header
        self.task_num, size, self.res = struct.unpack("=HH{}s".format(len(res[4:])),res)
    
    def serialize_task(self):
        return struct.pack("=HH",EXIT,0)
    
    def return_res(self):
        return EXIT, self.res

class TaskTimeout():
    res =""
    def __init__(self, timeout):   
        self.timeout = timeout
    # init method or constructor    
    def deserialize_response(self,res):
        #5 bytes is the size of the header
        self.task_num, size, self.res = struct.unpack("=HH{}s".format(len(res[4:])),res)
    
    def serialize_task(self):
        return struct.pack("=HHH",TIMEOUT,4,self.timeout)
    
    def return_res(self):
        return self.task_nun, self.res

###############################################################################
# Encrpytion Wrappers
###############################################################################

def encrypyt_wrapper(SHARED_KEY, PUB_KEY,plaintext):
    random = np.random.RandomState(seed=1)
    nonce = bytes(random.randint(0, 256, NONCE_LEN, dtype=np.uint8))
    mac, c = monocypher.lock(SHARED_KEY, nonce, plaintext)
    print ("encrypt: SHARED_KEY {}\nNONCE {}\nMAC {}".format(SHARED_KEY.hex(),nonce.hex(),mac.hex()))
    print (len(c),c.hex())
    #sender pub key [unnessecary], nonce, mac, len of cyber text, cipher text
    
    return struct.pack("{}s{}s{}sL{}s".format(KEY_LEN,NONCE_LEN,MAC_LEN,len(c)),PUB_KEY,nonce,mac,len(c),c)

def decrypt_wrapper(SHARED_KEY,enc_msg):
    OFFSET = NONCE_LEN+KEY_LEN+MAC_LEN+8
    their_key, nonce, mac, clen, cipher = struct.unpack("{}s{}s{}sL{}s".format(KEY_LEN,NONCE_LEN,MAC_LEN,(len(enc_msg)-OFFSET)),enc_msg)
    print ("decrypt: SHARED_KEY {}\nNONCE {}\nMAC {}".format(SHARED_KEY.hex(),nonce.hex(),mac.hex()))
    print (clen,cipher)
    msg = monocypher.unlock(SHARED_KEY, nonce, mac, cipher)
    print(msg)
    return msg

###############################################################################
# Encrpytion Wrappers testing
###############################################################################
class TestEncWrappers(unittest.TestCase):
    def test_key_exchange_random(self):
        #!COMMENT write a test for the above encrypt and decrypt library
        #!CUT_START
        a_private_secret, a_public_secret = monocypher.generate_key_exchange_key_pair()
        b_private_secret, b_public_secret = monocypher.generate_key_exchange_key_pair()
        b_shared_secret = monocypher.key_exchange(b_private_secret, a_public_secret)
        a_shared_secret = monocypher.key_exchange(a_private_secret, b_public_secret)
        self.assertEqual(a_shared_secret, b_shared_secret)
        dumb_message = bytes("this is my message",encoding='utf8')

        #CODE CAVING
        self.assertEqual(dumb_message,
                         decrypt_wrapper(a_shared_secret,
                                         encrypyt_wrapper(b_shared_secret, a_public_secret,
                                                          dumb_message))
                        )
        #!CUT_END

if __name__ == "__main__":
    global PRIV_KEY, PUB_KEY
    PRIV_KEY, PUB_KEY = monocypher.generate_key_exchange_key_pair()
    unittest.main()