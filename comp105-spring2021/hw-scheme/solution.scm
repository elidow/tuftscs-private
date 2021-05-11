;;;;;;;;;;;;;;;;;;; COMP 105 SCHEME ASSIGNMENT ;;;;;;;;;;;;;;;;
;;;; Eli Dow

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;; Exercise 1


;; helper function

;; (contig-sublist-prefix?) Given two lists of atoms xs and ys,
;;                          contig-sublist-prefix? returns #t if xs is a
;;                          contiguous subsequence of ys where that subsequence
;;                          is also a prefix of ys; it returns #f otherwise

;; laws
;;    (contig-sublist-prefix? '() (cons y ys)) == #t
;;    (contig-sublist-prefix? (cons x xs) '()) == #f where (cons x xs) != '()
;;    (contig-sublist-prefix? (cons x xs) (cons y ys)) == #f  where x != y
;;    (contig-sublist-prefix? (cons x xs) (cons y ys)) == 
;;         (contig-sublist-prefix? xs ys) where x == y

(define contig-sublist-prefix? (xs ys)
    (if (null? xs)
        #t
        (if (null? ys) 
            #f
            (if (!= (car xs) (car ys))
                #f 
                (contig-sublist-prefix? (cdr xs) (cdr ys))))))

        ;; unit tests
        (check-assert (contig-sublist-prefix? '() '()))  ;; both null
        (check-assert 
            (contig-sublist-prefix? '() '(2 4 6)))       ;; only xs = null
        (check-assert 
            (not (contig-sublist-prefix? '(1) '())))     ;; only ys = null
        (check-assert 
            (contig-sublist-prefix? '(1) '(1 2 3)))      ;; non-empty lists: #t
        (check-assert 
            (contig-sublist-prefix? '(1 2 3) '(1 2 3 4 5)))
        (check-assert 
            (not (contig-sublist-prefix? '(1 2 3) 
                  '(0 1 2 3 4 5))))                      ;; non-empty lists: #f
        (check-assert 
            (not (contig-sublist-prefix? '(1 3 5) 
                  '(0 1 2 3 4 5))))   


;; main function

;; (contig-sublist?) Given two lists of atoms xs and ys, contig-sublist? returns
;;                   #t if xs is a contiguous subsequence of ys
;;                   list; it returns #f otherwise

;; laws
;;    (contig-sublist? '() (cons y ys)) == #t
;;    (contig-sublist? (cons x xs) '()) == #f where (cons x xs) != '()
;;    (contig-sublist? (cons x xs) (cons y ys)) == 
;;        (|| (contig-sublist-prefix? (cons x xs) (cons y ys)) 
;;            (contig-sublist? (cons x xs) ys))

(define contig-sublist? (xs ys)
    (if (null? xs)
        #t 
        (if (null? ys) 
            #f 
            (|| (contig-sublist-prefix? xs ys)     ;; car ys included in subseq.
                (contig-sublist? xs (cdr ys))))))  ;; or car ys not included

        ;; unit tests
        (check-assert (contig-sublist? '() '()))         ;; both null
        (check-assert 
            (contig-sublist? '() '(2 4 6)))              ;; only xs = null
        (check-assert 
            (not (contig-sublist? '(1) '())))            ;; only ys = null
        (check-assert 
            (contig-sublist? '(1) '(1 2 3)))             ;; non-empty lists: #t
        (check-assert 
            (contig-sublist? '(1 2 3) '(0 1 2 3 4 5)))
        (check-assert 
            (not (contig-sublist? '(1 3 5) 
                  '(0 1 2 3 4 5))))                      ;; non-empty lists: #f
        (check-assert 
            (not (contig-sublist? '(5 6) 
                  '(0 1 2 3 4 5))))    


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;; Exercise 8


;; (mirror) Given a list of S-expressions or an S-expression xs, mirror returns
;;          a list in which every list in xs is recursively mirrored and the
;;          the resulting lists are in reverse order

;; laws
;;    (mirror '()) == '()
;;    (mirror x) == x where (atom? x) == #t and x != '()
;;    (mirror (cons x xs)) == (reverse (map mirror (cons x xs)))

(define mirror (xs)
    (if (null? xs)
        '()
        (if (atom? xs)
            xs
            (reverse (map mirror xs)))))

        ;; unit tests
        (check-expect (mirror '()) '())                 ;; null case
        (check-expect (mirror 3) 3)                     ;; atom case
        (check-expect (mirror '(2)) '(2))               ;; list: one atom elem
        (check-expect (mirror '(1 2 3 4)) '(4 3 2 1))   ;; list: many atom elems
        (check-expect (mirror '(1 (1 2) 3 4)) 
            '(4 3 (2 1) 1))                             ;; nested lists
        (check-expect (mirror '((1 (2 3)) 4 (5 6) 7)) 
            '(7 (6 5) 4 ((3 2) 1)) )


;; (flatten) Given a list of S-expressions xs, flatten returns a list having
;;           the same atoms as xs in the same order, but in a flat list without
;;           any internal brackets

;; laws
;;    (flatten '()) == '()
;;    (flatten (cons x xs)) == (cons x (flatten xs))
;;                             where (atom? x) == #t and x != '()
;;    (flatten (cons x xs)) == 
;;        (append (flatten x) (flatten xs)) where (atom? x) == #f

(define flatten (xs)
    (if (null? xs)
        '()
        (if (atom? (car xs))
            (cons (car xs) (flatten (cdr xs)))
            (append (flatten (car xs)) (flatten (cdr xs))))))


        ;; unit tests
        (check-expect (flatten '()) '())                ;; null case
        (check-expect (flatten '(1)) '(1))              ;; list: one atom elem
        (check-expect (flatten '(1 2 3 4)) '(1 2 3 4))  ;; list: many atom elems
        (check-expect (flatten '(h (e l) l (o))) 
            '(h e l l o))                               ;; nested lists
        (check-expect (flatten '((((9))))) '(9))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;; Exercise 31


;; (takewhile) Given a predicate p? and a list of S-expressions xs, takewhile
;;             returns the longest prefix of the list in which every element 
;;             satisfies the predicate

;; laws
;;    takewhile p? '() == '()
;;    takewhile p? (cons x xs) == (cons x (takewhile p? xs))
;;                where (p? x) == #t
;;    takewhile p? (cons x xs) == '()
;;                where (p? x) == #f

(define takewhile (p? xs)
    (if (null? xs) 
        '()
        (if (p? (car xs))
            (cons (car xs) (takewhile p? (cdr xs)))
            '())))


        ;; unit tests
        (check-expect (takewhile number? '()) '())  ;; null case
        (check-expect 
            (takewhile number? '(1 2)) '(1 2))      ;; whole list satisfies
        (check-expect 
            (takewhile number? '(1 2 (1 2 3 4) 5)) 
            '(1 2))                                 ;; part of list satisfies
        (check-expect 
            (takewhile boolean? '(#t #f #t #f 2)) 
            '(#t #f #t #f))
        (check-expect 
            (takewhile pair? '(1 2 3)) '())         ;; none of list satisfies


;; (dropwhile) Given a predicate p? and a list of S-expressions xs, dropwhile
;;             drops the longest prefix of the list in which every element 
;;             satisfies the predicate, and returns whatever is leftover

;; laws
;;    dropwhile p? '() == '()
;;    dropwhile p? (cons x xs) == (dropwhile p? xs)
;;            where (p? x) == #t
;;    dropwhile p? (cons x xs) == xs
;;            where (p? x) == #f

(define dropwhile (p? xs)
    (if (null? xs) 
        '()
        (if (p? (car xs))
            (dropwhile p? (cdr xs))
            xs)))


        ;; unit tests
        (check-expect (dropwhile number? '()) '()) ;; null case
        (check-expect 
            (dropwhile number? '(1 2)) '())        ;; whole list satisfies
        (check-expect 
            (dropwhile number? '(1 2 (1 2 3 4) 5)) 
            '((1 2 3 4) 5))                        ;; part of list satisfies
        (check-expect 
            (dropwhile boolean? '(#t #f #t #f 2)) '(2))
        (check-expect 
            (dropwhile pair? '(1 2 3)) '(1 2 3))   ;; none of list satisfies


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;; Exercise B


;; (take n xs) Given a natural number n (including 0) and a list of values xs,
;;             take returns the longest prefix of xs that contains at most n
;;             elements

;; laws
;;    (take n '()) == '()
;;    (take 0 (cons x xs)) == '()
;;    (take n (cons x xs)) == (cons x (take (- n 1) xs)) where n != 0

(define take (n xs)
    (if (null? xs)
        '()
        (if (= n 0)
            '()
            (cons (car xs) (take (- n 1) (cdr xs))))))


        ;; unit tests
        (check-expect (take 2 '()) '())                   ;; xs is null
        (check-expect (take 0 '(1 2 3 4 5)) '())          ;; n = 0
        (check-expect (take 3 '(1 2 3 4 5)) '(1 2 3))     ;; take some n elems
        (check-expect (take 4 '(8 9 10 11)) '(8 9 10 11)) ;; take all n elems
        (check-expect (take 6 '(20 19 18)) '(20 19 18))   ;; take > all n elems


;; (drop n xs) Given a natural number n (including 0) and a list of values xs,
;;             drop removes n elements from the front of the list and 
;;             returns the results

;; laws
;;    (drop n '()) == '()
;;    (drop 0 (cons x xs)) == xs
;;    (drop n (cons x xs)) == (drop (- n 1) xs)

(define drop (n xs)
    (if (null? xs)
        '()
        (if (= n 0)
            xs
            (drop (- n 1) (cdr xs)))))


        ;; unit tests
        (check-expect (drop 2 '()) '())                   ;; xs is null
        (check-expect (drop 0 '(1 2 3 4 5)) '(1 2 3 4 5)) ;; n = 0
        (check-expect (drop 3 '(1 2 3 4 5)) '(4 5))       ;; drop some n elems
        (check-expect (drop 4 '(8 9 10 11)) '())          ;; drop all n elems
        (check-expect (drop 6 '(20 19 18)) '())           ;; drop > all n elems


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;; Exercise C


;; (zip) Given two lists of values xs and ys, which both have equal length, zip
;;       converts the pair of lists to a list of pairs by pairing corresponding
;;       indexes; returns the results of the conversion

;; laws
;;    (zip '() '()) == '()
;;    (zip (cons x xs) (cons y ys)) == 
;;        (append (list1 (cons x (cons y '()))) (zip xs ys))

(define zip (xs ys)
    (if (and (null? xs) (null? ys))
        '()
        (append (list1 (cons (car xs) (cons (car ys) '()))) ;; appends pair of
            (zip (cdr xs) (cdr ys)))))                      ;; first two with
                                                            ;; rest of elems

        ;; unit tests
        (check-expect (zip '() '()) '())                  ;; both empty lists
        (check-expect 
            (zip '(Norman) '(Ramsey)) '((Norman Ramsey))) ;; lists: one elem
        (check-expect 
            (zip '(1 2 3) '(4 5 6)) '((1 4) (2 5) (3 6))) ;; lists: many elems
        (check-expect 
            (zip '(11 11 15) '(Guyer Sheldon Korman)) 
            '((11 Guyer) (11 Sheldon) (15 Korman)))


;; (unzip) Given a list of pairs xs, unzip converts the list of pairs to a pair
;;         of lists by grouping the first elements and the second elements
;;         of each pair seperately; returns the results of the conversion

;; laws
;;    not mandatory

(define unzip (xs)
    (if (null? xs)
        '()
        (list2 (map car xs) (map cadr xs))))   ;; makes 2 lists of first elems
                                               ;; and second elems

        ;; unit tests
        (check-expect (unzip '()) '())                   ;; empty list
        (check-expect 
            (unzip '((Hello There))) '((Hello) (There))) ;; one pair   
        (check-expect 
            (unzip '((1 5) (2 6) (3 7) (4 8)))
            '((1 2 3 4) (5 6 7 8)))                      ;; multiple pairs
        (check-expect 
            (unzip '((11 Guyer) (11 Sheldon) (15 Korman))) 
            '((11 11 15) (Guyer Sheldon Korman)))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;; Exercise D


;; for unit testing purposes: Given number a, returns a * a
(define square (a) (* a a))

;; helper function

;; (find-max-elem) Given a function fun that maps a value in set A to a number,
;;                  a value m, and a nonempty list of values xs,
;;                  find-max-elem returns the element in xs for which (fun xs)
;;                  or (fun m) is the largest possible value

;; laws
;;    (find-max-elem fun m '()) == m
;;    (find-max-elem fun m (cons x xs)) == (find_max_value fun x xs)
;;            where fun x > fun m
;;    (find-max-elem fun m (cons x xs)) ==  (find_max_value fun m xs)
;;            where fun x <= fun m

(define find-max-elem (fun m xs)
    (if (null? xs)
        m
        (if (> (fun (car xs)) (fun m))
            (find-max-elem fun (car xs) (cdr xs))
            (find-max-elem fun m (cdr xs)))))


        ;; unit tests
        (check-expect (find-max-elem square 5 '()) 5) ;; xs is null
        (check-expect 
            (find-max-elem car '(105 1) '((15 2))) 
            '(105 1))                                 ;; fun m > any other elem
        (check-expect 
            (find-max-elem car 
             '(105 1) '((160 2) (170 3))) '(170 3))   ;; fun m < elem everytime
        (check-expect 
            (find-max-elem max* 
             '(1 6) '((7 19 8) (17 16))) '(7 19 8))   ;; mix
        (check-expect 
            (find-max-elem min* 
             '(1 6) '((7 19 8) (17 16) (18 16))) 
              '(17 16))                               ;; tie (goes to first)


;; main function

;; (arg-max) Given a function fun that maps a value in set A to a number
;;           and a nonempty list of values xs, arg-max returns the 
;;           element in xs for which (fun xs) is the largest possible value

;; laws
;;    (arg-max fun (cons x xs)) == (find_max_value fun x xs)

(define arg-max (fun xs)
    (find-max-elem fun (car xs) (cdr xs)))


        ;; unit tests
        (check-expect (arg-max square '(1 2 3 4 5)) 5) ;; function with atoms
        (check-expect
            (arg-max car '((105 1))) '(105 1))         ;; function with lists
        (check-expect 
            (arg-max car '( (105 1) (160 2) (170 3))) 
            '(170 3))
        (check-expect 
            (arg-max max* '( (1 6) (7 19 8) (17 16))) 
            '(7 19 8))                                 
        (check-expect 
            (arg-max min* '( (1 6) (7 19 8) (17 16) (18 16)))
             '(17 16))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;; Exercise E


;; (rightmost-point) Given a nonempty list of point records xs, rightmost-point
;;                   returns the record with the largest x coordinate

;; laws
;;    not mandatory

;; definition of record point: contains x and y coordinates
(record point [x y])

(define rightmost-point (xs)
    (arg-max point-x xs))


        ;; unit tests
        (check-expect
            (rightmost-point '((make-point 3 4))) 
            '(make-point 3 4))                      ;; one point
        (check-expect
            (rightmost-point 
             '((make-point 3 4) (make-point 9 1))) 
              '(make-point 9 1))                    ;; multiple points
        (check-expect 
            (rightmost-point 
             '((make-point -2 4) (make-point -4 7)))
              '(make-point -2 4))                   ;; multiple points, negative
        (check-expect 
            (rightmost-point 
             '((make-point 3 4) (make-point 3 4)))
              '(make-point 3 4))                    ;; tie (goes to first)