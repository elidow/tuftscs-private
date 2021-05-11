;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;;  Solution to Exercise 37

;; Natural Class
(class Natural
   [subclass-of Magnitude]
   ;;16384
    (class-method base () 16384);;Private

    (class-method fromSmall: (anInteger)
        ((anInteger < 0) ifTrue:ifFalse:
            {(self error: 'natural-cannot-be-negative)}
            {((super new) init: anInteger)})
    )

    (method validated ()
        ((self invariant) ifFalse:
        {(self printrep)
        (self error: 'invariant-violation)})
        self)

    (method init: (anInteger)  [locals q r]
        ((anInteger = 0) ifTrue:ifFalse:
            {(NatZero new)}
            {(set q (Natural fromSmall: (anInteger div: (Natural base))))
             (set r (anInteger mod: (Natural base)))
                (Natural first:rest: r q)}))

    (class-method first:rest: (anInteger aNatural)
            (((anInteger = 0) and: {(aNatural isZero)})
            ifTrue:ifFalse:
            {(NatZero new)}
            {((NatNonZero create:nonzero: anInteger aNatural) validated)})
    )
   
    (method = (aNatural) 
        ((self compare:withLt:withEq:withGt: aNatural 
        {false} {true} {false}) value))

    (method < (aNatural) 
        ((self compare:withLt:withEq:withGt: aNatural 
        {true} {false} {false}) value))


    (method + (aNatural) (self plus:carry: aNatural 0))
    (method * (aNatural) (self subclassResponsibility))
    (method - (aNatural)
        (self subtract:withDifference:ifNegative:
            aNatural
            [block (x) x]
            {(self error: 'Natural-subtraction-went-negative)}))

    (method sdiv: (n) (self sdivmod:with: n [block (q r) q]))
    (method smod: (n) (self sdivmod:with: n [block (q r) r]))
    (method sdivmod:with: (n aBlock) (self subclassResponsibility))

    (method decimal () (self subclassResponsibility))
    (method isZero  () (self subclassResponsibility))

    (method print   () ((self decimal) do: [block (x) (x print)]))

    (method invariant () (self subclassResponsibility))
    (method printrep ()  (self subclassResponsibility))


    ;;page 681
    (method modBase () (self subclassResponsibility))    ;;page 681
    (method divBase () (self subclassResponsibility))    ;;page 681
    (method timesBase () (self subclassResponsibility))  ;;page 681
    (method compare:withLt:withEq:withGt:                ;;page 681
        (aNatural ltBlock eqBlock gtBlock) (self subclassResponsibility))
    (method plus:carry: (aNatural c) (self subclassResponsibility))
    (method minus:borrow: (aNatural c) (self subclassResponsibility))

    (method subtract:withDifference:ifNegative: (aNatural diffBlock exnBlock)
        ((self < aNatural) ifTrue:ifFalse:
            exnBlock
            {(diffBlock value: (self minus:borrow: aNatural 0))}
        )
    )

    (method decimal () [locals temp retList]
            (set temp self)
            (set retList (List new))
            ({(temp isZero)} whileFalse:
                {
                (retList addFirst: (temp smod: 10))
                (set temp (temp sdiv: 10))
                }
            )
            ((retList isEmpty) ifTrue:ifFalse:
                    {(retList add: 0)}
                    {retList}
            )
    )

    (method compare-symbol: (aNat)
        (self compare:withLt:withEq:withGt: aNat {'LT} {'EQ} {'GT}))
)


;; NatZero class
;;A (Zero) = NatZero
;;I (Zero) = isZero
(class NatZero 
    [subclass-of Natural]
    
    (method * (aNatural) self)
    
    (method sdivmod:with: (n aBlock) 
        ((n == 0) ifTrue:ifFalse:
          {(self error: 'cannot-divide-0-by-0)}
          {(aBlock value:value: (NatZero new) 0)})
    )

    
    (method isZero () true)
    (method invariant () true)

    (method printrep () ('0 print))

    (method modBase () 0)
    (method divBase () self)
    (method timesBase () self)
    (method compare:withLt:withEq:withGt: (aNatural ltBlock eqBlock gtBlock) 
        ((aNatural isZero) ifTrue:ifFalse:
            {eqBlock}
            {ltBlock})
    )

    (method plus:carry: (aNatural c) 
        ((aNatural isZero) ifTrue:ifFalse:
            {(Natural fromSmall: c)}
            {(aNatural + (Natural fromSmall: c))})
    )

    (method minus:borrow: (aNatural c) 
        ((aNatural isZero) ifTrue:ifFalse:
            {((c = 0) ifTrue:ifFalse:
                {self}
                {(self error: 'subtracting-nonzero-carry-from-zero)})
            }
            {(self error: 'subtracting-nonzero-nat-from-zero)}
        )
    )
)


;; NatNonZero
;;Abstraction (NonZero) = n*b + d
;;Invariant (NatNonZero) = b > 10, 
;;                         (n != NatZero or d != 0) , 0 < d < b, 
(class NatNonZero [subclass-of Natural] 
    [ivars n d]

    (class-method create:nonzero: (anInteger aNatural)
        ((self new) in:it: anInteger aNatural))

    (method in:it: (anInteger aNatural) ;;Private
        (set d anInteger)
        (set n aNatural)
        self
    )

     (method * (aNatural) 
        [locals argd argn zlo zhi snd trd]
        (set argd (aNatural modBase))
        (set argn (aNatural divBase))
        (set zlo ((d * argd) mod: (Natural base)))
        (set zhi ((d * argd) div: (Natural base)))
        (set snd (((Natural fromSmall: zhi) + 
                  ((argn * (Natural fromSmall: d)) + 
                  (n * (Natural fromSmall: argd)))) timesBase))
        (set trd (((argn * n) timesBase) timesBase))
        (((Natural fromSmall: zlo) + snd) + trd)
    )
   

    (method sdivmod:with: (divisor aBlock) [locals carry q' r' q r]
        
            ((divisor <= 0) ifTrue:ifFalse: 
                {(self error: 'divisor-smaller-or-equal-to-0)}
                {   
                    (set q (n sdivmod:with: divisor [block (quo rem) quo]))   
                    (set r (n sdivmod:with: divisor [block (quo rem) rem]))
                    (set carry ((r * (Natural base)) + d))
                    (set q' (Natural first:rest: (carry div: divisor) q))
                    (set r' (carry mod: divisor))
                    (aBlock value:value: q' r')
                }))


    
    (method isZero  () false)
    
    (method invariant () 
            (((
        (n isZero) not) 
        or: {(d > 0)}) 
        and: {(d < (Natural base))}))

    (method printrep () 
        (n printrep)
        (', print)
        (d print))

    (method modBase () d)
    (method divBase () n)
    (method timesBase () (NatNonZero create:nonzero: 0 self))
    

    (method compare:withLt:withEq:withGt: (aNatural ltBlock eqBlock gtBlock) 
        [locals argd argn resBlock]
        (set argd (aNatural modBase))
        (set argn (aNatural divBase))
        (n compare:withLt:withEq:withGt: argn ltBlock 
            {((argd = d) ifTrue:ifFalse:
                eqBlock
                {(
                    (argd < d) ifTrue:ifFalse:
                     gtBlock
                     ltBlock
                )}    
            )} gtBlock)
    )

    (method plus:carry: (aNatural c) [locals argd argn retd retn]
        (set argd (aNatural modBase))
        (set argn (aNatural divBase))
        (set retd (((argd + d) + c) mod: (Natural base)))
        (set retn (((argd + d) + c) div: (Natural base)))
        (Natural first:rest: retd (n plus:carry: argn retn))
    )
    
    (method minus:borrow: (aNatural c) [locals argd argn initDiff]
        (set argd (aNatural modBase))
        (set argn (aNatural divBase))
        (set initDiff ((d - argd) - c))
        ((initDiff < 0) ifTrue:ifFalse:
            {(Natural first:rest: ((Natural base) + initDiff) (n minus:borrow: argn 1))}
            {(Natural first:rest: initDiff (n minus:borrow: argn 0))}
        )
    )

   
)

(class DebugNat [subclass-of Object] [ivars nat] ; a natural number
    (class-method of: (aNat) ((self new) init: aNat))
    (method init: (n) (set nat n) self) ; private
    (method print () (nat printrep))
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;;  Solution to Exercise 38
(class LargeInteger
  [subclass-of Integer]
  [ivars magnitude]

  (class-method withMagnitude: (aNatural)
      ((self new) magnitude: aNatural))
  (method magnitude: (aNatural) ; private, for initialization
      (set magnitude aNatural)
      self)

  (method magnitude () magnitude)

  (class-method fromSmall: (anInteger)
     ((anInteger isNegative) ifTrue:ifFalse: 
        {(((self fromSmall: 1) + (self fromSmall: ((anInteger + 1) negated)))
          negated)}
        {((LargePositiveInteger new) magnitude: 
                 (Natural fromSmall: anInteger))}))
  (method asLargeInteger () self)
  (method isZero () (magnitude isZero))
  (method = (anInteger) ((self - anInteger)     isZero))
  (method < (anInteger) ((self - anInteger) isNegative))
  (method > (anInteger) ((anInteger - self) isNegative))

  (method + (aLargeInt) (self subclassResponsibility))
  (method * (aLargeInt) (self subclassResponsibility))

  (method div: (n) (self sdiv: n))
  (method mod: (n) (self smod: n))

  (method sdiv: (aSmallInteger) (self subclassResponsibility))
  (method smod: (aSmallInteger) 
        (self - ((LargeInteger fromSmall: aSmallInteger) * (self sdiv: aSmallInteger)))
  )

  (method print () (self subclassResponsibility))

  (method negated () 
    ((self isNegative) ifTrue:ifFalse:
        {(LargePositiveInteger withMagnitude: magnitude)}
        {((self isZero) ifTrue:ifFalse:
            {(LargePositiveInteger withMagnitude: magnitude)}
            {(LargeNegativeInteger withMagnitude: magnitude)}
        )}
    )
  )


)

;;A (LargePositiveInteger) == + LargeInteger 
;;I (LargePositiveInteger i) == i >= 0
(class LargePositiveInteger [subclass-of LargeInteger]
    (method isNegative () false)
    (method isStrictlyPositive () (((self magnitude) isZero) 
        ifTrue:ifFalse:
            {false}
            {true}
        )
    )
    (method isNonnegative () true)
    (method print () ((self magnitude) print))

    (method * (aLargeInt) (aLargeInt multiplyByLargePositiveInteger: self))

    (method multiplyByLargePositiveInteger: (aLargeInt) 
        (LargePositiveInteger withMagnitude: 
            ((self magnitude) * (aLargeInt magnitude))) 
    )

    (method multiplyByLargeNegativeInteger: (aLargeInt)
        ((self isZero) ifTrue:ifFalse:
            {(LargePositiveInteger fromSmall: 0)}
            {(LargeNegativeInteger withMagnitude: 
                ((self magnitude) * (aLargeInt magnitude)))})
    )

       (method + (aLargeInt) 
        (aLargeInt addLargePositiveIntegerTo: self))


    (method addLargePositiveIntegerTo: (aLargeInt) 
        (LargePositiveInteger withMagnitude: ((self magnitude) + (aLargeInt magnitude)))
    )

    (method addLargeNegativeIntegerTo: (aLargeInt) [locals smag imag]
            (set smag (self magnitude))
            (set imag (aLargeInt magnitude))
            ((smag < imag) ifTrue:ifFalse:
                {(LargeNegativeInteger withMagnitude: (imag - smag))}
                {(LargePositiveInteger withMagnitude: (smag - imag))}
            )
    )

    (method sdiv: (anInteger)
        ((anInteger isStrictlyPositive) ifTrue:ifFalse:
            {(LargePositiveInteger withMagnitude: (magnitude sdiv: anInteger))}
            {((((self - (LargeInteger fromSmall: anInteger)) - (LargeInteger fromSmall: 1))
                sdiv: (anInteger negated))
            negated)}))
   
)

;;A (LargeNegativeInteger) == - LargeInteger 
;;I (LargeNegativeInteger i) == i < 0
(class LargeNegativeInteger [subclass-of LargeInteger]
    (method isNegative () true)
    (method isNonnegative () false)
    (method isStrictlyPositive () false)

    (method * (aLargeInt) (aLargeInt multiplyByLargeNegativeInteger: self))

    (method print () ('- print) ((self magnitude) print))

    (method multiplyByLargeNegativeInteger: (aLargeInt) 
        (LargePositiveInteger withMagnitude: 
            ((self magnitude) * (aLargeInt magnitude))) 
    )

    (method multiplyByLargePositiveInteger: (aLargeInt) 
        ((aLargeInt isZero) ifTrue:ifFalse:
            {(LargePositiveInteger fromSmall: 0)}
            {(LargeNegativeInteger withMagnitude: 
                ((self magnitude) * (aLargeInt magnitude)))})
    )
    
     
    (method + (aLargeInt) 
        (aLargeInt addLargeNegativeIntegerTo: self))

    (method addLargeNegativeIntegerTo: (aLargeInt) 
        (LargeNegativeInteger withMagnitude: ((self magnitude) + (aLargeInt magnitude)))
    )

    (method addLargePositiveIntegerTo: (aLargeInt) [locals smag imag]
            (set smag (self magnitude))
            (set imag (aLargeInt magnitude))
            ((imag < smag) ifTrue:ifFalse:
                {(LargeNegativeInteger withMagnitude: (smag - imag))}
                {(LargePositiveInteger withMagnitude: (imag - smag))}
            )
    )

    (method sdiv: (anInteger)
        ((LargePositiveInteger withMagnitude: (self magnitude)) sdiv: (anInteger negated))
    )

           
)
