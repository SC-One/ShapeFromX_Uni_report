import cv2
import time
import asyncio
import websockets
import mediapipe as mp

# Define mediapipe face detection model
face_detection_model = mp.solutions.face_detection.FaceDetection()

# Load monocular depth estimation model
mono_model = cv2.dnn.readNet("model-small.onnx")  # Download: https://github.com/isl-org/MiDaS/releases/tag/v2_1

# ... (rest of your code remains unchanged)

async def consumer(websocket, path):
    cap = cv2.VideoCapture(0)
    while True:
        ret, frame = cap.read()
        height, width, channels = frame.shape

        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        # Detect faces in the frame using the Face Detection model
        face_results = face_detection_model.process(frame)
        if face_results.detections:
            for face in face_results.detections:
                # ... (your face detection code)

                blob = cv2.dnn.blobFromImage(frame, 1/255., (256,256), (123.675, 116.28, 103.53), True, False)
                mono_model.setInput(blob)
                depth_map = mono_model.forward()

                depth_map = depth_map[0,:,:]
                depth_map = cv2.resize(depth_map, (width, height))
                depth_map = cv2.normalize(depth_map, None, 0, 1, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_32F)

                # Encode depth_map as JPG bytes for transmission
                _, depth_map_encoded = cv2.imencode('.jpg', depth_map * 255)

                # Send depth_map to connected clients
                await websocket.send(depth_map_encoded.tobytes())

                # Simulate frame rate for demonstration purposes
                await asyncio.sleep(0.1)  # Adjust the sleep time based on your actual frame rate

    cap.release()

# Start WebSocket server
start_server = websockets.serve(consumer, "localhost", 8765)  # Change localhost and port if needed

asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()
