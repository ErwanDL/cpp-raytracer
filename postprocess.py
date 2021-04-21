import cv2

if __name__ == "__main__":
    img = cv2.imread("scene.ppm")
    cv2.imwrite("scene.png", img)
