#!/usr/bin/env python

import sys
from random import random
from os import urandom


"""
@function get_random_byte
Uses random() to get number on [0,1)
Then multiplies that number by 2**16 (and casts to int)
Then mods by 255 because bytes have values on [0, 255]
Then casts that as a char
@returns chr
"""
def get_random_byte():
    return chr(int(random() * 2**16) % 255)


"""
@function get_random_byte_xor
A secondary implementation of a random byte generator
Gets two random bits (similar method as above, but mod by 2 instead of 255)
Then XORs the bits
This process is repeated 8x to generate a bitstream of length 8, then casts to a char

I got this idea from my cryptography class and referenced this for additional help:
https://blog.cloudflare.com/ensuring-randomness-with-linuxs-random-number-generator/
@returns chr
"""
def get_random_byte_xor():
    arr = []
    for i in range(8):
        arr.append(int(random() * 2**32 % 2) ^ int(random() * 2**32 % 2))
    return chr(int(''.join(str(i) for i in arr), 2))


"""
@function get_urandom_byte
Uses /dev/urandom to get random bytes (so this is an OS-level random byte generator)
@returns chr
"""
def get_urandom_byte():
    return urandom(1)


"""
@function get_not_so_random_bytestream
Generates bytestream of characters in sequence, so it's not really random.
However, this gives entropy score of 8.0 because it generates a perfectly even distribution,
assuming it writes a multiple of 256 bytes

This function isn't used within this program, but it is what I used to generate a
"""
def get_not_so_random_bytestream():
    while True:
        for i in range(256):
            sys.stdout.write(chr(i))


if __name__ == "__main__":
    # the way I'm running this is ./generator | head -c 2048 > testfile
    # Head will close the pipe after 2048 bytes are read, which is why I have this exit upon IOError.
    # I referenced http://stackoverflow.com/questions/14207708/ioerror-errno-32-broken-pipe-python
    while (True):
        try:
            sys.stdout.write(get_urandom_byte());
        except IOError as e:
            break
