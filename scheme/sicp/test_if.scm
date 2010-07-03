#!/usr/bin/guile -s
!#

;test a specific idea about if and std procedure
(define (if2 test then-clausole else-clausole)
  (display "test ....")
  (cond (test then-clausole)
        (else else-clausole)))


(define (test1 a)
  (if (< a 0)
      (display "then ...")
      (display "else ...")))

(define (test2 a)
  (if2 (< a 0)
      (display "then2 ...")
      (display "else2 ...")))

;this show why we have a specific if condition and not just a std procedure
(test1 -1)
(display "\n")
(test1 1)
(display "\n")
(test2 -1)
(display "\n")
(test2 1)
(display "\n")
