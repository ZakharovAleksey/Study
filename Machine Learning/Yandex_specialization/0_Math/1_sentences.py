import numpy as np
import os
import re
import collections

path = os.path.join(os.getcwd(), 'data', 'sentences.txt')


def get_data(path):
    if os.path.exists(path):
        with open(path, 'r') as f:
            return f.readlines()
    return []


if __name__ == '__main__':
    data = get_data(path)
    s = set()
    for line in data:
        s = s.union(
            {word
             for word in re.split('[^a-z]', line.lower()) if word})
    d = {word: id for id, word in enumerate(s)}
