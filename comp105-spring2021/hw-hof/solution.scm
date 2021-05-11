;;;;;;;;;;;;;;;;;;; COMP 105 HOF ASSIGNMENT ;;;;;;;;;;;;;;;;
;;;; Eli Dow

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;; Exercise 28


;; (b): max*

;; (max*) Given a non-empty list of integers xs, max* returns the maximum value
;;        of the list

;; (lambda) Given two integers m and n, lambda returns the largest of the two; 
;;          returns n if m = n

;; laws (not mandatory)
;;    (max* (cons x xs)) == (foldl (lambda (m n) (if (> m n) m n)) x xs)

(define max* (xs)
    (foldl (lambda (m n) (if (> m n) m n)) (car xs) (cdr xs)))


        ;; unit tests
        (check-expect (max* '(105)) 105)              ;; one elem
        (check-expect (max* '(1 2 3 4 5)) 5)          ;; many elems: all > 0
        (check-expect (max* '(2020 1874 2020)) 2020)  ;; many elems: tie
        (check-expect (max* '(-1 2 -3 4 -5)) 4)       ;; many elems: pos and neg


;; (e): sum

;; (sum) Given a non-empty list of integers xs, sum returns the sum of all
;;       the elements in xs

;; laws (not mandatory)
;;    (sum (cons x xs)) == (foldl + x xs)

(define sum (xs)
    (foldl + (car xs) (cdr xs)))


        ;; unit tests
        (check-expect (sum '(99)) 99)               ;; one elem
        (check-expect (sum '(1 2 3 4)) 10)          ;; many elems: all > 0
        (check-expect (sum '(20 2 3 80)) 105)
        (check-expect (sum '(78 -89 1 2 3 4)) -1)   ;; many elems: pos and neg


;; (f): product

;; (product) Given a non-empty list of integers xs, product returns the product
;;           of all the elements in xs

;; laws (not mandatory)
;;    (product (cons x xs)) == (foldl * x xs)

(define product (xs)
    (foldl * (car xs) (cdr xs)))


        ;; unit tests
        (check-expect (product '(99)) 99)             ;; one elem
        (check-expect (product '(1 2 3 4)) 24)        ;; many elems: all > 0
        (check-expect (product '(-5 4 2 -6 -1)) -240) ;; many elems: pos or neg
        (check-expect (product '(20 2 3 0)) 0)        ;; many elems: contains 0


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;; Exercise 29


;; (a): append

;; (append) Given two lists xs and ys, append returns a list with the elements
;;          of xs followed by the elements of ys

;; (lambda) Given an element m and a list n, lambda returns the list (cons m n)

;; laws (not mandatory)
;;    (append (cons x xs) (cons y ys)) == 
;;            (foldr (lambda (m n) (cons m n)) (cons y ys) (cons x xs))

(define append (xs ys) 
    (foldr (lambda (m n) (cons m n)) ys xs))


        ;; unit tests
        (check-expect (append '() '()) '())               ;; both empty
        (check-expect (append '(1) '()) '(1))             ;; one empty
        (check-expect 
            (append '(1 2 3) '(4 5 6)) '(1 2 3 4 5 6))    ;; both non-empty
        (check-expect (append '(COMP is very) '(fun)) 
            '(COMP is very fun))


;; (c): reverse

;; (reverse) Given a list of S-expressions xs, reverse returns the list reversed

;; (lambda) Given an element m and a list n, lambda returns the list (cons m n)

;; laws (not mandatory)
;;    (reverse (cons x xs)) == 
;;            (foldl (lambda (m n) (cons m n)) '() (cons x xs))

(define reverse (xs) 
    (foldl (lambda (m n) (cons m n)) '() xs))


        ;; unit tests
        (check-expect (reverse '()) '())                 ;; empty list
        (check-expect (reverse '(105) ) '(105))          ;; one elem
        (check-expect 
            (reverse '(1 2 3)) '(3 2 1))                 ;; many elems: atoms
        (check-expect 
            (reverse '((#t #f) (#f #t))) 
            '((#f #t) (#t #f)))                          ;; many elems: pairs


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;; Exercise 30

;; map

;; (map) Given a function fun and a list of S-expressions xs, map applies fun to
;;       each element in the list and returns the list of results

;; (lambda) Given an element m and a list n, lambda returns the list
;;          (cons (fun m) n)

;; laws (not mandatory)
;;    (map fun (cons x xs)) == 
;;            (foldr (lambda (m n) (cons (fun m) n)) '() (cons x xs))

(define map (fun xs) 
    (foldr (lambda (m n) (cons (fun m) n)) '() xs))


        ;; unit tests
        (check-expect (map (lambda (x) (+ x 1)) '()) '())   ;; list empty
        (check-expect 
            (map (lambda (x) (* x 10)) '(105)) '(1050))     ;; one elem
        (check-expect 
            (map (lambda (x) (not x)) '(#t #f #f #t)) 
            '(#f #t #t #f))                                 ;; many elems
        (check-expect 
            (map reverse '((1 2) (3 4 5) (6)) ) 
            '((2 1) (5 4 3) (6)))                          


;; filter

;; (filter) Given a predicate p? and list of S-expressions xs, filter returns
;;          a list of the elements in xs that satisfy p?

;; (lambda) Given an element m and a list n, lambda returns the list (cons m n) 
;;          if (p? m) == #t; returns n otherwise

;; laws (not mandatory)
;;    (filter p? (cons x xs)) == 
;;        (foldr (lambda (m n) (if (p? m) (cons m n) n)) '() (cons x xs))

(define filter (p? xs) 
    (foldr (lambda (m n) (if (p? m) (cons m n) n)) '() xs))


        ;; unit tests
        (check-expect (filter number? '()) '())      ;; null case
        (check-expect 
            (filter number? '(2 4 6 8 10)) 
            '(2 4 6 8 10))                           ;; many elems: all satisfy
        (check-expect 
            (filter boolean? '(#t true 1 (#t))) 
            '(#t))                                   ;; many elems: some satisfy
        (check-expect (filter atom? '((no))) '())    ;; many elems: none satisfy


;; exists?

;; (exists?) Given a predicate p? and a list of S-expressions xs, exists? 
;;           returns #t if any element in xs satisfies p?; returns #f otherwise

;; (lambda) Given an element m and a list n, lambda returns the list
;;          (cons m n) if (p? m) == #t; returns n otherwise

;; laws (not mandatory)
;;    (exists? p? (cons x xs)) == (not (null? 
;;        (foldr (lambda (m n) (if (p? m) (cons m n) n)) '() (cons x xs))))


(define exists? (p? xs) 
    (not (null? (foldr (lambda (m n) (if (p? m) (cons m n) n)) '() xs))))


        ;; unit tests
        (check-assert (exists? number? '(1 (2) 3 4)))    ;; exists
        (check-assert (exists? pair? '(1 2 (3 4) 5)))
        (check-assert (exists? null? '(hello () there)))
        (check-assert (not (exists? symbol? '())))       ;; doesn't exist
        (check-assert (not (exists? pair? '(1 2 3)))) 


;; all?

;; (all?) Given a predicate p? and a list of S-expressions xs, all? returns
;;           #t if every element in xs satisfies p?; returns #f otherwise

;; (lambda) Given an element m and a list n, lambda returns the list
;;          (cons m n) if (p? m) == #t; returns n otherwise

;; laws (not mandatory)
;;    (exists? p? (cons x xs)) == (equals? xs 
;;        (foldr (lambda (m n) (if (p? m) (cons m n) n)) '() (cons x xs)))


(define all? (p? xs) 
    (equal? xs (foldr (lambda (m n) (if (p? m) (cons m n) n)) '() xs)))


        ;; unit tests
        (check-assert (all? number? '(1 2 3 4)))            ;; all satisfy
        (check-assert (all? pair? '((1 2) (3 4))))
        (check-assert (all? symbol? '())) 
        (check-assert (not (all? null? '(hello () there)))) ;; not all satisfy
        (check-assert (not (all? pair? '((1 2) 3 (4 5))))) 


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;; Exercise 38

;; emptyset and member? defined because it's given
(val emptyset (lambda (x) #f))
(define member? (x s) (s x))

;; evens

;; (evens) Given an integer x, evens returns #t if x belongs to the set of all
;;         even numbers; returns #f otherwise

;; laws
;;    (evens x) == (member? x (= (mod x 2) 0))

(val evens (lambda (x) (= (mod x 2) 0)))


        ;; unit tests
        (check-assert (evens 48))   ;; in set
        (check-assert (evens -2))
        (check-assert (not (evens 19)))  ;; not in set
        (check-assert (not (evens -1)))


;; two-digits

;; (two-digits) Given an integer x, two-digits returns #t if x belongs to the
;;              set of all positive two-digit numbers; returns #f otherwise

;; laws
;;    (two-digits x) == (member? x (and (>= x 10) (<= x 99)))

(val two-digits (lambda (x) (and (>= x 10) (<= x 99))))


        ;; unit tests
        (check-assert (two-digits 10))         ;; in set
        (check-assert (two-digits 44))
        (check-assert (not (two-digits -18)))  ;; not in set
        (check-assert (not (two-digits 100)))


;; add-element

;; (add-element) Given an element x and a set xs, add-element returns a function
;;               that takes an element y and returns #t if y belongs to set s or
;;               is equal to x; returns #f otherwise

;; laws
;;    (member? y (add-element y s)) == #t
;;    (member? y (add-element x s)) == (member? y (or (s y) (= x y))) 
;;        where (not (equal? y x))

(define add-element (x s) 
    (lambda (y) (or (s y) (= x y))))


        ;; unit tests
        (check-assert ((add-element 1 evens) 1))          ;; elem we just added
        (check-assert ((add-element 100 two-digits) 100))
        (check-assert ((add-element 45 evens) 40))        ;; x !=y, still in set
        (check-assert ((add-element 100 two-digits) 99))
        (check-assert (not ((add-element 1 evens) 43)))   ;; x != y, not in set
        (check-assert (not ((add-element 100 two-digits) 9)))


;; union

;; (union) Given two sets s1 and s2, union returns a function that takes an 
;;         element y and returns #t if y belongs to the union of the sets; 
;;         returns false otherwise

;; laws
;;    (member? y (union s1 s2)) == (member? y (or (s1 y) (s2 y)))

(define union (s1 s2) 
    (lambda (y) (or (s1 y) (s2 y))))


        ;; unit tests
        (check-assert ((union evens two-digits) 256))  ;; belongs in s1 only
        (check-assert ((union evens two-digits) 67))   ;; belongs in s2 only
        (check-assert ((union evens two-digits) 48))   ;; belongs in both
        (check-assert 
            (not ((union evens two-digits) 3)))        ;; not in either


;; inter

;; (inter) Given two sets s1 and s2, inter returns a function that takes an 
;;         element y and returns #t if y belongs to the intersection of the
;;         sets; returns false otherwise

;; laws
;;    (member? y (inter s1 s2)) == (member? y (and (s1 y) (s2 y)))

(define inter (s1 s2) 
    (lambda (y) (and (s1 y) (s2 y))))

        ;; unit tests
        (check-assert ((inter evens two-digits) 72))       ;; belongs in both
        (check-assert (not ((inter evens two-digits) 2)))  ;; belongs in s1 only
        (check-assert (not ((inter evens two-digits) 67))) ;; belongs in s2 only
        (check-assert (not ((inter evens two-digits) 3)))  ;; not in either


;; diff

;; (diff) Given two sets s1 and s2, diff returns a function that takes an 
;;        element y and returns #t if y belongs to s1 but doesn't belong to s2;
;;        returns #f otherwise

;; laws
;;    (member? y (diff s1 s2)) == (member? y (and (s1 y) (not (s2 y)))

(define diff (s1 s2) 
    (lambda (y) (and (s1 y) (not (s2 y)))))

        ;; unit tests
        (check-assert ((diff evens two-digits) 2))        ;; in s1, not in s2
        (check-assert ((diff evens two-digits) 144))
        (check-assert ((diff two-digits evens) 11))
        (check-assert (not ((diff evens two-digits) 49))) ;; not in s1, in s2
        (check-assert (not ((diff two-digits evens) 1)))  ;; not in s1 or s2
        (check-assert (not ((diff two-digits evens) 46))) ;; in s1 and s2


;; definition of set-ops record
(record set-ops [emptyset member? add-element union inter diff])

;; (set-ops-from) Given an equality predicate eq?, set-ops-from returns a
;;                record of all defined set functions, created by make-set-ops

(define set-ops-from (eq?)
       (let ([emptyset    (lambda (x) #f)]
             [member?     (lambda (x s) (s x))]
             [add-element (lambda (x s y) (or (s y) (eq? x y)))]
             [union       (lambda (s1 s2 y) (or (s1 y) (s2 y)))]
             [inter       (lambda (s1 s2 y) (and (s1 y) (s2 y)))]
             [diff        (lambda (s1 s2 y) (and (s1 y) (not (s2 y))))])
         (make-set-ops emptyset member? add-element union inter diff)))


        ;; unit tests
        (check-assert (function? set-ops-from))           ;; for function itself
        (check-assert (set-ops? (set-ops-from =)))
        
        (val atom-set-ops (set-ops-from =))               ;; defs for unit tests
        (val atom-emptyset (set-ops-emptyset atom-set-ops))
        (val atom-member? (set-ops-member? atom-set-ops))
        (val atom-add-element (set-ops-add-element atom-set-ops))
        (val atom-union (set-ops-union atom-set-ops))
        (val atom-inter (set-ops-inter atom-set-ops))
        (val atom-diff (set-ops-diff atom-set-ops))

        (check-assert (not (emptyset 1)))                        ;; emptyset
        (check-assert (not (member? 33 evens)))                  ;; member
        (check-assert (atom-add-element 2 evens 2))              ;; add-element
        (check-assert (not (atom-add-element 2 evens 1)))
        (check-assert (atom-union evens two-digits 33))          ;; union
        (check-assert (not (atom-union evens two-digits 101)))
        (check-assert (atom-inter evens two-digits 66))          ;; inter
        (check-assert (not (atom-inter evens two-digits 102)))
        (check-assert (atom-diff evens two-digits 2))            ;; diff
        (check-assert (not (atom-diff two-digits evens 66)))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;; Exercise F 

;; (flip) Given a two-argument function f, flip returns a function
;;        that is like the argument function except that it expects its 
;;        arguments in opposite order

;; laws
;;    ((flip fun) x y) == (fun y x)


(define flip (fun) 
    (lambda (x y) (fun y x)))


        ;; unit tests
        (check-assert ((flip >) 7 8))                   ;; returns #t
        (check-expect ((flip append) '(6 1 7) '(9 8 1))
            '(9 8 1 6 1 7))
        (check-assert (not ((flip <) 7 8)))             ;; returns #f
        (check-assert (not ((flip >) 42 21)))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;; Exercise O

;; (ordered-by?) Given a comparision function precedes? that represents a
;;               transitive relation, ordered-by? returns #t if a list of values
;;               xs is ordered by that relation; returns #f otherwise

;; (lambda)  For ordered: Given an element x and a list of values xs, lambda1
;;           lambda1 returns #t if (cons x xs) is ordered by the relation 
;;           precedes?; returns #f otherwise

;; (lambda)  For start: Given a list of values xs, lambda2 returns #t if
;;           xs is ordered by the relation precedes?; returns #f otherwise

;; laws
;;    ((ordered-by? precedes?) '()) == #t 
;;    ((ordered-by? precedes?) x) == #t where (atom? x) == #t 
;;    ((ordered-by? precedes?) cons x (cons y zs)) == 
;;            ((ordered-by? precedes?) cons y (cons z zs)) 
;;            when (precedes? x (car xs)) == #t
;;    ((ordered-by? precedes?) cons x (cons y zs)) == 
;;            #f when (precedes? x (car xs)) == #f


(define ordered-by? (precedes?) 
    (letrec (
             [ordered  (lambda (x xs) 
                        (if (null? xs)              ;; if (cons x xs) is an atom
                            #t 
                            (if (precedes? x (car xs))
                                (ordered (car xs) (cdr xs))
                                #f)))] 
             [start  (lambda (xs)
                        (if (null? xs)              ;; if xs is null
                            #t
                           (ordered (car xs) (cdr xs))))]) 
     start))


        ;; unit tests
        (check-assert (function? ordered-by?))           ;; for function itself
        (check-assert (function? (ordered-by? <)))
        (check-error (ordered-by? < '(1 2 3)))

        (check-assert ((ordered-by? <) '(1 2)))               ;; returns #t
        (check-assert ((ordered-by? =) '(3 3 3 3)))
        (check-assert ((ordered-by? >=) '(10 8 6 4 2 0 -2)))
        (check-assert (not ((ordered-by? <) '(3 2 1))))       ;; returns #f
        (check-assert (not ((ordered-by? <=) '(3 3 4 5 6 3))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;; Exercise V


(val the-fault list1) ;; build a singleton fault set
(val no-faults '())   ;; an empty fault set

;; faults/none

;; (faults/none) faults/none returns a validator that takes a response r and
;;               always returns the empty list of faults no matter the response

;; laws
;;    ((faults/none) r) == (no-faults)

(define faults/none () 
    (lambda (r) no-faults))


        ;; unit tests
        (check-expect ((faults/none) 'COMP105) '())
        (check-expect ((faults/none) 'COMP10) '())


;; faults/always

;; (faults/always) Given a fault (symbol) F, faults/always returns a validator
;;                 that takes a response r and always returns a singleton list
;;                 containing F, no matter the response

;; laws
;;    ((faults/always F) r) == (the-fault F)

(define faults/always (F)
    (lambda (r) (the-fault F)))


        ;; unit tests
        (check-expect ((faults/always 'hey) 'COMP105) '(hey))
        (check-expect ((faults/always 'there) 'COMP105) '(there))


;; faults/equal

;; (faults/equal) Given a key (symbol) k and value v, faults/equal returns a
;;                 validator that takes a response r. That validator returns a 
;;                 a singleton list containing k if k is bound to v in the 
;;                 response; returns n empty list of faults otherwise

;; laws
;;    ((faults/equal k v) r) == (the-faults (bind k v '())) when 
;;        (= (find k r) v)
;;    ((faults/equal k v) r) == (no-faults) when (!= (find k r) v)

(define faults/equal (k v) 
    (lambda (r) (if (= (find k r) v) (the-fault k) no-faults)))


        ;; unit tests
        (check-expect ((faults/equal 'hey 'there) '((comp 105))) '())
        (check-expect ((faults/equal 'hey 'there) '((hey there))) '(hey))


;; faults/both

;; (faults/both) Given two validators V1 and V2, faults/both returns a new
;;               validator that combines checks from V1 and V2. This validator
;;               returns a single list of faults formed by the union of faults 
;;               from V1 and V2

;; laws
;;    (faults/both V1 V2) == (union (V1 r) (V2 r))

(define faults/both (V1 V2)
    (let* ([member?  (lambda (x s) (exists? ((curry =) x) s))]
                  [add-elem (lambda (x s) (if (member? x s) s (cons x s)))]
                  [union    (lambda (faults1 faults2)
                             (foldr add-elem faults2 faults1))])
     (lambda (r) (union (V1 r) (V2 r)))))


        ;; unit tests
        (check-expect 
            ((faults/both (faults/always 'hey) 
              (faults/always 'there)) '(comp 105)) '(hey there))
        (check-expect 
            ((faults/both (faults/always 'hey) (faults/always 'there))
             '(hey there)) '(hey there))


;; faults/switch 

;; (faults/switch) Given a key (symbol) k and a table t, faults/switch
;;                 returns a validator that validates the response r using a 
;;                 validator associated with k and selected from t

;; laws
;;    ((faults/switch k t) response) == (error) when (= '() (find (find k r) t))
;;    ((faults/switch k t) response) == 
;;           ((find (find k r) t) r) when (!= '() (find (find k r) t))

(define faults/switch (k t) 
    (lambda (r) 
        (if (= '() (find (find k r) t)) (error) ((find (find k r) t) r))))


        ;; unit tests
        (check-error ((faults/switch 'hey (bind 'hello (faults/none))) 
                       '(hey there)))


;; regrade validator written in scheme
(val regrade-validator  ;; example for the regrade form
  (faults/switch 'why
    (bind         'photo
                  (faults/none)
      (bind       'badsubmit
                   (faults/both (faults/equal 'badsubmit_asst '...)
                                (faults/equal 'info #f))
        (bind     'badgrade
                  (faults/both
                      (faults/equal 'badgrade_asst '...)
                      (faults/both
                         (faults/equal 'info #f)
                         (faults/equal 'problem #f)))
          (bind   'recitation
                  (faults/both
                      (faults/equal 'date #f)
                      (faults/equal 'leaders #f))
            (bind '#f
                  (faults/always 'nobutton)
                  '())))))))


        ;; unit tests
        (check-expect (regrade-validator '([why photo]))
                    '())

        (check-expect (regrade-validator '([why badsubmit]
                                            [info wrong-pdf]
                                            [badsubmit_asst opsem]))
                    '())

        (check-expect (regrade-validator '([why badsubmit]
                                            [badsubmit_asst opsem]
                                            [info #f]))
                    '(info))

        (check-expect (regrade-validator '([why badsubmit]
                                            [info wrong-pdf]
                                            [badsubmit_asst ...]))
                    '(badsubmit_asst))

        (check-expect (regrade-validator '([why badsubmit]
                                            [info #f]
                                            [problem #f]
                                            [badsubmit_asst ...]))
                    '(badsubmit_asst info))

        (check-expect (regrade-validator '([why badgrade]
                                            [info #f]
                                            [problem #f]
                                            [badgrade_asst impcore]))
                    '(info problem))