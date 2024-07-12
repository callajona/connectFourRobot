# Contour Approach

## Goal 
To detect the "state" of a game of Connect Four. State is referring to the contents of each game slot (empty, red, or yellow).
Idea: Find the board in the image by masking the blue. Detect the circles with contour detection. Find the centre points and detect the colours. 
This should work regardless of focal distance to a reasonable extent

## Explanation
- Apply mask to image to isolate the board (Blue mask)
- Detect contours
  - Largest contour = Board perimeter*
  - Smallest contours = noise
  - Middle contours = slots / larger noise
- Truncate contour list to include middle contours**
- Isolate slots with circulatory fitting. Only accept the most circular contours
  - If all 42 circles found:
    - Find centre coordinates
    - Order the coordinates
  - Else:
    - Figure out where other circles should be (ICP algorithm)
- Detect colours within circles – Average of multiple points?
- Obvious error check (colour on top of blank)

#### Next Questions
- Has the game state changed?
  - No: Time delay, run code again
  - Yes: Capture again to ensure the state is static – falling discs may cause problems
    - When static state found - run algorithm to calculate next best move

*Could find things outside of the board. Solution large background that fills the image or crop image to isolate the board.

** Exact cutoff points should depend on the focal length (distance from the camera to the board), the angle of the camera and distortion. If necessary, make these cutoff points dependent on the focal length [measure the length of the top side of the board edge (largest contour), compare to actual measured length of the board to calculate the focal length]. 


## Useful Links
### Contour
- [OpenCV: Contour Features](https://docs.opencv.org/4.x/dd/d49/tutorial_py_contour_features.html)

### ICP and other such Algorithms 
- [ICP](https://medium.com/@michaelscheinfeild/icp-iterative-closest-point-algorithm-32ecaf58e9da)
- [Git: Probreg](https://github.com/neka-nat/probreg)
- [Git: Fast Robust ICP](https://github.com/yaoyx689/Fast-Robust-ICP)

### Colour Space
- [HSV](https://web.cs.uni-paderborn.de/cgvb/colormaster/web/color-systems/hsv.html)

### Stack Exchange
- [Stack Exchange: OpenCV Blob detection - Circulatity Check](https://stackoverflow.com/questions/42203898/python-opencv-blob-detection-or-circle-detection)
- [Stack Exchange: Contour Fill](https://stackoverflow.com/questions/72408809/how-do-i-fill-up-mask-holes-in-opencv)
- [Stack Exchange: HSV Histograms](https://stackoverflow.com/questions/72739316/plotting-hsv-channel-histograms-from-a-bgr-image-opencv)
