;;;;;;;;;;;;;;;; Template for SAT Solver Test Cases  ;;;;;;;;;;;;
;;;; Eli Dow

(record not [arg])   ;; duplicates or records
(record or  [args])
(record and [args])


        ;; unit tests

        ;; description for f1: This test case is important because we need to 
        ;;     test that a complex formula with nested make-and formulas inside 
        ;;     a make-or formula will evaluate properly and return a correct 
        ;;     solution. In this case, at least one of the make-and formulas 
        ;;     must evaluate to true so that the entire formula does. We know
        ;;     that the first make-and formula has no solution, so it's up
        ;;     to the second make-and formula to return true. Thus, the only 
        ;;     satisfying assignment is a = #t, b = #t, c = #f

        (val f1 (make-or
                  (list2 (make-and (list2 'a (make-not 'a)))
                   (make-and (list3 'a 'b (make-not 'c))))))
        (val s1 '((a #t) (b #t) (c #f)))


        ;; description for f2: This test case is important because we need to 
        ;;     test nested not statements as well as negations for make-and 
        ;;     formulas. By boolean algebrea, we know that !(!a) == a. So an odd
        ;;     number of nested make-not formulas return !bool, whereas an even 
        ;;     number returns bool, which is demonstrated in the two formulas
        ;;     that make up the make-and formula. We are evaluating the 
        ;;     make-not of a make-and expression, so we know form DeMorgan's Law
        ;;     that !(a and b) == !a or !b. Thus, this expressio satisfies if 
        ;;     x == #t or y == #f

        (val f2 (make-not
                   (make-and 
                    (list2 (make-not (make-not (make-not 'x)))
                     (make-not (make-not (make-not (make-not 'y))))))))
        (val s2 '((x #t)))


        ;; description for f3: This test case is important because we need a
        ;;     case where make-or returns the failure continuation. It's 
        ;;     difficult to fail with make-or because it requires that all 
        ;;     arguments have no solution. Thus, we need the expression 
        ;;     (x and !x) in each make-or argument. Thus, no matter the 
        ;;     assignment, it won't be satisfied.

        (val f3 (make-or
                   (list2 (make-and (list2 'a (make-not 'a)))
                    (make-and (list2 (make-not 'b) 'b)))))
        (val s3 'no-solution)