# IC - Project 2

## Compile

Go to `project02/project02` folder and run `make`

## Testing

### Exercise 1

```bash
../opencv-bin/ex1 <input_image> <output_image> <channel (B/G/R) | copy>
```
In this exercise, testing involves four distinct steps, corresponding to four different output options. These options encompass the extraction of the three `color channels (B/G/R)` from the input image, producing an output image in the selected color, as well as the `copy` option, which duplicates the original image to create a new image with the specified output filename (<output_image>).

An interesting observation is that when selecting the image located at `../imagens_PPM/peppers.ppm` an image predominantly consisting of red and green hues, if the command line input is similar to `../opencv-bin/ex1 imagens_PPM/peppers.ppm newImages/new_image.ppm b `, the resulting output image appears entirely black due to the absence of blue tones within this image