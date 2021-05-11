;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Exercise 36(a)
;;  Eli Dow

;; addition
(Fraction addSelector:withMethod: '+
    (compiled-method (x) 
        [locals temp]
        (set x (self coerce: x))
        (set temp (den lcm: (x den)))             ;; rest from Fraction
        (((Fraction new) setNum:den: 
                        ((num * (temp div: den)) +
                         ((x num) * (temp div: (x den))))
                         temp)
            divReduce)))

        ;; unit tests
        (check-expect ((1 / 2) + 4) (9 / 2))
        (check-expect ((3 / 4) + 3) (15 / 4))
        (check-expect ((5 / 8) + -1) (-3 / 8))


;; subtraction
(Fraction addSelector:withMethod: '-
    (compiled-method (x)
        (set x (self coerce: x))
        (self + (x negated))))

        ;; unit tests
        (check-expect ((7 / 2) - 3) (1 / 2))
        (check-expect ((1 / 2) - 3) (-5 / 2))
        (check-expect ((1 / 5) - -3) (16 / 5))


;; multiplication
(Fraction addSelector:withMethod: '*
    (compiled-method (x)
        (set x (self coerce: x))
        (((Fraction new) setNum:den:              ;; rest from Fraction
                         (num * (x num)) (den * (x den))) divReduce)))

        ;; unit tests
        (check-expect ((1 / 2) * 3) (3 / 2))
        (check-expect ((2 / 3) * 10) (20 / 3))
        (check-expect ((3 / 4) * -4) -3)


;; division
(Fraction addSelector:withMethod: 'div:
    (compiled-method (x)
        (set x (self coerce: x))
        (self * (x reciprocal))))

        ;; unit tests
        (check-expect ((3 / 2) div: 3) (1 / 2))
        (check-expect ((1 / 2) div: 3) (1 / 6))
        (check-expect ((9 / 4) div: -3) (-3 / 4))


;; equals
(Fraction addSelector:withMethod: '=
    (compiled-method (x)
        (set x (self coerce: x))
        ((num = (x num)) and: {(den = (x den))}))) ;; rest from Fraction

        ;; unit tests
        (check-expect ((6 / 2) = 3) true)
        (check-expect ((10 / 2) = 10) false)
        (check-expect ((-20 / 2) = -10) true)
        (check-expect ((-10 / 2) = 10) false)


;; less than (don't need greater than)
(Fraction addSelector:withMethod: '<
    (compiled-method (x)
        (set x (self coerce: x))
        ((num * (x den)) < ((x num) * den))))     ;; rest from Fraction

        ;; unit tests
        (check-expect ((1 / 2) < 3) true)
        (check-expect ((11 / 2) < 5) false)
        (check-expect ((-11 / 2) < -5) true)
        (check-expect ((-9 / 2) < -5) false)