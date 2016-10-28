(define = integer-equal?)
(define > integer-greater?)
(define < integer-less?)

(define (>= a b)
  (or (= a b)
      (> a b)))

(define (<= a b)
  (or (= a b)
	  (< a b)))

(define (!= a b)
  (if (= a b)
      #f
      #t))
  
(define (++ n) (+ n 1))
(define (-- n) (- n 1))

(define (pow x n)
	      (if (= n 0)
		  1
		  (* x (pow x (- n 1)))))
(define (! x)
	    (if (= x 0)
		1
		(* x (! (- x 1)))))

(define (not a) (if a #f #t))

(define (map f l) (if (nil? l)
		      l
		      (cons (f (car l))
			    (map f (cdr l)))))
(define (map-w-idx f l)
  (define (map-w-idx-true f l i)
    (if (nil? l)
	l
	(cons (f (car l) i) (map-w-idx-true f (cdr l) (+ i 1)))
	))
  (map-w-idx-true f l 0))


(define (for-each f l) (if (nil? l)
			   #void
			   ((lambda ()
			      (f (car l))
			      (for-each f (cdr l))))))
(define (for-each-w-idx f l)
  (define (for-each-w-idx-true f l i)
    (if (nil? l)
	#void
	(begin
	  (f (car l) i)
	  (for-eachw-idx-true f (cdr l) (+ i 1)))))
  (for-each-w-idx-true f l 0))


(define (const a) (lambda (b) a))

(define (foldr fun id li)
    (if (nil? li)
	id
	(foldr fun (fun id (car li)) (cdr li))))

(define (sum li)
  (foldr (lambda (a b) (+ a b)) 0 li))

(define list (lambda l l))

(define (list? l) (or (pair? l) (nil? l)))

(define (length l)
  (define (true-length l n)
    (if (nil? l)
	0
	(true-length (cdr l) (+ n 1))))
  (true-length l)
  )


(define (id a) a)


(define (caar l) (car (car l)))
(define (caaar l) (car (caar l)))
(define (caaaar l) (car (caaar l)))

(define (cddr l) (cdr (cdr l)))
(define (cdddr l) (cdr (cddr l)))
(define (cddddr l) (cdr (cdddr l)))


(define (cadr l) (car (cdr l)))
(define (caddr l) (car (cddr l)))
(define (cadar l) (car (cdr (car l))))


(define (cdar l) (cdr (car l)))

(define-macro (cond . args)
  (define clause (car args))
  (define condition (car clause))
  (if (eq? condition 'else)
      (set! condition #t)
      )
  (cons 'if  (cons condition (cons (cons 'begin (cdr clause)) (if (nil? (cdr args)) '() (list (cons 'cond (cdr args)))))))
  )


(define-macro (set-car! key val)
 (list 'set! key (list 'cons  val (list 'cdr key))))

(define-macro (set-cdr! key val)
  (list 'set! key (list 'cons (list 'car key) val)))

(define-macro (begin . body)
  (list (cons 'lambda (cons '() body)))
  )



(define (append a b)
  (define (list-maker l)
    (if (nil? (cdr l))
	(cons (car l) b)
	(list-maker (cons (car l)  (list-maker (cdr l))))
	))
  (cond
   ((nil? a) b)
   ((nil? b) a)
   (else (list-maker a))))
 

(define-macro (let a b . rest)
  (define args (if (symbol? a) b a))
  (define arg-symbols (map car args))
  (define arg-vals   (map cadr args))
  (define body (if (symbol? a) rest (cons b rest)))
  (define func (cons 'lambda (cons arg-symbols body)))
  (if (symbol? a)
      (list 'begin (list 'define a func) (cons a arg-vals))
      (cons func arg-vals)
      )
  ) 



