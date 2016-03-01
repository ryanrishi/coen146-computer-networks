#!/usr/bin/env python

from math import log
import sys

"""
Initialize an array to hold frequency of characters
"""
frequency = []
for i in range(256):
    frequency.append(0)

count = 0   # total number of bytes read


"""
Read from stdin until stdin is empty
"""
while True:
    try:
        byte = ord(sys.stdin.read(1))
        frequency[byte] += 1
        count += 1
    except TypeError as e:
        # once stdin is empty
        break


"""
Compute frequency distribution and entropy using shannon entropy formula
http://blogs.cisco.com/security/on_information_entropy/
"""
entropy = 0
for i in range(256):
    if frequency[i]:
        probability = float(frequency[i]) / float(count)
        entropy += probability * log(1/probability, 2)


print 'Number of bytes read: %d' % count
print 'Entropy: %.8f' % entropy
