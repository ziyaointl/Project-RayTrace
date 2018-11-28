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

(define (draw)
  
  (exitonclick))

; Please leave this last line alone.  You may add additional procedures above
; this line.
(draw)