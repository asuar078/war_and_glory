import pygatt
from binascii import hexlify

adapter = pygatt.GATTToolBackend()

def handle_data(handle, value):
    """
    handle -- integer, characteristic read handle the data was received on
    value -- bytearray, the data returned in the notification
    """
    print("Received data: %s" % hexlify(value))

try:
    adapter.start()
    device = adapter.connect('E5:04:28:61:C6:46')

    device.subscribe("00002902-0000-1000-8000-00805f9b34fb",
                     callback=handle_data)
finally:
    adapter.stop()
