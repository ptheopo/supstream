#ifndef UNIT_TESTS_H
# define UNIT_TESTS_H

# include "CUnit/Basic.h"

# include <stdbool.h> 

# include "ast.h"

int     suite_init_ast(void);
int     suite_clean_ast(void);
void    test_ast_node_new(void);
void    test_ast_node_is_iblock(void);
void    test_ast_node_is_iline(void);
void    test_ast_node_is_iscalar(void);
void    test_ast_iscalar_value(void);
void    test_ast_ilb_add_simple(void);
void    test_ast_ilb_add(void);

#endif
