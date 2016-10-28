(define (assert cond msg)
  (if cond
      #void
      (begin
	(pretty-print msg)
	(quit))))
