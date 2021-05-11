;;;;;;;;;;;;;;;; COMP 105 Continuations ASSIGNMENT ;;;;;;;;;;;;
;;;; Eli Dow


;; Boolean Formulas
(record not [arg])
(record and [args])
(record or [args])


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;; Exercise L


;; (list-of?) Given a predicate that can be applied to any value A? and an 
;;            arbitrary μScheme value v, list-of? returns #t if v is a list of 
;;            values such that each element of the list satisfies A?; returns 
;;            #f otherwise

;; laws
;;    (list-of? A? v) == #t when (null? v) == #t
;;    (list-of? A? v) == #f when (not (pair? v)) == #f
;;    (list-of? A? (cons x xs)) == (list-of? A? xs) when (A? x) == #t
;;    (list-of? A? (cons x xs)) == #f when (A? x) == #f                     


(define value? (_) #t) ;; tell if the argument is a value, given


(define list-of? (A? v) 
    (if (null? v)
        #t
        (if (not (pair? v)) 
            #f
            (if (A? (car v))
                    (list-of? A? (cdr v))
                    #f))))
                    

        ;; unit tests
        (check-assert (list-of? boolean? '()))            ;; v = empty list
        (check-assert (not (list-of? boolean? #t)))       ;; v = non-pair
        (check-assert (not (list-of? symbol? 4)))
        (check-assert (not (list-of? number? 
                                    (lambda (x) (+ x 1)))))
        (check-assert (list-of? boolean? '(#t #f #t #f))) ;; v = list, #t case
        (check-assert (list-of? function? 
                        (list2 (lambda (x) (* x 2)) (lambda (x) (* x 4)))))
        (check-assert (not (list-of? number? 
                            (cons 'Comp 105))))           ;; v = list, #f case
        (check-assert (not (list-of? pair? 
                            '(list2 (1 2) (lambda (x) (cons x '()))))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;; Exercise F


;; (formula?) Given an arbitrary μScheme value v, formula? returns #t if the 
;;            value represents a boolean formula; returns #f otherwise

;; laws
;;    (formula? v) == #t where (symbol? v) == #t
;;    (formula? (make-not f)) == #t 
;;    (formula? (make-and fs)) == #t
;;    (formula? (make-or fs)) == #t
;;    (formula? v) == #f where v has none of the forms of the previous laws


(define formula? (v) 
    (if (symbol? v)
        #t 
        (if (not? v)
            #t 
            (if (or (and? v) (or? v))  ;; combined two laws as spec suggested
                #t 
                #f))))


        ;; unit tests
        (check-assert (formula? 'Jumbo))                  ;; symbol
        (check-assert (formula? (make-not #t)))           ;; make-not
        (check-assert (formula? (make-not #f)))
        (check-assert (formula? (make-and '(#t #t #t))))  ;; make-and
        (check-assert (formula? (make-and '(#t #f))))
        (check-assert (formula? (make-or '(#t #t #f))))   ;; make-or
        (check-assert (formula? (make-or '(#f #f))))
        (check-assert (not (formula? 4)))                 ;; not a bool formula
        (check-assert (not (formula? '(1 2 3 4))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;; Exercise E


;; (eval-formula) Given a boolean formula f and an environment env where the env
;;                is an association list of symbol keys and boolean values, 
;;                eval-formula returns #t given that f is satisfied in env;
;;                returns #f otherwise

;; laws
;;    (eval-formula f env) == (find f env) when (symbol? f) == #t
;;    (eval-formula (make-not v) env) == 
;;            (not (eval-formula v env)) when (not? (make-not v)) == #t
;;    (eval-formula (make-and fs) env) == 
;;            (all? (lambda (x) (eval-formula x env)) fs) 
;;            when (and? (make-and fs)) == #t
;;    (eval-formula (make-or fs) env) == 
;;            (exists? (lambda (x) (eval-formula x env) fs)
;;            when (or? (make-or fs)) == #t


(define eval-formula (f env)
    (if (symbol? f)
        (find f env)
            (if (not? f)
                (not (eval-formula (not-arg f) env))
                (if (and? f)
                    (all? (lambda (x) (eval-formula x env)) (and-args f))
                    (exists? (lambda (x) (eval-formula x env)) (or-args f))))))


        ;; unit tests
        (check-assert (eval-formula 'a '((a #t) (b #f) (c #t)))) ;; symbol
        (check-assert (not (eval-formula 'a '((a #f) (b #f)))))    
        (check-assert 
            (eval-formula (make-not 'c) '((c #f) (d #t))))       ;; make not
        (check-assert (not 
                       (eval-formula (make-not 'c) '((c #t) (d #f)))))   
        (check-assert 
            (eval-formula (make-and '(a b)) 
                '((a #t) (b #t))))                               ;; make-and
        (check-assert (not 
                       (eval-formula (make-and '(a b c)) 
                        '((a #f) (b #f) (c #f)))))
        (check-assert 
                    (eval-formula (make-or '(a b)) 
                     '((a #t) (b #f))))                          ;; make-or
        (check-assert (not 
                       (eval-formula (make-or '(a b c)) 
                        '((a #f) (b #f) (c #f)))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;; Exercise T


;; these tests are located in the file solver.tests.scm


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;; Exercise S


;; Note: In these contracts, an assignment is an association list of 
;;       variable-boolean pairs that represents an environment

;; (find-formula-true-asst) Given a boolean formula f, a failure continuation,
;;                fail, and a success continuation succ, find-formula-true-asst
;;                searches for an assignment that satisfies f. If it finds a 
;;                satisfying assignment, it calls succ, passing both the 
;;                assignment and a resume continuation; otherwise, it calls fail

;; all algebraic laws are grouped with letrec funciton

(define find-formula-true-asst (f fail succ)
    (letrec (

;; (find-formula-asst) The funciton find-formula-asst is given the following
;;                arguments: a single formula f, a boolean value bool, an
;;                assignment cur, a failure continuation fail, and a success
;;                continuation succeed. Calling find-formula-asst extends
;;                assignment cur to find an assignment that makes the single 
;;                formula equal to bool

;; laws
;;     (find-formula-asst x bool cur fail succeed) == 
;;         (find-formula-symbol x bool cur fail succeed) 
;;         where x is a symbol
;;     (find-formula-asst (make-not f) bool cur fail succeed) == 
;;         (find-formula-asst f (not bool) cur fail succeed)
;;     (find-formula-asst (make-and fs) #t cur fail succeed) == 
;;         (find-all-asst fs #t cur fail succeed)
;;     (find-formula-asst (make-and fs) #f cur fail succeed) == 
;;         (find-any-asst fs #f cur fail succeed)
;;     (find-formula-asst (make-or fs) #t cur fail succeed) == 
;;         (find-any-asst fs #t cur fail succeed)
;;     (find-formula-asst (make-or  fs) #f cur fail succeed) == 
;;         (find-all-asst fs #f cur fail succeed)

             [find-formula-asst 
              (lambda (f bool cur fail succeed)
                (if (symbol? f)
                    (find-formula-symbol f bool cur fail succeed)
                    (if (not? f)
                        (find-formula-asst (not-arg f) 
                            (not bool) cur fail succeed)
                        (if (and? f)
                            (if bool
                                (find-all-asst (and-args f) 
                                    bool cur fail succeed)
                                (find-any-asst (and-args f) 
                                    bool cur fail succeed))
                            (if bool
                                (find-any-asst (or-args f) 
                                    bool cur fail succeed)
                                (find-all-asst (or-args f) 
                                    bool cur fail succeed))))))]

;; (find-all-asst) The funciton find-all-asst is given the following
;;                arguments: a list of formula fs, a boolean value bool, an
;;                assignment cur, a failure continuation fail, and a success
;;                continuation succeed. Calling find-all-asst extends cur to
;;                find an assignment that makes every formula in the list 
;;                formulas equal to bool

;; laws
;;     (find-all-asst '() bool cur fail succeed) == (succeed cur fail)
;;     (find-all-asst (cons f fs) bool cur fail succeed) ==  
;;         (find-formula-asst f bool cur fail (lambda (xs r) 
;;         (find-all-asst fs bool xs r succeed)))

              [find-all-asst 
               (lambda (fs bool cur fail succeed) 
                (if (null? fs)
                    (succeed cur fail)
                    (find-formula-asst (car fs) bool cur fail 
                        (lambda (xs r) 
                            (find-all-asst (cdr fs) bool xs r succeed)))))]

;; (find-any-asst) The funciton find-any-asst is given the following
;;                arguments: a list of formula fs, a boolean value bool, an
;;                assignment cur, a failure continuation fail, and a success
;;                continuation succeed. Calling find-any-asst extends cur to 
;;                find an assignment that makes any one of the formulas equal 
;;                to bool.
    
;; laws
;;     (find-any-asst '() bool cur fail succeed) == fail
;;     (find-any-asst (cons f fs) bool cur fail succeed) == 
;;         (find-formula-asst f bool cur (lambda () 
;;         (find-any-asst fs bool cur fail succeed)) succeed)        

             [find-any-asst 
              (lambda (fs bool cur fail succeed)
                (if (null? fs)
                    (fail)
                    (find-formula-asst (car fs) bool cur 
                        (lambda () (find-any-asst (cdr fs) 
                                    bool cur fail succeed)) succeed)))]

;; (find-formula-symbol) The funciton find-formula-symbol is given the following
;;                arguments: symbol x, a boolean value bool, an
;;                assignment cur, a failure continuation fail, and a success
;;                continuation succeed. Calling find-formula-symbol does one of
;;                three things: succeeds with the current environment if x is
;;                bound to bool, fails if x is bound to (not bool), and succeeds
;;                with the current environment extended with x if x is not bound
;;                in cur

;; laws             
;;     (find-formula-symbol x bool cur fail succeed) == 
;;         (succeed (bind x bool cur) fail) 
;;         where x is not bound in cur
;;     (find-formula-symbol x bool cur fail succeed) == 
;;         (succeed cur fail) where x is bool in cur
;;     (find-formula-symbol x bool cur fail succeed) == 
;;         (fail) where x is (not bool) in cur

             [find-formula-symbol 
                (lambda (x bool cur fail succeed)
                    (if (null? (find x cur))
                        (succeed (bind x bool cur) fail)
                        (if (= (find x cur) bool)
                            (succeed cur fail)
                            (fail))))])
     (find-formula-asst f #t '() fail succ)))


        ;; given unit tests
        (check-assert (function? find-formula-true-asst)) ;; correct name
        (check-error (find-formula-true-asst))            ;; not 0 arguments
        (check-error (find-formula-true-asst 'x))         ;; not 1 argument
        (check-error (find-formula-true-asst 'x 
                      (lambda () 'fail)))                 ;; not 2 args 
        (check-error (find-formula-true-asst 'x 
                      (lambda () 'fail) 
                          (lambda (c r) 'succeed) 'z))    ;; not 4 args

        (check-error (find-formula-true-asst 'x           ;; success expects
                      (lambda () 'fail) 
                          (lambda () 'succeed)))          ;; 2 args, not 0
        (check-error (find-formula-true-asst 'x           ;; success expects
                      (lambda () 'fail) 
                          (lambda (_) 'succeed)))         ;; 2 args, not 1
        (check-error (find-formula-true-asst
                       (make-and (list2 'x (make-not 'x)))
                        (lambda (_) 'fail)                ;; failure expects
                         (lambda (_) 'succeed)))          ;; 0 args, not 1

        (check-expect   ;; x can be solved
            (find-formula-true-asst 'x
                                    (lambda () 'fail)
                                    (lambda (cur resume) 'succeed))
            'succeed)
        (check-expect   ;; x is solved by '((x #t))
            (find-formula-true-asst 'x
                                    (lambda () 'fail)
                                    (lambda (cur resume) (find 'x cur)))
            #t)
        (check-expect   ;; (make-not 'x) can be solved
            (find-formula-true-asst (make-not 'x)
                                    (lambda () 'fail)
                                    (lambda (cur resume) 'succeed))
            'succeed)

        (check-expect   ;; (make-not 'x) is solved by '((x #f))
            (find-formula-true-asst (make-not 'x)
                                    (lambda () 'fail)
                                    (lambda (cur resume) (find 'x cur)))
            #f)

        (check-expect   ;; (make-and (list2 'x (make-not 'x))) cannot be solved
            (find-formula-true-asst (make-and (list2 'x (make-not 'x)))
                                    (lambda () 'fail)
                                    (lambda (cur resume) 'succeed))
            'fail)


        ;; extra unit tests
        (check-expect   ;; (make-or (list3 'x (make-not 'x) 'x)) can be solved
            (find-formula-true-asst (make-or (list3 'x (make-not 'x) 'x))
                                    (lambda () 'fail)
                                    (lambda (cur resume) 'succeed))
            'succeed)
        (check-expect   ;; (make-or) with multiple (make-and) cannot be solved
            (find-formula-true-asst (make-or 
                                     (list2 (make-and (list2 (make-not 'x) 'x)) 
                                       (make-and (list2 'x (make-not 'x) ))))
                                        (lambda () 'fail)
                                        (lambda (cur resume) 'succeed))
            'fail)
        (check-expect   ;; (make-and) with multiple (make-not) can be solved
            (find-formula-true-asst (make-and (list2 (make-not 'x) 
                                                (make-not 'x)))
                                                 (lambda () 'fail)
                                                 (lambda (cur resume) 'succeed))
            'succeed)