## IC - Project 1

## Compile

Go to `sndfile-example-src` folder and run `make`

## Testing

### Exercise 1

```bash
../sndfile-example-bin/wav_hist wavs/sample01.wav
```
To get the histogram run `cd plots/ex1` and then the python script `plots.py` 

</br>

### Exercise 2

```bash
../sndfile-example-bin/wav_cmp wavs/sample01.wav <outputfile.wav>
```
To get the histogram run `cd plots/ex2` and then the python script `plots.py` 

</br>

### Exercise 3

```bash
../sndfile-example-bin/wav_quant wavs/sample01.wav <outputfile.wav> <bits_to_keep>

```
The number of bits to keep should be an integer between 1 and 16.

</br>

### Exercise 4

***Single Echo***
```bash
../sndfile-example-bin/wav_effects <inputfile.wav> <outputfile.wav> -single_echo 
```
In order to check the program results you can listen to the output audio file.


**Bass**
```bash
../sndfile-example-bin/wav_effects <inputfile.wav> <outputfile.wav> -bass <bass gain>
```
In order to check the program results you can listen to the output audio file.


**Reverse**
```bash
../sndfile-example-bin/wav_effects <inputfile.wav> <outputfile.wav> -reverse 
```
In order to check the program results you can listen to the output audio file or you can do a reverse of the reversed.wav and check if the audio files is equal to the original one

**Vibrato**
```bash
../sndfile-example-bin/wav_effects <inputfile.wav> <outputfile.wav> -vibrato 
```
In order to check the program results you can listen to the output audio file.

**Amplitude Modulation**
```bash
../sndfile-example-bin/wav_effects <inputfile.wav> <outputfile.wav> -amplitude_modulation
```
In order to check the program results you can listen to the output audio file.

</br>

### Exercise 5
**Write into file**
```bash
../sndfile-example-bin/test_BitStream <filename> w
```
Writes into file data.

**Reads from file**
```bash
../sndfile-example-bin/test_BitStream <filename> r
```
Reads data from file data.

</br>

### Exercise 6
**Encode data**
```bash
../sndfile-example-bin/encoder <inputFileName1.txt> <outputFileName.txt> <bitCountFileName.txt>
```

**Decode data**
```bash
../sndfile-example-bin/decoder <outputFileName1> <outputFileName.txt> 
```

At the end of two commands run `dif inputFileName1.txt outputFileName` in order to check if there is indeed differences between those two files.

</br>

### Exercise 7
**Compress File**
```bash
    Usage: wav_dct [ -c file to compress ]
                   [ -d file to decompress ]
                   [ -bs blockSize (def 1024) ]
                   [ -frac dctFraction (def 0.2) ]
                   [ -qe quantizationExponent (!do not change!) (def 11) ]
                   [ -qm quantizationMantissa (!do not change!) (def 11) ]
                   outputfile
```

