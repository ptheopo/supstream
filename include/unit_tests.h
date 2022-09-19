#ifndef UNIT_TESTS_H
# define UNIT_TESTS_H

# include "CUnit/Basic.h"

# include "ast.h"

int     suite_init_ast(void);
int     suite_clean_ast(void);
void    test_ast_node_new(void);

#endif
