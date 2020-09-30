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


#these are the task numbers!
ECHO = 0

class task():
    num = 0
    args = ""
    def serialize(self):
        return ""

    def deserialize(self, binary):
        return ""

class serializable_list():
    list = []


def encrypyt_wrapper(SHARED_KEY, plaintext):
    random = np.random.RandomState(seed=1)
    nonce = bytes(random.randint(0, 256, NONCE_LEN, dtype=np.uint8))
    mac, c = monocypher.lock(SHARED_KEY, nonce, bytes(plaintext,encoding='utf8'))
    print ("encrypt: SHARED_KEY{}\nNONCE {}\nMAC {}".format(SHARED_KEY,nonce,mac))
    print (len(c),c)
    #sender pub key [unnessecary], nonce, mac, len of cyber text, cipher text
    
    return struct.pack("{}s{}s{}sL{}s".format(KEY_LEN,NONCE_LEN,MAC_LEN,len(c)),PUB_KEY,nonce,mac,len(c),c)

def decrypt_wrapper(SHARED_KEY,enc_msg):
    OFFSET = NONCE_LEN+KEY_LEN+MAC_LEN+8
    their_key, nonce, mac, clen, cipher = struct.unpack("{}s{}s{}sL{}s".format(KEY_LEN,NONCE_LEN,MAC_LEN,(len(enc_msg)-OFFSET)),enc_msg)
    print ("decrypt: SHARED_KEY{}\nNONCE {}\nMAC {}".format(SHARED_KEY,nonce,mac))
    print (clen,cipher)
    msg = monocypher.unlock(SHARED_KEY, nonce, mac, cipher)
    print(msg)
    return msg




class TestMonocypher(unittest.TestCase):


    def test_key_exchange_random(self):
        a_private_secret, a_public_secret = monocypher.generate_key_exchange_key_pair()
        b_private_secret, b_public_secret = monocypher.generate_key_exchange_key_pair()
        b_shared_secret = monocypher.key_exchange(b_private_secret, a_public_secret)
        a_shared_secret = monocypher.key_exchange(a_private_secret, b_public_secret)
        self.assertEqual(a_shared_secret, b_shared_secret)
        dumb_message = "this is my message"

        #CODE CAVING
        self.assertEqual(dumb_message,
                         decrypt_wrapper(a_shared_secret,
                                         encrypyt_wrapper(b_shared_secret,
                                                          dumb_message)).decode()
                        )

if __name__ == "__main__":
    global PRIV_KEY, PUB_KEY
    PRIV_KEY, PUB_KEY = monocypher.generate_key_exchange_key_pair()
    unittest.main()