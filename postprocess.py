import cv2

if __name__ == "__main__":
    # Not much here yet, denoising will be added here later.
    img = cv2.imread("scene.ppm")
    cv2.imwrite("scene.png", img)
