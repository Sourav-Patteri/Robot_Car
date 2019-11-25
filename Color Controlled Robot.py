#SOURAV PATTERI
##FINAL PROJECT
import cv2 as cv
import numpy as np
import serial
import time

Arduino = serial.Serial('com8',9600) #Create Serial Port object called arduino Serial Data
##(Tried a lot of different ways to connect bluetooth but seems my module was damaged)
time.sleep(2) #wait for 2 seconds for the communication to get established

cam = cv.VideoCapture(0)

lower_red = np.array([154,36,40])
upper_red = np.array([174,255,255])

while(1):
    ret, frame = cam.read()
    frame = cv.flip(frame,1)

    w = frame.shape[1]
    h = frame.shape[0]
    
    # Smoothen the Image
    image_smooth = cv.GaussianBlur(frame,(7,7),0)

    # Define ROI
    mask = np.zeros_like(frame)

    mask[50:350, 50:350] = [255,255,255]

    image_roi = cv.bitwise_and(image_smooth, mask)
    cv.rectangle(frame, (50,50), (350,350), (0,0,255), 2)
    cv.line(frame, (150,50), (150,350), (0,0,255), 1)
    cv.line(frame, (250,50), (250,350), (0,0,255), 1)
    cv.line(frame, (50,150), (350,150), (0,0,255), 1)
    cv.line(frame, (50,250), (350,250), (0,0,255), 1)
    

    # Threshold the Image for Red Color
    image_hsv = cv.cvtColor(image_smooth, cv.COLOR_BGR2HSV)
    image_threshold = cv.inRange(image_hsv, lower_red, upper_red)

    # Find contours
    contours, heirarchy = cv.findContours(image_threshold, \
                                        cv.RETR_TREE, \
                                        cv.CHAIN_APPROX_NONE)

    # Find the index of the largest contour
    if (len(contours)!=0):
        areas = [cv.contourArea(c) for c in contours]
        max_index = np.argmax(areas)
        cnt = contours[max_index]

        #Pointer on Video
        M = cv.moments(cnt)
        if(M['m00'] != 0):
            cx = int(M['m10']/M['m00'])
            cy = int(M['m01']/M['m00'])
            cv.circle(frame, (cx, cy), 4, (0,255,0), -1)

            #Cursor Motion. Depending on the position of cursor and in the x and y axis inside the grid it send values of f b l r s to arduino.
            if cx in range(150,250):
                if cy < 150:
                    Arduino.write(b'f')
                    print("Forward")
                elif cy >250:
                    Arduino.write(b'b')
                    print("Backward")

                else:
                    Arduino.write(b's')
                    print("Stop")


            if cy in range(150,250):
                if cx < 150:
                    Arduino.write(b'l')
                    print("Left")

                elif cx > 250:
                    Arduino.write(b'r')
                    print("Right")

                else:
                    Arduino.write(b's')
                    print("Stop")

                    
            if cx in range(250,350):
                if cy < 150:
                    Arduino.write(b'f')
                    print("Forward")
                    Arduino.write(b'r')
                    print("Right")
                elif cy > 250:
                    Arduino.write(b'b')
                    print("Backward")
                    Arduino.write(b'r')
                    print("Right")
                else:
                    Arduino.write(b's')
                    print("Stop")

            if cx in range(0,150):
                if cy < 150:
                    Arduino.write(b'f')
                    print("Forward")
                    Arduino.write(b'l')
                    print("Left")
                elif cy > 250:
                    Arduino.write(b'b')
                    print("Backward")
                    Arduino.write(b'l')
                    print("Left")
                else:
                    Arduino.write(b's')
                    print("Stop")

        
    cv.imshow('Frame', frame)

    key = cv.waitKey(100)
    if key == 27:
        break

cv.destroyAllWindows()
