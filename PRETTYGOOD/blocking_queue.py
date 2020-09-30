import threading, queue

q = queue.Queue()

# turn-on the worker thread
threading.Thread(target=GET_POST_worker, daemon=True).start()

# send thirty task requests to the worker
for item in range(30):
    q.put(item)
print('All task requests sent\n', end='')

# block until all tasks are done
q.join()
print('All work completed')

class TestMonocypher(unittest.TestCase):
    def test_echo(self):
      

if __name__ == "__main__":
    global PRIV_KEY, PUB_KEY
    PRIV_KEY, PUB_KEY = monocypher.generate_key_exchange_key_pair()

    # turn-on the worker thread
    threading.Thread(target=GET_POST_worker, daemon=True).start()
    unittest.main()