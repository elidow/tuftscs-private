(use bignum.smt)
(use factorial.smt)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;;  Solution to Exercise 39


(SmallInteger addSelector:withMethod: 'asLargeInteger
    (compiled-method () (LargeInteger fromSmall: self)))


(LargeInteger addSelector:withMethod: 'asLargeInteger
    (compiled-method () self))

;; multiplication


(SmallInteger addSelector:withMethod: '* 
    (compiled-method (anInteger) (anInteger multiplyBySmallInteger: self)))


(SmallInteger addSelector:withMethod: 'multiplyBySmallInteger: 
    (compiled-method (anInteger) 
        ((primitive mulWithOverflow self anInteger 
            {((self asLargeInteger) * anInteger)})
               value)))


(SmallInteger addSelector:withMethod: 'multiplyByLargePositiveInteger:
    (compiled-method (aLargeInt) [locals largeself]
        (set largeself (self asLargeInteger))
        ((largeself isNegative) ifTrue:ifFalse:
            {  
            ((aLargeInt isZero) ifTrue:ifFalse:
                {(LargePositiveInteger fromSmall: 0)}
                {(LargeNegativeInteger withMagnitude: 
                    ((largeself magnitude) * (aLargeInt magnitude)))})}
            {  (LargePositiveInteger withMagnitude: 
            ((aLargeInt magnitude) * (largeself magnitude))) })
    )
)


(SmallInteger addSelector:withMethod: 'multiplyByLargeNegativeInteger:
    (compiled-method (aLargeInt) [locals largeself]
        (set largeself (self asLargeInteger))
        ((largeself isNegative) ifTrue:ifFalse:
            {  
             (LargePositiveInteger withMagnitude: 
                ((largeself magnitude) * (aLargeInt magnitude))) 
            }
            {  ((largeself isZero) ifTrue:ifFalse:
                    {(LargePositiveInteger fromSmall: 0)}
                    {(LargeNegativeInteger withMagnitude: 
                        ((aLargeInt magnitude) * (largeself magnitude) ))})})
    )
)


(LargeInteger addSelector:withMethod: 'multiplyBySmallInteger:
    (compiled-method (anInteger) (self * (anInteger asLargeInteger))))

;; addition


(SmallInteger addSelector:withMethod: '+
    (compiled-method (anInteger) (anInteger addSmallIntegerTo: self)))


(SmallInteger addSelector:withMethod: 'addSmallIntegerTo: 
    (compiled-method (anInteger) 
        ((primitive addWithOverflow self anInteger 
            {  ((self asLargeInteger) + anInteger)})
               value)))


(SmallInteger addSelector:withMethod: 'addLargePositiveIntegerTo:
    (compiled-method (aLargeInt) [locals smag imag largeself]
        (set largeself (self asLargeInteger))
        ((largeself isNegative) ifTrue:ifFalse:
            {(set smag (largeself magnitude))
                (set imag (aLargeInt magnitude))
                ((imag < smag) ifTrue:ifFalse:
                    {(LargeNegativeInteger withMagnitude: (smag - imag))}
                    {(LargePositiveInteger withMagnitude: (imag - smag))})}
            {(LargePositiveInteger withMagnitude: 
                ((largeself magnitude) + (aLargeInt magnitude)))}
        )
    )
)

(SmallInteger addSelector:withMethod: 'addLargeNegativeIntegerTo:
    (compiled-method (aLargeInt) [locals smag imag largeself]
        (set largeself (self asLargeInteger))
        ((largeself isNegative) ifTrue:ifFalse:
            {(LargeNegativeInteger withMagnitude: 
                ((largeself magnitude) + (aLargeInt magnitude)))}
            {   (set smag (largeself magnitude))
                (set imag (aLargeInt magnitude))
                ((smag < imag) ifTrue:ifFalse:
                    {(LargeNegativeInteger withMagnitude: (imag - smag))}
                    {(LargePositiveInteger withMagnitude: (smag - imag))})}
        )
    )
)


(LargeInteger addSelector:withMethod: 'addSmallIntegerTo:
    (compiled-method (anInteger) (self + (anInteger asLargeInteger))))

;; subtraction and negation


(SmallInteger addSelector:withMethod: '-
    (compiled-method (anInteger) 
        (self + (anInteger negated))))

(SmallInteger addSelector:withMethod: 'negated
     (compiled-method () 
        ((primitive subWithOverflow 0 self
            {((0 asLargeInteger) - (self asLargeInteger))})
               value)))


;; extra credit

(SmallInteger addSelector:withMethod: '> 
    (compiled-method (n) (n < self)))


(SmallInteger addSelector:withMethod: '<
    (compiled-method (n) (n smallerThanSmall: self)))


(SmallInteger addSelector:withMethod: 'smallerThanSmall:
    (compiled-method (n) (primitive > self n)))


(LargeInteger addSelector:withMethod: 'smallerThanSmall:
    (compiled-method (n) (self > n)))


(SmallInteger addSelector:withMethod: '=
    (compiled-method (n) (n eqToSmall: self)))


(SmallInteger addSelector:withMethod: 'eqToSmall:
    (compiled-method (n) (primitive sameObject self n)))


(LargeInteger addSelector:withMethod: 'eqToSmall:
    (compiled-method (n) ((self - n) isZero)))

