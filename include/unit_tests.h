#ifndef UNIT_TESTS_H
# define UNIT_TESTS_H

# include "CUnit/Basic.h"

# include <stdbool.h> 

# include "ast.h"

/* AST From Scratch */

int     suite_init_ast_fscratch(void);
int     suite_clean_ast_fscratch(void);
void    test_ast_fscratch_node_new(void);
void    test_ast_fscratch_node_is_iblock(void);
void    test_ast_fscratch_node_is_iline(void);
void    test_ast_fscratch_node_is_iscalar(void);
void    test_ast_fscratch_iscalar_value(void);
void    test_ast_fscratch_ilb_add_simple(void);
void    test_ast_fscratch_ilb_add(void);

/* AST From Sample */

int     suite_init_ast_fsample(void);
int     suite_clean_ast_fsample(void);

#endif
