#!/usr/bin/guile -s
!#

; computing way of changing 100 coins 



(define (cv2val kinds-of-coins)
  (cond ((= kinds-of-coins 1) 1)
        ((= kinds-of-coins 2) 5)
        ((= kinds-of-coins 3) 10)
        ((= kinds-of-coins 4) 25)
        ((= kinds-of-coins 5) 50)))



(define (cc amount kind_of_coins)
  (cond ((= amount 0) 1)
	((or (< amount 0) (< kind_of_coins 1)) 0)
	(else 
	 (+ (cc amount (- kind_of_coins 1))
	    (cc (- amount (cv2val kind_of_coins))  kind_of_coins)))))

(define (exchange amount) (cc amount 5))


(display (exchange 100))
(display "\n")