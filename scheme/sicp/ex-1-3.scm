#!/usr/bin/guile -s
!#

;1.3.  Define a procedure that takes three numbers as arguments and returns the sum of the squares of the two larger numbers.
(define (sqr a)
  (* a a))


(define (sum_sqr a b)
  (display a)
  (display " ")
  (display b)
  (display "\n")
  (+ (sqr a) (sqr b)))

(define (sum_sqr_better a b c)
  (cond ((and (< a b) (< c a)) (sum_sqr a b))
	((and (< a b) (< a c)) (sum_sqr c b))
	((and (< b a) (< b c)) (sum_sqr a c))
	((and (< b a) (< c b)) (sum_sqr a b))
	))


(display (sum_sqr_better 2 3 4))
(display "\n")
(display (sum_sqr_better 3 2 4))
(display "\n")
(display (sum_sqr_better 4 2 3))
(display "\n")
(display (sum_sqr_better 4 3 2))
(display "\n")