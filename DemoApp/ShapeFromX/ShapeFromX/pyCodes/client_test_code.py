import asyncio
import websockets
import cv2
import numpy as np
import matplotlib.pyplot as plt

async def receive_depth_map():
    async with websockets.connect("ws://localhost:8765") as websocket:  # Change URL and port if needed
        fig, ax = plt.subplots()  # Create Matplotlib figure and axis
        imgplot = ax.imshow(np.zeros((480, 640, 3), dtype=np.uint8))  # Initialize with blank image

        while True:
            # Receive depth map bytes from the server
            depth_map_bytes = await websocket.recv()

            # Decode received bytes back to depth map image
            depth_map_np = cv2.imdecode(np.frombuffer(depth_map_bytes, dtype=np.uint8), cv2.IMREAD_UNCHANGED)

            # Update displayed depth map
            imgplot.set_data(cv2.cvtColor(depth_map_np, cv2.COLOR_BGR2RGB))
            plt.pause(0.01)  # Update display

asyncio.get_event_loop().run_until_complete(receive_depth_map())
