(use bignum.smt)
(use mixnum.smt)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;;  Solution to Exercise T

; Summary: Checking if (200 - 200) * 112139 = 0, natural class
(check-print (((Natural fromSmall: 200) - 
             (Natural fromSmall: 200)) * (Natural fromSmall: 112139)) 0)  

; Summary: Checking if (160 * 160) < (159 * 161), natural class
(check-print (((Natural fromSmall: 160) * (Natural fromSmall: 160)) < 
              ((Natural fromSmall: 161) * (Natural fromSmall: 159))) <False>)

; Summary: 333 divided by (11 mod 11), expect 0, natural class
(check-error ((Natural fromSmall: 333) div: 
              ((Natural fromSmall: 11) mod: -11)))

;---------------------------------------------------------

; Summary: (999 * 999) mod 4, large integers
(check-print (((LargeInteger fromSmall: 999) * 
                 (LargeInteger fromSmall: 999)) mod: 4) 1)

; Summary: ((-12 + (-2 * -6)) + 0 - 1) = -1, large integers
(check-print ((((LargeInteger fromSmall: -12) + 
             ((LargeInteger fromSmall: -2) * (LargeInteger fromSmall: -6)))
               + ((LargeInteger fromSmall: -0) negated)) 
               + (LargeInteger fromSmall: -1)) -1) 

; Summary: ((((81 * -81) / 9) / 9) / 9) - 91, large integers
(check-print ((((((LargeInteger fromSmall: 81) * 
                (LargeInteger fromSmall: -81)) div: 9) div: 9) div: 9) - 
                    (LargeInteger fromSmall: 91)) -100)

                
;---------------------------------------------------------------------------

(Integer addSelector:withMethod: 'times10
    (compiled-method () [locals two four]
        (set two (self + self))
        (set four (two + two))
        ((four + four) + two)))
        
(Natural addSelector:withMethod: 'raisedToInteger:
    (Number compiledMethodAt: 'raisedToInteger:))


; Summary:(-2^43 + 2^43 )= 0, mixed arithmetic
(check-print ((-2 raisedToInteger: 43) + (2 raisedToInteger: 43)) 0 )

; Summary; (1234561 * 10 * 10 * 10 * 10) = 12345610000, mixed arithmetic
(check-print ((((1234561 times10) times10) times10) times10) 12345610000) 

; Summary: (1 - (-9^9)) = 387420490, mixed arithmetic
(check-print (1 - (-9 raisedToInteger: 9)) 387420490)

