# Vector arithmetic

def vector(x=0, y=0, z=0):
    """
    >>> vector(1, 2, 3)
    [1, 2, 3]
    """
    return [x, y, z]

def add(v1, v2):
    """
    >>> v1, v2 = vector(1, 2, 3), vector(4, 5, 6)
    >>> add(v1, v2)
    [5, 7, 9]
    """
    return [(v1[i] + v2[i]) for i in range(3)]

def sub(v1, v2):
    """
    >>> v1, v2 = vector(1, 2, 3), vector(4, 5, 6)
    >>> sub(v1, v2)
    [-3, -3, -3]
    """
    return [v1[i] - v2[i] for i in range(3)]

def mul(v, x):
    """
    >>> v = vector(2, 5, 9)
    >>> mul(v, 3)
    [6, 15, 27]
    """
    return [v[i] * x for i in range(3)]

def dot(v1, v2):
    """
    >>> v1, v2 = vector(1, 2, 3), vector(10, 5, 6)
    >>> dot(v1, v2)
    38
    """
    return sum([v1[i] * v2[i] for i in range(3)])

def norm(v):
    """
    >>> norm(vector(2**0.5, 3, 5))
    6.0
    """
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

# Sphere

def sphere(r, p, emission, color, reflection):
    return [r, p, emission, color, reflection]

def radius(s):
    return s[0]

def center(s):
    return s[1]

def emission(s):
    return s[2]

def color(s):
    return s[3]

def reflection(s):
    return s[4]

# Intersect

def intersect(r, s):
    """
    Takes in a ray and a sphere, returns t if intersects and 0 otherwise
    """
    o_c = sub(origin(r), center(s))
    a = dot(direction(r), direction(r))
    b = 2 * dot(direction(r), o_c)
    c = dot(o_c, o_c) - r * r 
    dis = (b * b) - (4 * a * c)
    if dis < 0:
        return 0
    for t in [(-b - dis) / (2 * a), (-b + dis) / (2 * a)]:
        if t > 0:
            return t

# Scene

spheres = [
   sphere(1e5, vector( 1e5+1,40.8,81.6), vector(),vector(.75,.25,.25),'DIFF'), #Left
   sphere(1e5, vector(-1e5+99,40.8,81.6),vector(),vector(.25,.25,.75),'DIFF'), #Rght 
   sphere(1e5, vector(50,40.8, 1e5),     vector(),vector(.75,.75,.75),'DIFF'), #Back 
   sphere(1e5, vector(50,40.8,-1e5+170), vector(),vector(),           'DIFF'), #Frnt 
   sphere(1e5, vector(50, 1e5, 81.6),    vector(),vector(.75,.75,.75),'DIFF'), #Botm 
   sphere(1e5, vector(50,-1e5+81.6,81.6),vector(),vector(.75,.75,.75),'DIFF'), #Top 
   sphere(16.5,vector(27,16.5,47),       vector(),vector(1,1,1)*.999, 'SPEC'), #Mirr 
   sphere(16.5,vector(73,16.5,78),       vector(),vector(1,1,1)*.999, 'REFR'), #Glas 
   sphere(600, vector(50,681.6-.27,81.6),vector(12,12,12),  vector(), 'DIFF')  #Lite
]

