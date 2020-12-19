#ifndef MINUNIT_H
#define MINUNIT_H

/*
 * Based on: http://www.jera.com/techinfo/jtns/jtn002.html
 */

#define mu_assert(test, message) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                               if (message) return message; } while (0)

extern int tests_run;

#endif /* MINUNIT_H */
