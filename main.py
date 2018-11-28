from random import random
from imageio import imwrite
# import cv2
from math import sin, sqrt, pi, cos
from multiprocessing import Pool
import numpy as np

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

def weight(v, w):
    """
    >>> v1, v2 = vector(1, 2, 3), vector(10, 5, 6)
    >>> weight(v1, v2)
    [10, 10, 18]
    """
    return [v[i] * w[i] for i in range(3)]

def normalize(v):
    return mul(v, 1/norm(v))

def cross(v1, v2):
    return vector(v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2], v1[0] * v2[1] - v1[1] * v2[0])

def scale(color):
    return [int((clamp(e)**(1/2.2))*255+0.5) for e in color]

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

# Constants

INF = float('inf')
WIDTH = 512
HEIGHT = 384
CHANNELS = 3
MAX_DEPTH = 10
SAMPLES = 16
SUBSAMPLES = 2
MAGIC_NUMBER = 0.5135 # fov?
RAY_OFFSET = 140
CAMERA_POS = vector(50,52,295.6)
CAMERA_DIR = normalize(vector(0,-0.042612,-1))
CX = vector(WIDTH * MAGIC_NUMBER / HEIGHT)
CY = mul(normalize(cross(CX, CAMERA_DIR)), MAGIC_NUMBER)
BLACK = vector()
FUDGE = 1e-4


# Global

img = np.zeros((HEIGHT, WIDTH, CHANNELS), dtype=np.uint8)

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
        if t > FUDGE:
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
    for i in range(len(scene)):
        t = intersect(r, scene[i])
        if t < final_t and t > 0:
            final_t = t
            index = i
    return final_t, index

# Scene

spheres = [
   sphere(1e5, vector( 1e5+1,40.8,81.6), vector(),vector(.75,.75,.75),'DIFF'), #Left
   sphere(1e5, vector(-1e5+99,40.8,81.6),vector(),vector(.75,.75,.75),'DIFF'), #Rght 
   sphere(1e5, vector(50,40.8, 1e5),     vector(),vector(.75,.75,.75),'DIFF'), #Back 
   sphere(1e5, vector(50,40.8,-1e5+170), vector(),vector(0.5294, 0.8078, 0.9216),'DIFF'), #Frnt 
   sphere(1e5, vector(50, 1e5, 81.6),    vector(),vector(.75,.75,.75),'DIFF'), #Botm 
   sphere(1e5, vector(50,-1e5+81.6,81.6),vector(),vector(.75,.75,.75),'DIFF'), #Top 
   sphere(16.5,vector(27,45.5,47),       vector(),mul(vector(1,1,1), .999), 'REFR'), #Glas 
   sphere(16.5,vector(73,30,78),       vector(),mul(vector(1,1,1), .999), 'REFR'), #Glas 
   sphere(1.5, vector(50,81.6-16.5,81.6),vector(400,400,400),  vector(), 'DIFF')  #Lite
]

# Convert colors to displayable range
# clamp and gamma correction here

def tent_filter(x):
    assert x >= 0 and x <= 2, "{} not in domain of tent filter".format(x)
    if x < 1:
        return x**0.5 -1
    return 1 - (2 - x)**0.5

def render(y):
    """
    Produces the image
    """
    print('Rendering row: {}'.format(y))
    row = []
    for x in range(WIDTH):
        # print(x, y)
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
                r = vector(clamp(r[0]), clamp(r[1]), clamp(r[2]))
                color = add(color, mul(r, 1/SUBSAMPLES**2)) # need to clamp r before adding
        row.append([x, y, scale(color)])
    return row

def orthonormal_basis(w):
    """
    Takes in a vector w
    Returns a tuple of three vectors w, u, v orthonormal to each other
    """
    u = normalize(cross(vector(0.0, 1.0) if abs(w[0]) > 0.1 else vector(1.0), w))
    v = cross(w, u)
    return w, u, v

def clamp(x):
    if x < 0:
        return 0
    if x > 1:
        return 1
    return x

def radiance(r, depth, emissive=True):
    t, i = intersect_scene(r, spheres)
    if i == -1 or depth > MAX_DEPTH:
        return BLACK
    # Constant shading: for testing purposes only
    # return color(spheres[i])
    s = spheres[i]
    intersection_p = add(origin(r), mul(direction(r), t))
    sphere_normal = normalize(sub(intersection_p, center(s)))
    # nl: Properly oriented version of sphere_normal
    nl = sphere_normal if dot(sphere_normal, direction(r)) < 0 else mul(sphere_normal, -1)
    # Russian Roulette
    surface_color = color(s)
    p = max(surface_color)
    if depth > 5 or p == 0.0:
        if (random() < p): # continue
            surface_color = mul(surface_color, 1 / p)
        else:
            return mul(emission(s), int(emissive))
    if reflection(s) == 'DIFF':
        r1 = 2 * pi * random() # random angle
        r2 = random()
        r2s = sqrt(r2) # random distance
        # Create orthonormal coordinate frame
        w, u, v = orthonormal_basis(nl.copy())
        # Create random direction
        d1 = mul(mul(u, cos(r1)), r2s)
        d2 = mul(mul(v, sin(r1)), r2s)
        d3 = mul(w, sqrt(1 - r2))
        d = normalize(add(d1, add(d2, d3)))

        # Sample lights
        e = vector()
        for l in spheres:
            if max(emission(l)) > 0:
                # Create random direction towards the light
                sw, su, sv = orthonormal_basis(sub(center(l), intersection_p))
                x_sp = sub(intersection_p, center(l))
                cos_a_max = sqrt(1 - radius(l)**2 / dot(x_sp, x_sp))
                eps1, eps2 = random(), random()
                cos_a = 1 - eps1 + eps1 * cos_a_max
                sin_a = sqrt(1 - cos_a**2)
                phi = 2 * pi * eps2
                
                l1 = mul(mul(su, cos(phi)), sin_a)
                l2 = mul(mul(sv, sin(phi)), sin_a)
                l3 = mul(sw, cos_a)
                dl = normalize(add(l1, add(l2, l3)))

                # Shadow ray
                _, shadow_i = intersect_scene(ray(intersection_p, dl), spheres)
                if shadow_i != -1 and spheres[shadow_i] is l:
                    omega = 2 * pi * (1 - cos_a_max)
                    e = add(e, mul(weight(surface_color, mul(emission(l), dot(dl, nl) * omega)), 1/pi))
        self_emission = mul(emission(s), int(emissive))
        more_bounces = weight(surface_color, radiance(ray(intersection_p, d), depth + 1, False))
        return add(self_emission, add(more_bounces, e))
    elif reflection(s) == 'SPEC':
        new_d = sub(direction(r), mul(sphere_normal, 2 * dot(direction(r), sphere_normal)))
        return add(emission(s), weight(surface_color, radiance(ray(intersection_p, new_d), depth + 1, True)))
    # reflection(s) == 'REFR'
    new_d = sub(direction(r), mul(sphere_normal, 2 * dot(direction(r), sphere_normal)))
    refl_ray = ray(intersection_p, new_d)
    into = dot(sphere_normal, nl) > 0
    nc = 1.0
    nt = 1.5 # IOR for glass
    nnt = nc/nt if into else nt/nc
    ddn = dot(direction(r), nl)
    cos2t = 1- nnt * nnt * (1- ddn * ddn)
    if cos2t < 0: # total internal reflection
        return add(emission(s), weight(surface_color, radiance(refl_ray, depth + 1, True)))
    tdir = normalize(sub(mul(direction(r), nnt), mul(sphere_normal, ((1 if into else -1)*(ddn*nnt+sqrt(cos2t))))))
    a=nt-nc
    b=nt+nc
    R0=a*a/(b*b) # reflectance at normal incidence
    c = 1 - (-ddn if into else dot(tdir, sphere_normal))
    Re = R0 + (1 - R0) * (c**5) # Fresnel reflectance
    Tr = 1 - Re
    P = .25 + .5 * Re
    RP = Re / P
    TP= Tr / (1 - P)
    if depth > 2:
        if random() < p:
            rest = mul(radiance(refl_ray, depth + 1, True), RP)
        else:
            rest = mul(radiance(ray(intersection_p, tdir), depth + 1, True), TP)
    else:
        rest = add(mul(radiance(refl_ray, depth + 1, True), Re),
            mul(radiance(ray(intersection_p, tdir), depth + 1, True), Tr))
    return add(emission(s), weight(surface_color, rest))

if __name__ == '__main__':
    with Pool() as pool:
        rows = pool.map(render, range(HEIGHT))
    print('dumping')
    for r in rows:
        for p in r:
            try:
                right, down = WIDTH - p[0] - 1, HEIGHT - p[1] - 1
                img[down][right][0] = p[2][0]
                img[down][right][1] = p[2][1]
                img[down][right][2] = p[2][2]
            except IndexError:
                print(p)

# cv2.imshow('img', img)
# cv2.waitKey()

# Save the image
imwrite("image.png", img)
