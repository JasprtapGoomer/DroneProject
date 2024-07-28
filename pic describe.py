#@author Jaspartap
#this python codes describes a png picture and detects people and faces
import cv2
import face_recognition
import numpy as np
from transformers import BlipProcessor, BlipForConditionalGeneration

# Load BLIP processor and model for image captioning
processor = BlipProcessor.from_pretrained("Salesforce/blip-image-captioning-base")
model = BlipForConditionalGeneration.from_pretrained("Salesforce/blip-image-captioning-base")

known_face_encodings = []
known_face_names = []

def describe_image(image_path):
    # Load image
    image = cv2.imread(image_path)
    rgb_image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

    # Detect faces
    face_locations = face_recognition.face_locations(rgb_image)
    face_encodings = face_recognition.face_encodings(rgb_image, face_locations)

    # Describe image using BLIP model
    inputs = processor(images=image, return_tensors="pt")
    out = model.generate(**inputs)
    description = processor.decode(out[0], skip_special_tokens=True)

    # Identify faces in the image
    face_descriptions = []
    for (top, right, bottom, left), face_encoding in zip(face_locations, face_encodings):
        matches = face_recognition.compare_faces(known_face_encodings, face_encoding)
        name = "Unknown"

        if True in matches:
            first_match_index = matches.index(True)
            name = known_face_names[first_match_index]

        face_descriptions.append({
            "location": (top, right, bottom, left),
            "name": name
        })

        # Draw rectangle around the face
        cv2.rectangle(image, (left, top), (right, bottom), (0, 255, 0), 2)
        cv2.putText(image, name, (left + 6, bottom - 6), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 1)

    # Save the annotated image
    output_image_path = "annotated_" + image_path.split("/")[-1]
    cv2.imwrite(output_image_path, image)

    return description, face_descriptions, output_image_path

def store_face_name(image_path, name):
    # Load image
    image = cv2.imread(image_path)
    rgb_image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

    # Detect faces
    face_locations = face_recognition.face_locations(rgb_image)
    face_encodings = face_recognition.face_encodings(rgb_image, face_locations)

    if face_encodings:
        # Add the first face found to the known faces
        known_face_encodings.append(face_encodings[0])
        known_face_names.append(name)
        return f"Stored {name}'s face data."
    else:
        return "No faces found in the image."

# Example usage:
if __name__ == "__main__":
    image_path = "/Users/jaspartapgoomer/Downloads/IMG_1162.jpg"
    description, face_descriptions, output_image_path = describe_image(image_path)
    print("Image description:", description)
    print("Face descriptions:", face_descriptions)
    print("Annotated image saved as:", output_image_path)

    # Store a face with a name
    #result = store_face_name("path_to_image_with_person.png", "Person's Name")
    #print(result)
