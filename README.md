# Rubix Solver
 - Rubix cube solver using computer vision.
 - Identifies the state of your rubix cube and helps you to solve it.
 - Step by step instructions to solve the rubix cube.

<img src="https://raw.githubusercontent.com/tentone/rubix-solver/master/readme/animation.gif" width="600">

## Setup
 - Install dependencies from the `install.sh` file.
 - Install [OpenCV](https://github.com/opencv/opencv) from github (pre-built binary or from souce).
    - Can use the `install-opencv.sh` script available on the repository
 - Run the code using cmake or the `run.sh` script.


## Vision Algorihtm
 - The vision algorithm is quite simple and is composed of three main steps.
 - We look for clusters of quads that compose the face of the cube.
 - Each quad is tested for color using HSL filtering.
 - Finally the quads are sorted based on the position.
 - The faces are identified by their center color.

<img src="https://raw.githubusercontent.com/tentone/rubix-solver/master/readme/vision.png" width="600">


## Solve Algorithm
 - The algorithm used to solve the rubix cube in the system uses a brute-force approach.
    - Some optimizations are performed to remove ambigous movements.
 - The depth of moves is limited by performance of the algorithm.
 - An alternative algorithms such as the [Thisletwaite's algorithm](https://github.com/conanwu777/rubik) could be used instead.
 - The cube movements use the notation presented in the picture bellow.

<img src="https://raw.githubusercontent.com/tentone/rubix-solver/master/readme/notation.png" width="400">

## License
 - This project is distributed under MIT license available on the repository page.