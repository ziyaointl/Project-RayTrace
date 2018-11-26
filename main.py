from random import random
from turtle import *
from tkinter import *
from math import sin, sqrt

# Vector arithmetic

def vector(x=0.0, y=0.0, z=0.0):
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

# Turtle graphics

def init_canvas():
    canvas = getcanvas()
    image = PhotoImage(width=WIDTH, height=HEIGHT)
    canvas.create_image((0, 0), image=image, state='normal')
    bgcolor("#000000")
    return image

def pixel(x, y, color):
    right, down = x, HEIGHT - y
    scaled = tuple(int(x * 255) for x in color)
    color = "#%02x%02x%02x" % scaled
    IMAGE.put(color, (right, down))

# Constants

INF = float('inf')
WIDTH = 512
HEIGHT = 384
SAMPLES = 1
SUBSAMPLES = 2
MAGIC_NUMBER = 0.5135 # fov?
RAY_OFFSET = 140
CAMERA_POS = vector(50,52,295.6)
CAMERA_DIR = normalize(vector(0,-0.042612,-1))
CX = vector(WIDTH * MAGIC_NUMBER / HEIGHT)
CY = mul(normalize(cross(CX, CAMERA_DIR)), MAGIC_NUMBER)
BLACK = vector()
IMAGE = init_canvas()

# Intersect

def intersect(r, s):
    """
    Takes in a ray and a sphere, returns t if intersects and 0 otherwise
    """
    o_c = sub(origin(r), center(s))
    a = dot(direction(r), direction(r))
    b = 2 * dot(direction(r), o_c)
    c = dot(o_c, o_c) - radius(s)**2
    dis = (b * b) - (4 * a * c)
    if dis < 0:
        return 0
    dis = dis**0.5
    for t in [(-b - dis) / (2 * a), (-b + dis) / (2 * a)]:
        if t > 0:
            return t
    return 0

def intersect_scene(r, scene):
    """
    Takes in a scene (a list of sphere) and a ray
    Return t and the corresponding index of the closest sphere
    If no sphere is found, return -1 as the index
    """
    final_t = INF
    index = -1
    for i in range(scene.size()):
        t = intersect(r, scene[i])
        if t < final_t:
            final_t = t
            index = i
    return final_t, index

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

# Convert colors to displayable range
# clamp and gamma correction here

def tent_filter(x):
    assert x >= 0 and x <= 2, "{} not in domain of tent filter".format(x)
    if x < 1:
        return x**0.5 -1
    return 1 - (2 - x)**0.5

def render():
    """
    Produces the image
    """
    for y in range(HEIGHT):
        for x in range(WIDTH):
            color = vector()
            for sy in range(SUBSAMPLES):
                for sx in range(SUBSAMPLES):
                    r = vector() # radiance
                    for s in range(SAMPLES):
                        dx = tent_filter(2 * random())
                        dy = tent_filter(2 * random())
                        x_ratio = (((sx + 0.5 + dx) / 2 + x) / WIDTH - 0.5)
                        y_ratio = (((sy + 0.5 + dy) / 2 + y) / HEIGHT - 0.5)
                        d = add(CAMERA_DIR, add(mul(CX, x_ratio), mul(CY, y_ratio)))
                        
                        o = add(CAMERA_POS, mul(d, RAY_OFFSET))
                        d = normalize(d)
                        r = add(r, mul(radiance(ray(o, d), 0), 1/SAMPLES))
                    color = add(color, mul(r, 1/SUBSAMPLES**2)) # need to clamp r before adding
            # write pixel to the screen

def radiance(r, depth, emissive=True):
    return vector()