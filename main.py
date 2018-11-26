# Vector arithmetic

def vector(x, y, z):
    return [x, y, z]

def add(v1, v2):
    return [v1[i] + v2[i] for i in range(3)]

def sub(v1, v2):
    return [v1[i] - v2[i] for i in range(3)]

def mul(v, x):
    return [v[i] * x for i in range(3)]

def dot(v1, v2):
    return sum([v1[i] * v2[i] for i in range(3)])

def norm(v):
    return dot(v, v)**0.5

def normalize(v):
    return mul(v, 1/norm(v))

def cross(v1, v2):
    return vector(v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2], v1[0] * v2[1] - v1[1] * v2[0])

# Ray

def ray(o, d):
    return [o, d]

def origin(r):
    return r[0]

def direction(r):
    return r[1]

