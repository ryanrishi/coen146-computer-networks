"""
Ryan Rishi
A. Amer
COEN 146 L - Computer Networks Lab
3 March 2016

Prompt user for a Prime Number and store the input in p
Prompt user for a primitive root of p and store the input in r
Prompt user for Alice's private key and store the input in a
Prompt user for Bob's private key and store in b
Calculate Alice's public key (x = r^a mod p)
Calculate Bob's public key (y = r^b mod p)
Assume public values x and y are exchanged
Calculate Alice's shared private key ( k_a = y^a mod p)
Calculate Bob's shared key (k_b = x^b mod p)
Is k_a = k_b?
"""

import math
from fractions import gcd


def is_prime(n):
    if n == 1 or n == 2:
        return True
    for i in xrange(1, int(math.ceil(math.sqrt(n))) + 1):
        if gcd(i, n) is not 1:
            return False
    return True


if __name__ == "__main__":
    p = None
    while not p:
        p = input('Enter a prime number: ')
        p = int(p)
        if not is_prime(p):
            print '%d is not prime' % p
            p = None

    r = input('Enter a primitive root of p: ')  # for the purposes of this lab, no need to sanitize that this is primite root
    a = input('Enter Alice\'s public key: ')
    b = input('Enter Bob\'s public key: ')

    g_a = r ** a % p    # Alice's private key
    g_b = r ** b % p    # Bob's private key
    k_a = g_b ** a % p  # Shared key as Alice would compute it
    k_b = g_a ** b % p  # Shared key as Bob would compute it

    print 'Alice\'s public key is\t%d'% g_a
    print 'Bob\'s public key is \t%d' % g_b
    if (k_a == k_b):
        print 'Their shared key is\t%d' % k_a
    else:
        print 'Their shared keys do not match.'
