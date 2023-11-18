import time
import cv2
import mediapipe as mp

# define mediapipe face detection model
face_detection_model = mp.solutions.face_detection.FaceDetection()

# load monocular depth estimation model
mono_model = cv2.dnn.readNet("model-small.onnx") # download: https://github.com/isl-org/MiDaS/releases/tag/v2_1

# OPTIONAL - if you have CUDA
#mono_model.setPreferableBackend(cv2.dnn.DNN_BACKEND_CUDA)
#mono_model.setPreferableTarget(cv2.dnn.DNN_TARGET_CUDA)

def depth_to_distance(depth) -> float:
    """convert depth map to a real-world distance
    """
    return -1.5 * depth + 2

# run your webcam
cap = cv2.VideoCapture(0)

while True:

    # read the frame from the video stream
    ret, frame = cap.read()
    height, width, channels = frame.shape

    # start the clock
    start_time = time.time()

    # convert the frame to RGB color space
    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

    # detect faces in the frame using the Face Detection model
    face_results = face_detection_model.process(frame)
    if face_results.detections:
        for face in face_results.detections:
            # draw bounding boxes around the detected faces
            mp.solutions.drawing_utils.draw_detection(frame, face)
            # in 0-1 scale
            boundary_box = face.location_data.relative_bounding_box
            # scale up to the image size
            boundary_box_scaled = int(boundary_box.xmin * width), int(boundary_box.ymin * height), int(boundary_box.width * width), int(boundary_box.height * height)
            # display the face detection score
            cv2.putText(frame, f'{int(face.score[0]*100)}%', (boundary_box_scaled[0], boundary_box_scaled[1] - 20), cv2.FONT_HERSHEY_SIMPLEX, 1, (0,255,0), 1)

            # interest point of depth in a face. The center will be measured.
            interest_point = (boundary_box_scaled[0] + boundary_box_scaled[2] / 2, boundary_box_scaled[1] + boundary_box_scaled[3] / 2)
        
        # MiDaS v2.1 Small ( Scale : 1 / 255, Size : 256 x 256, Mean Subtraction : ( 123.675, 116.28, 103.53 ), Channels Order : RGB,swapRB=True, crop=False )
        blob = cv2.dnn.blobFromImage(frame, 1/255., (256,256), (123.675, 116.28, 103.53), True, False)

        # set the input into the model
        mono_model.setInput(blob)

        # get depth map
        depth_map = mono_model.forward()
        
        # resize it to the real world
        depth_map = depth_map[0,:,:]
        depth_map = cv2.resize(depth_map, (width, height))
        depth_map = cv2.normalize(depth_map, None, 0, 1, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_32F)

        # change colors to display it in OpenCV
        frame = cv2.cvtColor(frame, cv2.COLOR_RGB2BGR)

        # get the depth of the point of interest
        depth = depth_map[int(interest_point[0]), int(interest_point[1])]

        depth_distance = depth_to_distance(depth)
        cv2.putText(frame,f"Depth to face: {str(round(depth_distance,2)*100)} cm",(15,100), cv2.FONT_HERSHEY_SIMPLEX, 1, (0,100,255), 4)
        
        # calculate fps 
        fps = 1 / (time.time() - start_time)
        cv2.putText(frame,f"FPS is {int(fps)}",(15,65), cv2.FONT_HERSHEY_SIMPLEX, 1, (0,255,0), 4)

        cv2.imshow("Face Distance Detection", frame)
        cv2.imshow("Depth Map", depth_map)

        if cv2.waitKey(5) & 0xFF == 27:
            break

cap.release()