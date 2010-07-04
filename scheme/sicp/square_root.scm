#!/usr/bin/guile -s
!#

;calculate square root with babilonian method

;we need some math aid

(define (abs x) 
  (if (< x 0)
      (- x)
      x))

(define (average x y)
  (/ (+ x y) 2))
;we need square to check the guess

(define (square x) (* x x))

;define the "is aceptable" meaning

(define (is_aceptable? guess x)
  (< (abs (- (square guess) x)) .000000000000001))

;define how to improve guess

(define (improve guess x)
  (average guess (/ x guess)))

; try is the function that test if a guess is close enought to the real square root
(define (try guess x)
  (if (is_aceptable? guess x)
      guess
      (try (improve guess x) x)))


; define square root of x as initial try of one
(define (sqrt x) (try 1.0 x))

(display (sqrt 4))
(display "\n")