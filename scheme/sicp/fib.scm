#!/usr/bin/guile -s
!#

;calculate fibonacci series

; 0 1  1 2  3 ->  fib x = (fib x-1) + (fib x -2)


; time O(fib(n))

(define (fib_1 x ) 
  (if (< x 2)
      x
      (+ (fib (- x 1)) (fib (- x 2)))))

;this way make just 1 evaluation of the fibonacci series
;time O(n)
(define (fib_2 x)
  (define (fibo x y z)
    (if (= x 0)
	z
	(fibo (- x 1) (+ y z) y)))
  (fibo x 1 0))



;this is selector for fib implementation tu be tested
(define fib fib_2)

(define (test_fib x)
  (display (fib x))
  (display "\n")
  (if (= x 0)
      "end"
      (test_fib (- x 1))))


(test_fib 35)
