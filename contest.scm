;;; Scheme Recursive Art Contest Entry
;;;
;;; Please do not include your name or personal info in this file.
;;;
;;; Title: Random
;;;
;;; Description:
;;;    If I could use Monte Carlo on a test
;;;    I can approximate anything
;;;    By guessing a bajillion times

;;; Vector

(define (cadr lst)
  (car (cdr lst))
)

(define (caddr lst)
  (car (cdr (cdr lst)))
)

(define (vector x y z)
  (list x y z)
)

(define (add v1 v2)
  (vector (+ (car v1) (car v2)) (+ (cadr v1) (cadr v2)) (+ (caddr v1) (caddr v2)))
)

(define (sub v1 v2)
  (vector (- (car v1) (car v2)) (- (cadr v1) (cadr v2)) (- (caddr v1) (caddr v2)))
)

(define (mul v1 x)
  (vector (* (car v1) x) (* (cadr v1) x) (* (caddr v1) x))
)

(define (dot v1 v2)
  (+ (* (car v1) (car v2)) (* (cadr v1) (cadr v2)) (* (caddr v1) (caddr v2)))
)

(define (norm v)
  (expt (dot v v) 0.5)
)

(define (normalize v)
  (mul v (/ 1 (norm v)))
)

(define (cross v1 v2)
  (define x (- (* (cadr v1) (caddr v2)) (* (caddr v1) (cadr v2))))
  (define y (- (* (caddr v1) (car v2)) (* (car v1) (caddr v2))))
  (define z (- (* (car v1) (cadr v2)) (* (cadr v1) (car v2))))
  (vector x y z)
)

;;; Ray
(define (ray o d)
  (list o d)
)

(define (origin r)
  (car r)
)

(define (direction r)
  (cadr r)
)

;;; Sphere

(define (sphere r p emission color reflection)
  (list r p emission color reflection)
)

(define (radius s)
  (car s)
)

(define (position s)
  (cadr s)
)

(define (emission s)
  (caddr s)
)

(define (color s)
  (cadddr s)
)

(define (reflection s)
  (car (cddddr s))
)

;;; Constants
(define WIDTH 512)
(define HEIGHT 384)
(define CHANNELS 3)
(define MAX_DEPTH 10)
(define SAMPELS 16)
(define SUBSAMPLES 2)
(define MAGIC_NUMBER 0.5135)
(define RAY_OFFSET 140)
(define CAMERA_POS (vector 50 52 295.6))
(define CAMERA_DIR (normalize (vector 0 -0.042612 -1)))
(define CX (vector (/ (* WIDTH MAGIC_NUMBER) HEIGHT) 0 0))
(define CY (mul (normalize (cross CX CAMERA_DIR)) MAGIC_NUMBER))
(define RAY_OFFSET 140)
(define BLACK (vector 0 0 0))
(define FUDGE 1e-4)
(define INF (inf))

;;; Scene

(define scene ())
(define scene (cons (sphere 1e5 (vector (+ 1e5 1) 40.8 81.6) (vector 0 0 0) (vector .75 .75 .75) 'DIFF) scene))
(define scene (cons (sphere 1e5 (vector (+ 99 (- 1e5)) 40.8 81.6) (vector 0 0 0) (vector .75 .75 .75) 'DIFF) scene))
(define scene (cons (sphere 1e5 (vector 50 40.8 1e5) (vector 0 0 0) (vector .75 .75 .75) 'DIFF) scene))
(define scene (cons (sphere 1e5 (vector 50 40.8 (+ 170 (- 1e5))) (vector 0 0 0)  (vector 0.5294 0.8078 0.9216) 'DIFF) scene))
(define scene (cons (sphere 1e5 (vector 50 1e5 81.6) (vector 0 0 0) (vector .75 .75 .75) 'DIFF) scene))
(define scene (cons (sphere 1e5 (vector 50 (+ 81.6 (- 1e5)) 81.6)  (vector 0 0 0)  (vector .75 .75 .75) 'DIFF) scene))
(define scene (cons (sphere 16.5 (vector 27 45.5 47) (vector 0 0 0) (mul (vector 1 1 1) .999) 'REFR) scene))
(define scene (cons (sphere 16.5 (vector 73 30 78) (vector 0 0 0) (mul (vector 1 1 1) .999) 'REFR) scene))
(define scene (cons (sphere 1.5 (vector 50 65.1 81.6)  (vector 400 400 400) (vector 0 0 0)  'DIFF) scene))

;;; Intersect

(define (intersect r s)
  (define o_c (sub (origin r) (center s)))
  (define a (dot (direction r) (direction r)))
  (define b (* 2 (dot o_c (direction r))))
  (define c (- (dot o_c o_c) (expt (radius s) 2)))
  (define dis (- (* b b) (* 4 a c)))
  (if (< dis 0)
    0
    (begin
      (define dis (expt dis 0.5))
      (cond ((> (/ (- (- b) dis) (* 2 a)) FUDGE) (/ (- (- b) dis) (* 2 a)))
        ((> (/ (+ (- b) dis) (* 2 a)) FUDGE) (/ (+ (- b) dis) (* 2 a)))
        (else 0)
      )
    )
  )
)

(define (intersect_scene r scene)
  (define final_t inf)
  (define (helper final_t obj scene)
    (if (null? scene)
      (list final_t obj)
      (define t (intersect r (car scene)))
      (if (and (< t final_t) (> t 0))
        (helper t (car scene) (cdr scene))
      )
    )
  )
  (helper 0 '() scene)
)

(define (draw)
  
  (exitonclick))

; Please leave this last line alone.  You may add additional procedures above
; this line.
(draw)