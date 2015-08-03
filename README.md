## STAG Suite Tools

### About

This is a suite that has two associated projects, one intended for Steganography - STAG hide, and other intended for Steganalysis - STAG find. They were developed using C++ and the Qt suite, the 4.8 version.

If you want to check out the source code, just get the `source` folder and check that! But if you want to only use them as programs, get the `release` folder as they include both programs with all DLLs needed.

**Disclaimer:**
This was a project I done when I was an undergrad student researcher at GRIS, the CSIRT (Computer Security Incident Response Team) of UFRJ (Federal University of Rio de Janeiro - Brazil), you can find more about GRIS and their job at their [site](https://gris.dcc.ufrj.br)(portuguese only), they do an important job at tutoring undergrad students and teaching the college community about information security - a camp that is still full of misconception and lack of information.

This project works fine and achieved its purpose of making me understand Steganography and Steganalysis. Even though there's a lot of work that could be done to add new methods and improve the steganalysis tool overall, I don't plan on improving this project anytime soon because I don't quite have the time currently. The tools were made in Portuguese, but I guess it is pretty straight-forward for anyone interested in using it.

### STAG hide

This was the project related to the Steganography part, where I wanted to hide information in images. I use the LSB filter, which stands for Least Significant Bits. This means that, for every pixel in a chosen image, I use the *n* least significant bits for its color components (red, blue and green) to hide some information, where *n* is a number input by the user. In a practical perspective, this means that when *n* is up to 3, I change each color component of a pixel in a maximum absolute value of 8, which is pretty much imperceptible if you are not comparing the result image to the original very closely. And even with a lower *n* (most of the time 1 is enough), you can write a lot of info in an image.

With n equals to 1, you can write 3 bits per pixel, which roughly translates to 1 byte every 3 pixels. In unicode, each char uses 1 byte, so even a low resolution image can store a lot of information.

For simplicity, I only allow the user to hide text inputs, but I store them as bits, so I could store anything actually, but the user interface would get more complex and I was still trying to fully grasp what I could learn of Steganography.

I also write the length of the hidden info in the first pixel of the message hiding, so I can read that more easily from the *STAG find*, but that might not have been the best solution.

### STAG find

This was the Steganalysis counter-part. But it is not really an analysis tool, it is a reader for when the user already knows at what pixel the message starts, and the number of bits used for the LSB algorithm. It works like that, but it could be more aggressive in the *analysis* part, like trying to infer the initial pixel and number of bits by reading the pixels of the image. Or just outputting some possibilities for the user so he could make an educated guess. But I never got to really work on that part =(.