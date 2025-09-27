import time
import os
import requests


# To test 5CS in Vice, point drive 11 to the following directory:
workingPath = "/Users/eric/Documents/projects/vice-device"


def download_url_as_bytes(url):
    """
    Download the content from a given URL and return it as a byte array.
    """
    try:
        response = requests.get(url, headers = {"User-Agent": "MockFujiNetBridge/1.0","Accept": "application/json"})
        response.raise_for_status()  # Raise an exception for bad status codes (4xx or 5xx)
        return response.content
    except requests.exceptions.RequestException as e:
        print(f"Error downloading URL: {e}")
        return bytes()
        
def processWatchFile():
    """
    Read the command of the watch file and send it as a message via the meshtastic interface.
    """

    with open(watchFile, "r", encoding="utf-8") as file:
        command = file.read().strip().lower()
        if command.startswith("n:"):
            command = command[2:]  # remove first two characters ("n:")
        print(f"Sending: {command}")

        if os.path.exists(outFile):
            os.remove(outFile)

        payload = download_url_as_bytes(command) 
        print(f"Received {len(payload)} bytes:")
        #time.sleep(0.200)  #  simulate delay
        hexdump(payload)

        with open(outFile, "wb") as file:
            file.write(payload)

        if os.path.exists(watchFile):
            os.remove(watchFile)

def hexdump(data: bytes, width: int = 16):
    for i in range(0, len(data), width):
        chunk = data[i:i+width]
        
        # Hex view
        hex_bytes = " ".join(f"{b:02x}" for b in chunk)
        
        # Pad hex output to align text view
        hex_bytes = hex_bytes.ljust(width * 3)
        
        # Text view (printable ASCII, else '.')
        text = "".join(chr(b) if 32 <= b < 127 else "." for b in chunk)
        
        print(f"{i:08x}  {hex_bytes}  {text}")

print("Mock FujiNet Network Bridge")
print("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-")

watchFile = workingPath + "/vice-out"
outFile = workingPath + "/vice-in"

prevWatchFileCtime = 0
waitMinAmount = 0.050
waitMaxAmount = 0.200
waitAmount = waitMinAmount

print(f"Watching: {watchFile}")

try:
    while True:
        if os.path.exists(watchFile) and os.path.getsize(watchFile) > 0 and os.path.getctime(watchFile) != prevWatchFileCtime:
            try:
                with open(watchFile, 'a') as f:
                    writable = f.writable()
                if writable:
                    prevWatchFileCtime = os.path.getctime(watchFile)
                    waitAmount=waitMinAmount
                    processWatchFile()
                else:
                    waitAmount = min(waitAmount + waitMinAmount, waitMaxAmount) 
            except IOError:
                time.sleep(waitMinAmount)
           

        time.sleep(waitMinAmount)  #  waitMinAmount
except KeyboardInterrupt:
    print("Exiting")
iface.close()
