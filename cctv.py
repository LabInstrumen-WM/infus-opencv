import numpy as np
import cv2
import time
from picamera.array import PiRGBArray
from picamera import PiCamera
import os
#tim=int (input("How Long CCTV ON?"))
VIDEO_BASE_FOLDER = '/home/pi/Videos/'
#VIDEO_LENGTH = tim*60
# global definitions
current_time_s = lambda: int(round(time.time()))
# use this to define your file name builder
def get_video_filename(now):
    return VIDEO_BASE_FOLDER+str(now)+'_video.h264'
def get_video_filenames(now):
    return VIDEO_BASE_FOLDER+str(now)+'_video.mp4'
# the timestamp when the video write started
video_time  = 0
# main logic
def see():
    # initialize the camera and grab a reference
    camera = PiCamera()
    camera.resolution = (800, 600)
    camera.framerate = 32
    rawCapture = PiRGBArray(camera, size=(800, 600))
    # allow the camera to warmup
    time.sleep(0.1)
    clock = current_time_s()
    video_time = clock
    # start recording using piCamera API
    camera.start_recording(get_video_filename (clock))
    # grab one frame at the time from the stream
    for frame in camera.capture_continuous(rawCapture, format="rgb", use_video_port=True):
        # grab the raw NumPy array representing the image
        frame = frame.array
        # bail if the camera stop sending frames.
        # TODO roll your own, this is crude at best!
        
        if frame==None:
            break
        # establish a time for the rest of the logic
        now = current_time_s()
        # simultaneous detect and write
        #if video_time+VIDEO_LENGTH<now:
            # close the current file
         #   camera.stop_recording()
          #  os.system("cd Videos")
           # os.system("MP4Box -add"+" "+get_video_filename (clock)+" "+get_video_filenames(clock))
            #os.system("rm"+" "+get_video_filename (clock))
            #break
            #video_time = now
            # open a new file and start recording
            #camera.start_recording(get_video_filename(now))
# TODO add your magic here
        
        # all we do is show the frame ...
        cv2.imshow('frame',frame)
        if cv2.waitKey(1)&0xFF==ord('q'):
        # save the last video file before shutting down
            camera.stop_recording()
            os.system("cd Videos")
            os.system("MP4Box -add"+" "+get_video_filename (clock)+" "+get_video_filenames(clock))
            os.system("rm"+" "+get_video_filename (clock))
            break
# clear the stream in preparation for the next frame
        rawCapture.truncate(0)
    print ('shutting down')
    
    
cv2.destroyAllWindows()
see()
