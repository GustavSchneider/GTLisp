(include "test_util.scm")

(define (test-eq?)
  (let ((msg "test-eq-failed"))
    (assert (eq? 'a 'a) msg)
    (assert (eq? 'banan 'banan) msg)
    (assert (not (eq? 1 1)) msg)))

(pretty-print "running primitive_proc_test.scm")
(test-eq?)
(pretty-print "OK!")
