#!/usr/bin/guile -s
!#

;A function f is defined by the rule that f(n) = n if n<3 and f(n) = f(n - 1) + 2f(n - 2) + 3f(n - 3) if n> 3. Write a procedure that computes f by means of a recursive process. Write a procedure that computes f by means of an iterative process.

; f(n) = n if n<3
; f(n) = f(n - 1) + 2f(n - 2) + 3f(n - 3) if n >= 3

(define (f_rec n)
  (if (< n 3)
      n
      (+ (f_rec (- n 1))
	 (* 2 (f_rec (- n 2)))
	 (* 3 (f_rec (- n 3)))
	 )))

(define (f_iter n)
  (define (f_internal count step-1 step-2 step-3)
    (if (= count 0)
	step-1
	(f_internal (- count 1) 
		   (+ step-1 (* 2 step-2) (* 3  step-3))
		   step-1
		   step-2)))
  (if (< n 3)
      n
      (f_internal (- n 2) 2 1 0 ))
  )


(define f f_iter)

(define (test x)
  (if (= x 0)
      (display "test done \n")
      ((display (f x))
       (display "\n")
       (test (- x 1)))
      ))

(test 30)