import sys

while True:
    try:
        for i in range(256):
            sys.stdout.write(chr(i))
    except IOError as e:
        break
