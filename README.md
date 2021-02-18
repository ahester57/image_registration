# Image Registration
### Austin Hester
### CS 6420 - Computer Vision
### UMSL SP2021, Prof. Sanjiv Bhatia

----
## Purpose

The purpose of this assignment is to apply and assess the effectiveness of both manual and automatic approaches to image
registration.

----

## Task

Image registration is the process of aligning image data in different images to a single coordinate system. The images may
have been sensed using different sensors or at different times and the goal of image registration is to make sure that the objects
in those images overlap each other, both in terms of position as well as scale. It is an essential process to enable comparison
between different images of the same scene or objects.

You are given a set of two images. The first image is the target image while the second image is the template image. Your job
is to resample the target image so that it is in the same coordinate system as that of the template image using image registration
techniques.

Image registration can be performed manually or automatically. The manual registration requires a human to identify a set of
points in the reference image as well as the corresponding points in the target image. Your program will use these points to
compute an affine or projective homography between the two images. Next, you should use the computed homography to warp
the target image to the coordinate space of the reference image. Lastly, display the images side-by-side to assess the quality of
registration. Also use image subtraction to show the amount of translation and scaling in the resulting image compared to the
input image.

----

#### Usage

```
Usage: register.exe [params] imagefile templatefile warpfile

        -M, --manual
                Perform Manual Registration
        -e, --epsilon (value:0.0001)
                ECC's Convergence Epsilon
        -h, --help (value:true)
                Show Help Message
        -m, --motion_type (value:affine)
                Type of Motion: translation/euclidean/affine/homography
        -o, --output_warp (value:out_warp.ecc)
                Output warp matrix filename
        -w, --warp_img_file (value:warped_ecc.jpg)
                Output warp image

        imagefile (value:<none>)
                Input Image
        templatefile (value:<none>)
                Template Image
        warpfile (value:<none>)
                Warp Matrix

```

----

https://github.com/ahester57/image_registration
