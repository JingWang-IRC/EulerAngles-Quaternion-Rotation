# EulerAngles-Quaternion-Rotation
Implemented rotation using Euler angles and quaternion respectively.
[A short demonstration](https://www.youtube.com/watch?v=KdTAo2uqvpg&t=1s) has been uploaded to Youtube.
For more detailed information, you can refer to the description (PDF) in this repository.
All the code are in the **Anime_Assignment1** folder, while the 3D models are in the **Release** folder.

# Euler angles and Quaternion
Euler angles is a easy way to implement rotations.
However, it will encounters a problem called [Gimbal Lock](https://www.youtube.com/watch?v=zc8b2Jo7mno&t=79s).
Also, the order of rotation is a problem of Euler angle.
When you rotate the object following the order, you can change the Yaw, Pitch, Roll angles accordingly.
Whereas, if you decide to rotate in the order of X-Y-Z, but rotate it in other order (like Y-X-Z), the rotation will not be what you expected.
Quaternion can solve this problem.
But for people who use quaternion for the first time may meet a problem that after using quaternion, they still have Gimbal Lock.
This is because they did not store the orientation until last rotation.
Instead, we should update the final quaternion after every frame, and the next rotation should based on this orientation.
The detailed implementation is in Anime_Assignment1/main.cpp. 

# Dependency
The project is build in Visual Studio 2017.
The used library are GLM-0.9.8.5, GLEW-2.1.0, GLFW-3.3.2, GLAD, Assimp5.0.0. 
