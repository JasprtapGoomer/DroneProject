#@author Jaspartap
#This is a gui code which access you rcameras and the basic models for image proccesing and facial detection
import cv2
import face_recognition
import numpy as np
from PIL import Image, ImageTk
import tkinter as tk
from tkinter import simpledialog
import pickle

# Load YOLOv3 class labels
with open("coco.names", "r") as f:
    class_labels = f.read().strip().split("\n")

# Load YOLOv3 model and configuration
net = cv2.dnn.readNetFromDarknet("yolov3.cfg", "yolov3.weights")
layer_names = net.getLayerNames()
output_layers = [layer_names[i - 1] for i in net.getUnconnectedOutLayers()]

class FaceRecognitionApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Camera Stream with Face Recognition and Object Detection")
        self.root.attributes("-fullscreen", True)

        self.video_label = tk.Label(root)
        self.video_label.pack(fill=tk.BOTH, expand=True)

        self.register_button = tk.Button(root, text="Register Face", command=self.register_face)
        self.register_button.pack()

        # Use the default camera (usually index 0)
        self.cap = cv2.VideoCapture(0)

        self.known_face_encodings = []
        self.known_face_names = []
        self.load_known_faces()

        self.update_frame()

    def load_known_faces(self):
        try:
            with open("known_faces.pkl", "rb") as f:
                known_faces = pickle.load(f)
                self.known_face_encodings, self.known_face_names = known_faces
        except FileNotFoundError:
            pass

    def save_known_faces(self):
        with open("known_faces.pkl", "wb") as f:
            known_faces = (self.known_face_encodings, self.known_face_names)
            pickle.dump(known_faces, f)

    def register_face(self):
        ret, frame = self.cap.read()
        if ret:
            frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            face_locations = face_recognition.face_locations(frame_rgb)
            if face_locations:
                face_encoding = face_recognition.face_encodings(frame_rgb, face_locations)[0]
                name = simpledialog.askstring("Input", "Enter the name of the person:", parent=self.root)
                if name:
                    self.known_face_encodings.append(face_encoding)
                    self.known_face_names.append(name)
                    self.save_known_faces()

    def update_frame(self):
        ret, frame = self.cap.read()
        if ret:
            # Resize the frame to a larger size
            frame = cv2.resize(frame, (1280, 720))

            frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            face_locations = face_recognition.face_locations(frame_rgb)
            face_encodings = face_recognition.face_encodings(frame_rgb, face_locations)

            # Perform face recognition
            for (top, right, bottom, left), face_encoding in zip(face_locations, face_encodings):
                name = "Unknown"

                if self.known_face_encodings:
                    face_distances = face_recognition.face_distance(self.known_face_encodings, face_encoding)
                    if len(face_distances) > 0:
                        best_match_index = np.argmin(face_distances)
                        matches = face_recognition.compare_faces(self.known_face_encodings, face_encoding)
                        if matches[best_match_index]:
                            name = self.known_face_names[best_match_index]

                cv2.rectangle(frame, (left, top), (right, bottom), (0, 0, 255), 2)
                cv2.rectangle(frame, (left, bottom - 35), (right, bottom), (0, 0, 255), cv2.FILLED)
                font = cv2.FONT_HERSHEY_DUPLEX
                cv2.putText(frame, f"Hi, {name}!", (left + 6, bottom - 6), font, 0.5, (255, 255, 255), 1)

            # Perform object detection with YOLOv3
            (h, w) = frame.shape[:2]
            blob = cv2.dnn.blobFromImage(frame, 1 / 255.0, (416, 416), swapRB=True, crop=False)
            net.setInput(blob)
            detections = net.forward(output_layers)

            boxes = []
            confidences = []
            class_ids = []

            for output in detections:
                for detection in output:
                    scores = detection[5:]
                    class_id = np.argmax(scores)
                    confidence = scores[class_id]
                    if confidence > 0.5:
                        box = detection[0:4] * np.array([w, h, w, h])
                        (centerX, centerY, width, height) = box.astype("int")
                        x = int(centerX - (width / 2))
                        y = int(centerY - (height / 2))

                        boxes.append([x, y, int(width), int(height)])
                        confidences.append(float(confidence))
                        class_ids.append(class_id)

            idxs = cv2.dnn.NMSBoxes(boxes, confidences, 0.5, 0.3)

            if len(idxs) > 0:
                for i in idxs.flatten():
                    (x, y) = (boxes[i][0], boxes[i][1])
                    (w, h) = (boxes[i][2], boxes[i][3])
                    color = [int(c) for c in np.random.randint(0, 255, size=(3,))]
                    cv2.rectangle(frame, (x, y), (x + w, y + h), color, 2)
                    text = "{}: {:.4f}".format(class_labels[class_ids[i]], confidences[i])
                    cv2.putText(frame, text, (x, y - 5), cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 2)

            img = Image.fromarray(frame)
            self.imgtk = ImageTk.PhotoImage(image=img)
            self.video_label.imgtk = self.imgtk
            self.video_label.configure(image=self.imgtk)

        self.root.after(10, self.update_frame)

    def __del__(self):
        if self.cap.isOpened():
            self.cap.release()

if __name__ == "__main__":
    root = tk.Tk()
    app = FaceRecognitionApp(root)
    root.mainloop()
