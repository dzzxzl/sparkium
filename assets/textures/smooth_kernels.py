import cv2
import numpy as np

input_image_path = "moon.png"

# Read the noisy image
noisy_image = cv2.imread(input_image_path, cv2.IMREAD_GRAYSCALE)

# Apply Gaussian blur to smooth the image
smoothed_image = cv2.GaussianBlur(noisy_image, (3, 3), 0)

# Compute the gradient using Sobel operators
gradient_x = cv2.Sobel(smoothed_image, cv2.CV_64F, 1, 0, ksize=3)
gradient_y = cv2.Sobel(smoothed_image, cv2.CV_64F, 0, 1, ksize=3)

old_gradient_x = cv2.Sobel(noisy_image, cv2.CV_64F, 1, 0, ksize=3)
old_gradient_y = cv2.Sobel(noisy_image, cv2.CV_64F, 0, 1, ksize=3)

# Combine gradient_x and gradient_y to get the magnitude
gradient_magnitude = np.sqrt(gradient_x**2 + gradient_y**2)
old_gradient_magnitude = np.sqrt(old_gradient_x**2 + old_gradient_y**2)

# Optionally, convert gradient_magnitude to uint8 if needed
gradient_magnitude = np.uint8(gradient_magnitude)
old_gradient_magnitude = np.uint8(old_gradient_magnitude)

# Display the results
cv2.imshow('Original Image', noisy_image)
cv2.imshow('Smoothed Image', smoothed_image)
cv2.imshow('Gradient Magnitude', gradient_magnitude)
# save smoothed image
cv2.imwrite('smoothed_moon.png', smoothed_image)
cv2.imshow('Old Gradient Magnitude', old_gradient_magnitude)
cv2.waitKey(0)
cv2.destroyAllWindows()


