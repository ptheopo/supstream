#ifndef UNIT_TESTS_H
# define UNIT_TESTS_H

# include "CUnit/Basic.h"

# include "ast.h"

int     init_suite1(void);
int     clean_suite1(void);
void    test_ast_node_new(void);

#endif
