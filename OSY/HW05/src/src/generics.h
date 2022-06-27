#define _TAKE_TYPE(TYPE, ALIAS) TYPE
#define _TAKE_ALIAS(TYPE, ALIAS) ALIAS

#define _GET_TYPE(TYPE_LITERAL) _TAKE_TYPE(TYPE_LITERAL)
#define _GET_ALIAS(TYPE_LITERAL) _TAKE_ALIAS(TYPE_LITERAL)

#define _CONCAT2(A, B) A ## _ ## B
#define _CONCAT3(A, B, C) A ## _ ## B ## _ ## C

#define T _GET_TYPE(TYPE)
#define TAlias _GET_ALIAS(TYPE)

#define T_NAME(TYPENAME, TYPE) _CONCAT2(TYPENAME, TYPE)
#define F_NAME(TYPENAME, TYPE, NAME) _CONCAT3(TYPENAME, TYPE, NAME)