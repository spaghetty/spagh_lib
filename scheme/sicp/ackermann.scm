#!/usr/bin/guile -s
!#

(define (akm x y)
  (display x)
  (display " ")
  (display y)
  (display "\n")
  (cond ((= y 0) 0)
        ((= x 0) (* 2 y))
        ((= y 1) 2)
        (else (akm (- x 1)
                 (akm x (- y 1))))))

;(akm 1 10)

;(akm 1 9)(akm 1 8)(akm 1 7)(akm 1 6)(akm 1 5)
;;;(akm 1 4)(akm 1 3)(akm 1 2)(akm 1 1)

;(akm 1 9)(akm 1 8)(akm 1 7)(akm 1 6)(akm 1 5)(akm 1 4)(akm 1 3)(akm 1 2) 2


;(akm 0 2) => 4

(display (akm 1 10))
(display "\n")

;(display (akm 2 4))
;(display "\n")

;(display (akm 3 3))
;(display "\n")