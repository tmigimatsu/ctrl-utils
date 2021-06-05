"""
numpy.py

Copyright 2019. All Rights Reserved.

Created: 01/14/19
Authors: Toki Migimatsu
"""

import numpy as np

##
# Decode a Numpy array from Matlab format.
#
# Example:
# ```{.py}
# x = decode_matlab("1 2 3")
# A = decode_matlab("1 2; 3 4")
# ```
#
# @param s str or bytes
# @return numpy.ndarray[float64[m,n]]
def decode_matlab(s):
    try:
        s = s.decode("utf-8")
    except AttributeError:
        pass
    return np.array([list(map(float, row.strip().split())) for row in s.strip().split(";")]).squeeze()

##
# Encode a Numpy array to Matlab format.
#
# Example:
# ```{.py}
# x = encode_matlab(np.array([1,2,3]))        # "1 2 3"
# A = encode_matlab(np.array([[1,2],[3,4]]))  # "1 2; 3 4"
# ```
#
# @param A numpy.ndarray[float64[m,n]]
# @return str
def encode_matlab(A):
    if len(A.shape) == 1:
        return " ".join(map(str, A.tolist()))
    return "; ".join(" ".join(map(str, row)) for row in A.tolist())

