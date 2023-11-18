import cv2
import time
import asyncio
import websockets
import json

# Load monocular depth estimation model
mono_model = cv2.dnn.readNet("model-small.onnx")  # Download: https://github.com/isl-org/MiDaS/releases/tag/v2_1
 
async def consumer(websocket, path):
    cap = cv2.VideoCapture(0)
    while True:
        ret, frame = cap.read()
        height, width, channels = frame.shape

        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        blob = cv2.dnn.blobFromImage(frame, 1/255., (256,256), (123.675, 116.28, 103.53), True, False)
        mono_model.setInput(blob)
        depth_map = mono_model.forward()

        depth_map = depth_map[0,:,:]
        depth_map = cv2.resize(depth_map, (width, height))
        # print("size:",(960, int(960/width*height)))
        depth_map = cv2.resize(depth_map, (256, int(480/width*height)), interpolation=cv2.INTER_AREA)
        depth_map = cv2.normalize(depth_map, None, 0, 255, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_8U)

        # Prepare depth_map as a 2D array (vector of vectors)
        depth_map_2d = depth_map.tolist()

        # Send depth_map as a JSON object to connected clients 
        await websocket.send(json.dumps(depth_map_2d))

        # Simulate frame rate for demonstration purposes
        await asyncio.sleep(0.2)  # Adjust the sleep time based on your actual frame rate # max: 4 fps


    cap.release()

# Start WebSocket server
start_server = websockets.serve(consumer, "localhost", 50777)

asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()
