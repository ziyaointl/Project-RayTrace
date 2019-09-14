;;; Scheme Recursive Art Contest Entry
;;;
;;; Please do not include your name or personal info in this file.
;;;
;;; Title: rand()
;;;
;;; Description:
;;;    If I could use Monte Carlo on a test
;;;    I would approximate everything
;;;    By guessing a bajillion times

;;; Vector

(define (cadr lst)
  (car (cdr lst))
)

(define (caddr lst)
  (car (cdr (cdr lst)))
)

(define (cadddr lst)
  (car (cdr (cdr (cdr lst))))
)

(define (cddddr lst)
  (cdr (cdr (cdr (cdr lst))))
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

(define (center s)
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
(define SUBSAMPLES '((0 0) (0 1) (1 0) (1 1)))

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
      0
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
      (begin
        (define t (intersect r (car scene)))
        (if (and (< t final_t) (> t 0))
          (helper t (car scene) (cdr scene))
          (helper final_t obj (cdr scene)))
      )
    )
  )
  (helper 0 '() scene)
)

(define (tent-filter x)
  (if (< x 1)
    (- (expt x 0.5) 1)
    (- 1 (expt (- 2 x) 0.5))
  )
)

(define (clamp v)
  (map (lambda (e) (if (> e 1) 1.0 (if (< e 0) 0 e))) v)
)

(define (render w h)
  (render-pixel w h)
  (if (< w WIDTH)
    (render (+ w 1) h)
    (if (< h HEIGHT)
      (render 0 (+ h 1))
    )
  )
)

(define (render-pixel x y)
  (define (helper subsamples-lst color)
    (if (null? subsamples-lst) color
      (begin
        (define sx (car (car subsamples-lst)))
        (define sy (car (cdr (car subsamples-lst))))
        (define (helper2 i r)
          (if (eq? i SAMPELS)
            r
            (begin
              (define dx (tent-filter (* 2 (random))))
              (define dy (tent-filter (* 2 (random))))
              (define x-ratio (- (/ (+ (/ (+ sx 0.5 dx) 2) x) WIDTH) 0.5))
              (define y-ratio (- (/ (+ (/ (+ sy 0.5 dy) 2) y) HEIGHT) 0.5))
              (define d (add CAMERA_DIR (add (mul CX x-ratio) (mul CY y-ratio))))
              (define o (add CAMERA_POS (mul d RAY_OFFSET)))
              (define d (normalize d))
              (print (mul (radiance (ray o d) 0 #t) 1))
              (print (add r (mul (radiance (ray o d) 0 #t) (/ 1 SAMPELS))))
              (helper2 (+ i 1) (add r (mul (radiance (ray o d) 0 #t) (/ 1 SAMPELS))))
            )
          )
        )
        (define r (clamp (helper2 0 (vector 0 0 0))))
        ; (helper (cdr subsamples-lst) (add color (mul r (expt (/ 1 4) 2))))
      )
    )
  )
  (helper SUBSAMPLES (vector 0 0 0))
  ; (draw-pixel x y )
)

(define (radiance r depth emissive)
  (print (intersect_scene r scene))
  ; (define obj )
  ; (if (null? obj) BLACK (color obj))
)

(define (draw)
  (render 0 0)
  (exitonclick))

; Please leave this last line alone.  You may add additional procedures above
; this line.
(draw)