/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "src/parser/parser.y"

#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
#include <iostream>
#include "ast/tree.hpp"
#define YYDEBUG 1
void yyerror(const char *s);
extern int yylex(void);
extern int yylineno;
using namespace AST;
extern NodePtr root;

template <typename T>
inline std::shared_ptr<T> shared_cast(Node *ptr) {
  return std::shared_ptr<T>(static_cast<T *>(ptr));
}

inline std::string take_string(char *s) {
    std::string result(s);
    free(s);
    return result;
}


#line 98 "src/parser/parser.tab.cc"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.hh"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ADD = 3,                        /* "+"  */
  YYSYMBOL_SUB = 4,                        /* "-"  */
  YYSYMBOL_MUL = 5,                        /* "*"  */
  YYSYMBOL_DIV = 6,                        /* "/"  */
  YYSYMBOL_MOD = 7,                        /* "%"  */
  YYSYMBOL_ASSIGN = 8,                     /* "="  */
  YYSYMBOL_SEMICOLON = 9,                  /* ";"  */
  YYSYMBOL_COMMA = 10,                     /* ","  */
  YYSYMBOL_LPAREN = 11,                    /* "("  */
  YYSYMBOL_RPAREN = 12,                    /* ")"  */
  YYSYMBOL_LBRACE = 13,                    /* "{"  */
  YYSYMBOL_RBRACE = 14,                    /* "}"  */
  YYSYMBOL_LBRACK = 15,                    /* "["  */
  YYSYMBOL_RBRACK = 16,                    /* "]"  */
  YYSYMBOL_INT = 17,                       /* "int"  */
  YYSYMBOL_VOID = 18,                      /* "void"  */
  YYSYMBOL_IF = 19,                        /* "if"  */
  YYSYMBOL_ELSE = 20,                      /* "else"  */
  YYSYMBOL_WHILE = 21,                     /* "while"  */
  YYSYMBOL_RETURN = 22,                    /* "return"  */
  YYSYMBOL_LT = 23,                        /* "<"  */
  YYSYMBOL_GT = 24,                        /* ">"  */
  YYSYMBOL_LE = 25,                        /* "<="  */
  YYSYMBOL_GE = 26,                        /* ">="  */
  YYSYMBOL_EQ = 27,                        /* "=="  */
  YYSYMBOL_NE = 28,                        /* "!="  */
  YYSYMBOL_LAND = 29,                      /* "&&"  */
  YYSYMBOL_LOR = 30,                       /* "||"  */
  YYSYMBOL_NOT = 31,                       /* "!"  */
  YYSYMBOL_IDENT = 32,                     /* IDENT  */
  YYSYMBOL_INTCONST = 33,                  /* INTCONST  */
  YYSYMBOL_LOWER_THAN_ELSE = 34,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_YYACCEPT = 35,                  /* $accept  */
  YYSYMBOL_AstRoot = 36,                   /* AstRoot  */
  YYSYMBOL_CompUnit = 37,                  /* CompUnit  */
  YYSYMBOL_Decl = 38,                      /* Decl  */
  YYSYMBOL_BType = 39,                     /* BType  */
  YYSYMBOL_VarDecl = 40,                   /* VarDecl  */
  YYSYMBOL_VarDefs = 41,                   /* VarDefs  */
  YYSYMBOL_VarDef = 42,                    /* VarDef  */
  YYSYMBOL_VarDefHead = 43,                /* VarDefHead  */
  YYSYMBOL_OptInit = 44,                   /* OptInit  */
  YYSYMBOL_InitVal = 45,                   /* InitVal  */
  YYSYMBOL_InitValList = 46,               /* InitValList  */
  YYSYMBOL_FuncDef = 47,                   /* FuncDef  */
  YYSYMBOL_FuncFParams = 48,               /* FuncFParams  */
  YYSYMBOL_FuncFParam = 49,                /* FuncFParam  */
  YYSYMBOL_ParamDimsTail = 50,             /* ParamDimsTail  */
  YYSYMBOL_Block = 51,                     /* Block  */
  YYSYMBOL_BlockItems = 52,                /* BlockItems  */
  YYSYMBOL_BlockItem = 53,                 /* BlockItem  */
  YYSYMBOL_Stmt = 54,                      /* Stmt  */
  YYSYMBOL_Exp = 55,                       /* Exp  */
  YYSYMBOL_Cond = 56,                      /* Cond  */
  YYSYMBOL_LVal = 57,                      /* LVal  */
  YYSYMBOL_PrimaryExp = 58,                /* PrimaryExp  */
  YYSYMBOL_IntConst = 59,                  /* IntConst  */
  YYSYMBOL_UnaryExp = 60,                  /* UnaryExp  */
  YYSYMBOL_UnaryOp = 61,                   /* UnaryOp  */
  YYSYMBOL_FuncRParams = 62,               /* FuncRParams  */
  YYSYMBOL_MulExp = 63,                    /* MulExp  */
  YYSYMBOL_AddExp = 64,                    /* AddExp  */
  YYSYMBOL_RelExp = 65,                    /* RelExp  */
  YYSYMBOL_EqExp = 66,                     /* EqExp  */
  YYSYMBOL_LAndExp = 67,                   /* LAndExp  */
  YYSYMBOL_LOrExp = 68                     /* LOrExp  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  13
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   193

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  35
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  34
/* YYNRULES -- Number of rules.  */
#define YYNRULES  83
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  151

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   289


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    96,    96,   100,   101,   105,   106,   112,   115,   120,
     123,   124,   127,   136,   137,   143,   144,   147,   148,   149,
     152,   157,   170,   173,   176,   180,   186,   187,   193,   196,
     201,   210,   215,   221,   222,   225,   226,   229,   230,   233,
     236,   239,   242,   245,   248,   251,   254,   257,   262,   265,
     269,   272,   278,   279,   280,   283,   286,   287,   290,   294,
     299,   300,   301,   304,   305,   308,   309,   310,   311,   314,
     315,   316,   319,   320,   321,   322,   323,   326,   327,   328,
     331,   332,   335,   336
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "\"+\"", "\"-\"",
  "\"*\"", "\"/\"", "\"%\"", "\"=\"", "\";\"", "\",\"", "\"(\"", "\")\"",
  "\"{\"", "\"}\"", "\"[\"", "\"]\"", "\"int\"", "\"void\"", "\"if\"",
  "\"else\"", "\"while\"", "\"return\"", "\"<\"", "\">\"", "\"<=\"",
  "\">=\"", "\"==\"", "\"!=\"", "\"&&\"", "\"||\"", "\"!\"", "IDENT",
  "INTCONST", "LOWER_THAN_ELSE", "$accept", "AstRoot", "CompUnit", "Decl",
  "BType", "VarDecl", "VarDefs", "VarDef", "VarDefHead", "OptInit",
  "InitVal", "InitValList", "FuncDef", "FuncFParams", "FuncFParam",
  "ParamDimsTail", "Block", "BlockItems", "BlockItem", "Stmt", "Exp",
  "Cond", "LVal", "PrimaryExp", "IntConst", "UnaryExp", "UnaryOp",
  "FuncRParams", "MulExp", "AddExp", "RelExp", "EqExp", "LAndExp",
  "LOrExp", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-120)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       3,    17,    22,    44,     3,  -120,  -120,  -120,    24,    67,
    -120,     2,    58,  -120,  -120,  -120,    -6,  -120,    50,    39,
      52,  -120,    -5,    78,  -120,    61,    35,  -120,  -120,  -120,
    -120,  -120,   160,     5,  -120,    84,  -120,  -120,  -120,    83,
    -120,  -120,  -120,   160,    60,    77,    86,    78,    41,    75,
    -120,    90,    93,    78,   106,  -120,  -120,    12,   137,   160,
    -120,   160,   160,   160,   160,   160,  -120,  -120,    78,  -120,
    -120,    50,   109,   112,   150,  -120,  -120,   100,  -120,  -120,
     103,    31,   114,  -120,  -120,  -120,    39,  -120,  -120,  -120,
      63,   119,  -120,  -120,  -120,    60,    60,  -120,   160,   160,
    -120,   128,  -120,  -120,  -120,   160,   124,  -120,   160,  -120,
    -120,   133,    77,   101,    73,   121,   122,   139,  -120,   146,
     127,   147,  -120,   125,   160,   160,   160,   160,   160,   160,
     160,   160,   125,  -120,   149,   134,   152,    77,    77,    77,
      77,   101,   101,    73,   121,  -120,  -120,   157,   125,  -120,
    -120
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     2,     5,     7,     3,    13,     0,
      10,    15,     0,     1,     6,     4,     0,     9,     0,     0,
       0,    12,     0,     0,     8,     0,     0,    26,    13,    11,
      60,    61,     0,     0,    62,    50,    55,    16,    17,    53,
      56,    54,    65,     0,    69,    48,     0,     0,     0,     0,
      22,    28,     0,     0,     0,    18,    20,     0,     0,     0,
      59,     0,     0,     0,     0,     0,    14,    23,     0,    40,
      33,     0,     0,     0,     0,    38,    42,     0,    35,    37,
       0,    53,     0,    27,    24,    52,     0,    19,    57,    63,
       0,     0,    66,    67,    68,    70,    71,    25,     0,     0,
      46,     0,    34,    36,    41,     0,    29,    21,     0,    58,
      51,     0,    72,    77,    80,    82,    49,     0,    47,     0,
       0,    30,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    39,     0,     0,    43,    73,    74,    75,
      76,    78,    79,    81,    83,    45,    31,     0,     0,    32,
      44
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -120,  -120,  -120,    30,  -120,  -120,  -120,   148,  -120,  -120,
     -31,  -120,   170,   153,   126,  -120,   -20,  -120,    99,  -119,
     -18,    80,   -49,  -120,  -120,   -38,  -120,  -120,    51,   -67,
      14,    47,    49,  -120
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     3,     4,    75,    25,     6,     9,    10,    11,    21,
      37,    57,     7,    26,    27,   121,    76,    77,    78,    79,
      80,   111,    39,    40,    41,    42,    43,    90,    44,    45,
     113,   114,   115,   116
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      81,    38,    56,    50,   136,    60,    23,    47,    30,    31,
      19,    24,    24,   145,    54,    38,    32,    20,    33,    55,
       1,     2,    86,    92,    93,    94,    87,    67,    81,   150,
       5,   112,   112,    84,    14,    16,    34,    35,    36,   105,
      89,    91,    30,    31,    13,    52,    59,    53,    97,     8,
      32,    52,    33,    68,    12,   107,   101,   137,   138,   139,
     140,   112,   112,   112,   112,    61,    62,    63,    38,    22,
      34,    35,    36,   108,    81,   109,    17,    18,    30,    31,
      64,    65,    28,    81,    69,    46,    32,   119,    49,    70,
     122,    49,    71,    51,    72,    58,    73,    74,    59,    81,
     128,   129,    66,    30,    31,    82,    34,    35,    36,    69,
      24,    32,   104,    49,   102,    95,    96,    71,    85,    72,
      98,    73,    74,    99,   124,   125,   126,   127,    30,    31,
     106,    34,    35,    36,    69,   110,    32,   118,    49,   120,
      30,    31,   141,   142,    72,   123,    73,    74,    32,    88,
     130,   132,   131,    30,    31,   133,    34,    35,    36,   100,
     134,    32,   135,    30,    31,   146,    29,   147,    34,    35,
      36,    32,   148,   149,    15,    48,   103,   143,    83,   117,
     144,    34,    35,    36,     0,     0,     0,     0,     0,     0,
       0,    34,    35,    36
};

static const yytype_int16 yycheck[] =
{
      49,    19,    33,    23,   123,    43,    12,    12,     3,     4,
       8,    17,    17,   132,    32,    33,    11,    15,    13,    14,
      17,    18,    10,    61,    62,    63,    14,    47,    77,   148,
       0,    98,    99,    53,     4,    11,    31,    32,    33,     8,
      58,    59,     3,     4,     0,    10,    15,    12,    68,    32,
      11,    10,    13,    12,    32,    86,    74,   124,   125,   126,
     127,   128,   129,   130,   131,     5,     6,     7,    86,    11,
      31,    32,    33,    10,   123,    12,     9,    10,     3,     4,
       3,     4,    32,   132,     9,    33,    11,   105,    13,    14,
     108,    13,    17,    32,    19,    11,    21,    22,    15,   148,
      27,    28,    16,     3,     4,    15,    31,    32,    33,     9,
      17,    11,     9,    13,    14,    64,    65,    17,    12,    19,
      11,    21,    22,    11,    23,    24,    25,    26,     3,     4,
      16,    31,    32,    33,     9,    16,    11,     9,    13,    15,
       3,     4,   128,   129,    19,    12,    21,    22,    11,    12,
      29,    12,    30,     3,     4,     9,    31,    32,    33,     9,
      33,    11,    15,     3,     4,    16,    18,    33,    31,    32,
      33,    11,    20,    16,     4,    22,    77,   130,    52,    99,
     131,    31,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    33
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    17,    18,    36,    37,    38,    40,    47,    32,    41,
      42,    43,    32,     0,    38,    47,    11,     9,    10,     8,
      15,    44,    11,    12,    17,    39,    48,    49,    32,    42,
       3,     4,    11,    13,    31,    32,    33,    45,    55,    57,
      58,    59,    60,    61,    63,    64,    33,    12,    48,    13,
      51,    32,    10,    12,    55,    14,    45,    46,    11,    15,
      60,     5,     6,     7,     3,     4,    16,    51,    12,     9,
      14,    17,    19,    21,    22,    38,    51,    52,    53,    54,
      55,    57,    15,    49,    51,    12,    10,    14,    12,    55,
      62,    55,    60,    60,    60,    63,    63,    51,    11,    11,
       9,    55,    14,    53,     9,     8,    16,    45,    10,    12,
      16,    56,    64,    65,    66,    67,    68,    56,     9,    55,
      15,    50,    55,    12,    23,    24,    25,    26,    27,    28,
      29,    30,    12,     9,    33,    15,    54,    64,    64,    64,
      64,    65,    65,    66,    67,    54,    16,    33,    20,    16,
      54
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    35,    36,    37,    37,    37,    37,    38,    39,    40,
      41,    41,    42,    43,    43,    44,    44,    45,    45,    45,
      46,    46,    47,    47,    47,    47,    48,    48,    49,    49,
      49,    50,    50,    51,    51,    52,    52,    53,    53,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    55,    56,
      57,    57,    58,    58,    58,    59,    60,    60,    60,    60,
      61,    61,    61,    62,    62,    63,    63,    63,    63,    64,
      64,    64,    65,    65,    65,    65,    65,    66,    66,    66,
      67,    67,    68,    68
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     2,     1,     1,     3,
       1,     3,     2,     1,     4,     0,     2,     1,     2,     3,
       1,     3,     5,     5,     6,     6,     1,     3,     2,     4,
       5,     3,     4,     2,     3,     1,     2,     1,     1,     4,
       1,     2,     1,     5,     7,     5,     2,     3,     1,     1,
       1,     4,     3,     1,     1,     1,     1,     3,     4,     2,
       1,     1,     1,     1,     3,     1,     3,     3,     3,     1,
       3,     3,     1,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yykind)
    {
    case YYSYMBOL_IDENT: /* IDENT  */
#line 46 "src/parser/parser.y"
            { free(((*yyvaluep).str_val)); }
#line 1272 "src/parser/parser.tab.cc"
        break;

      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* AstRoot: CompUnit  */
#line 96 "src/parser/parser.y"
                   { root = NodePtr((yyvsp[0].node)); }
#line 1545 "src/parser/parser.tab.cc"
    break;

  case 3: /* CompUnit: FuncDef  */
#line 100 "src/parser/parser.y"
                   { (yyval.node) = new CompUnit(NodePtr((yyvsp[0].node))); }
#line 1551 "src/parser/parser.tab.cc"
    break;

  case 4: /* CompUnit: CompUnit FuncDef  */
#line 101 "src/parser/parser.y"
                       {
          static_cast<CompUnit*>((yyvsp[-1].node))->add_unit(NodePtr((yyvsp[0].node)));
          (yyval.node) = (yyvsp[-1].node);
      }
#line 1560 "src/parser/parser.tab.cc"
    break;

  case 5: /* CompUnit: Decl  */
#line 105 "src/parser/parser.y"
           { (yyval.node) = new CompUnit(NodePtr((yyvsp[0].node))); }
#line 1566 "src/parser/parser.tab.cc"
    break;

  case 6: /* CompUnit: CompUnit Decl  */
#line 106 "src/parser/parser.y"
                    {
          static_cast<CompUnit*>((yyvsp[-1].node))->add_unit(NodePtr((yyvsp[0].node)));
          (yyval.node) = (yyvsp[-1].node);
      }
#line 1575 "src/parser/parser.tab.cc"
    break;

  case 7: /* Decl: VarDecl  */
#line 112 "src/parser/parser.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1581 "src/parser/parser.tab.cc"
    break;

  case 8: /* BType: "int"  */
#line 115 "src/parser/parser.y"
              { (yyval.btype) = BasicType::Int; }
#line 1587 "src/parser/parser.tab.cc"
    break;

  case 9: /* VarDecl: "int" VarDefs ";"  */
#line 120 "src/parser/parser.y"
                            { static_cast<VarDecl *>((yyvsp[-1].node))->btype = BasicType::Int; (yyval.node) = (yyvsp[-1].node); }
#line 1593 "src/parser/parser.tab.cc"
    break;

  case 10: /* VarDefs: VarDef  */
#line 123 "src/parser/parser.y"
                 { (yyval.node) = new VarDecl(shared_cast<VarDef>((yyvsp[0].node))); }
#line 1599 "src/parser/parser.tab.cc"
    break;

  case 11: /* VarDefs: VarDefs "," VarDef  */
#line 124 "src/parser/parser.y"
                         { static_cast<VarDecl*>((yyvsp[-2].node))->add_def(shared_cast<VarDef>((yyvsp[0].node))); (yyval.node) = (yyvsp[-2].node); }
#line 1605 "src/parser/parser.tab.cc"
    break;

  case 12: /* VarDef: VarDefHead OptInit  */
#line 127 "src/parser/parser.y"
                            {
          if ((yyvsp[0].node)) {
            static_cast<VarDef*>((yyvsp[-1].node))->set_init(shared_cast<InitVal>((yyvsp[0].node)));
          }
          (yyval.node) = (yyvsp[-1].node);
      }
#line 1616 "src/parser/parser.tab.cc"
    break;

  case 13: /* VarDefHead: IDENT  */
#line 136 "src/parser/parser.y"
                   { (yyval.node) = new VarDef(take_string((yyvsp[0].str_val))); }
#line 1622 "src/parser/parser.tab.cc"
    break;

  case 14: /* VarDefHead: VarDefHead "[" INTCONST "]"  */
#line 137 "src/parser/parser.y"
                                  {
          static_cast<VarDef*>((yyvsp[-3].node))->add_dim((yyvsp[-1].int_val));
          (yyval.node) = (yyvsp[-3].node);
      }
#line 1631 "src/parser/parser.tab.cc"
    break;

  case 15: /* OptInit: %empty  */
#line 143 "src/parser/parser.y"
          { (yyval.node) = nullptr; }
#line 1637 "src/parser/parser.tab.cc"
    break;

  case 16: /* OptInit: "=" InitVal  */
#line 144 "src/parser/parser.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 1643 "src/parser/parser.tab.cc"
    break;

  case 17: /* InitVal: Exp  */
#line 147 "src/parser/parser.y"
              { (yyval.node) = new InitVal(NodePtr((yyvsp[0].node))); }
#line 1649 "src/parser/parser.tab.cc"
    break;

  case 18: /* InitVal: "{" "}"  */
#line 148 "src/parser/parser.y"
              { (yyval.node) = new InitVal(); }
#line 1655 "src/parser/parser.tab.cc"
    break;

  case 19: /* InitVal: "{" InitValList "}"  */
#line 149 "src/parser/parser.y"
                          { (yyval.node) = (yyvsp[-1].node); }
#line 1661 "src/parser/parser.tab.cc"
    break;

  case 20: /* InitValList: InitVal  */
#line 152 "src/parser/parser.y"
                      {
          auto init = new InitVal();
          init->add_elem(shared_cast<InitVal>((yyvsp[0].node)));
          (yyval.node) = init;
      }
#line 1671 "src/parser/parser.tab.cc"
    break;

  case 21: /* InitValList: InitValList "," InitVal  */
#line 157 "src/parser/parser.y"
                              {
          static_cast<InitVal*>((yyvsp[-2].node))->add_elem(shared_cast<InitVal>((yyvsp[0].node)));
          (yyval.node) = (yyvsp[-2].node);
      }
#line 1680 "src/parser/parser.tab.cc"
    break;

  case 22: /* FuncDef: "int" IDENT "(" ")" Block  */
#line 170 "src/parser/parser.y"
                                    {
          (yyval.node) = new FuncDef(BasicType::Int, take_string((yyvsp[-3].str_val)), shared_cast<Block>((yyvsp[0].node)));
      }
#line 1688 "src/parser/parser.tab.cc"
    break;

  case 23: /* FuncDef: "void" IDENT "(" ")" Block  */
#line 173 "src/parser/parser.y"
                                 {
          (yyval.node) = new FuncDef(BasicType::Void, take_string((yyvsp[-3].str_val)), shared_cast<Block>((yyvsp[0].node)));
      }
#line 1696 "src/parser/parser.tab.cc"
    break;

  case 24: /* FuncDef: "int" IDENT "(" FuncFParams ")" Block  */
#line 176 "src/parser/parser.y"
                                            {
          auto plist = shared_cast<ParamList>((yyvsp[-2].node));
          (yyval.node) = new FuncDef(BasicType::Int, take_string((yyvsp[-4].str_val)), plist->params, shared_cast<Block>((yyvsp[0].node)));
      }
#line 1705 "src/parser/parser.tab.cc"
    break;

  case 25: /* FuncDef: "void" IDENT "(" FuncFParams ")" Block  */
#line 180 "src/parser/parser.y"
                                             {
          auto plist = shared_cast<ParamList>((yyvsp[-2].node));
          (yyval.node) = new FuncDef(BasicType::Void, take_string((yyvsp[-4].str_val)), plist->params, shared_cast<Block>((yyvsp[0].node)));
      }
#line 1714 "src/parser/parser.tab.cc"
    break;

  case 26: /* FuncFParams: FuncFParam  */
#line 186 "src/parser/parser.y"
                         { (yyval.node) = new ParamList(shared_cast<Param>((yyvsp[0].node))); }
#line 1720 "src/parser/parser.tab.cc"
    break;

  case 27: /* FuncFParams: FuncFParams "," FuncFParam  */
#line 187 "src/parser/parser.y"
                                 {
          static_cast<ParamList*>((yyvsp[-2].node))->add_param(shared_cast<Param>((yyvsp[0].node)));
          (yyval.node) = (yyvsp[-2].node);
      }
#line 1729 "src/parser/parser.tab.cc"
    break;

  case 28: /* FuncFParam: BType IDENT  */
#line 193 "src/parser/parser.y"
                         {
          (yyval.node) = new Param((yyvsp[-1].btype), take_string((yyvsp[0].str_val)));
      }
#line 1737 "src/parser/parser.tab.cc"
    break;

  case 29: /* FuncFParam: BType IDENT "[" "]"  */
#line 196 "src/parser/parser.y"
                          {
          auto p = new Param((yyvsp[-3].btype), take_string((yyvsp[-2].str_val)));
          p->set_array(true);
          (yyval.node) = p;
      }
#line 1747 "src/parser/parser.tab.cc"
    break;

  case 30: /* FuncFParam: BType IDENT "[" "]" ParamDimsTail  */
#line 201 "src/parser/parser.y"
                                        {
          auto p = new Param((yyvsp[-4].btype), take_string((yyvsp[-3].str_val)));
          p->set_array(true);
          auto dims = shared_cast<DimList>((yyvsp[0].node));
          for (int dim : dims->dims) p->add_dim(dim);
          (yyval.node) = p;
      }
#line 1759 "src/parser/parser.tab.cc"
    break;

  case 31: /* ParamDimsTail: "[" INTCONST "]"  */
#line 210 "src/parser/parser.y"
                                 {
          auto d = new DimList();
          d->add_dim((yyvsp[-1].int_val));
          (yyval.node) = d;
      }
#line 1769 "src/parser/parser.tab.cc"
    break;

  case 32: /* ParamDimsTail: ParamDimsTail "[" INTCONST "]"  */
#line 215 "src/parser/parser.y"
                                     {
          static_cast<DimList*>((yyvsp[-3].node))->add_dim((yyvsp[-1].int_val));
          (yyval.node) = (yyvsp[-3].node);
      }
#line 1778 "src/parser/parser.tab.cc"
    break;

  case 33: /* Block: "{" "}"  */
#line 221 "src/parser/parser.y"
                { (yyval.node) = new Block(); }
#line 1784 "src/parser/parser.tab.cc"
    break;

  case 34: /* Block: "{" BlockItems "}"  */
#line 222 "src/parser/parser.y"
                         { (yyval.node) = (yyvsp[-1].node); }
#line 1790 "src/parser/parser.tab.cc"
    break;

  case 35: /* BlockItems: BlockItem  */
#line 225 "src/parser/parser.y"
                       { (yyval.node) = new Block(NodePtr((yyvsp[0].node))); }
#line 1796 "src/parser/parser.tab.cc"
    break;

  case 36: /* BlockItems: BlockItems BlockItem  */
#line 226 "src/parser/parser.y"
                           { static_cast<Block*>((yyvsp[-1].node))->add_stmt(NodePtr((yyvsp[0].node))); (yyval.node) = (yyvsp[-1].node); }
#line 1802 "src/parser/parser.tab.cc"
    break;

  case 37: /* BlockItem: Stmt  */
#line 229 "src/parser/parser.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1808 "src/parser/parser.tab.cc"
    break;

  case 38: /* BlockItem: Decl  */
#line 230 "src/parser/parser.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 1814 "src/parser/parser.tab.cc"
    break;

  case 39: /* Stmt: LVal "=" Exp ";"  */
#line 233 "src/parser/parser.y"
                        {
          (yyval.node) = new AssignStmt(shared_cast<LVal>((yyvsp[-3].node)), NodePtr((yyvsp[-1].node)));
      }
#line 1822 "src/parser/parser.tab.cc"
    break;

  case 40: /* Stmt: ";"  */
#line 236 "src/parser/parser.y"
          {
          (yyval.node) = new ExprStmt();
      }
#line 1830 "src/parser/parser.tab.cc"
    break;

  case 41: /* Stmt: Exp ";"  */
#line 239 "src/parser/parser.y"
              {
          (yyval.node) = new ExprStmt(NodePtr((yyvsp[-1].node)));
      }
#line 1838 "src/parser/parser.tab.cc"
    break;

  case 42: /* Stmt: Block  */
#line 242 "src/parser/parser.y"
            {
          (yyval.node) = (yyvsp[0].node);
      }
#line 1846 "src/parser/parser.tab.cc"
    break;

  case 43: /* Stmt: "if" "(" Cond ")" Stmt  */
#line 245 "src/parser/parser.y"
                                                   {
          (yyval.node) = new IfStmt(NodePtr((yyvsp[-2].node)), NodePtr((yyvsp[0].node)));
      }
#line 1854 "src/parser/parser.tab.cc"
    break;

  case 44: /* Stmt: "if" "(" Cond ")" Stmt "else" Stmt  */
#line 248 "src/parser/parser.y"
                                         {
          (yyval.node) = new IfStmt(NodePtr((yyvsp[-4].node)), NodePtr((yyvsp[-2].node)), NodePtr((yyvsp[0].node)));
      }
#line 1862 "src/parser/parser.tab.cc"
    break;

  case 45: /* Stmt: "while" "(" Cond ")" Stmt  */
#line 251 "src/parser/parser.y"
                                {
          (yyval.node) = new WhileStmt(NodePtr((yyvsp[-2].node)), NodePtr((yyvsp[0].node)));
      }
#line 1870 "src/parser/parser.tab.cc"
    break;

  case 46: /* Stmt: "return" ";"  */
#line 254 "src/parser/parser.y"
                   {
          (yyval.node) = new ReturnStmt();
      }
#line 1878 "src/parser/parser.tab.cc"
    break;

  case 47: /* Stmt: "return" Exp ";"  */
#line 257 "src/parser/parser.y"
                       {
          (yyval.node) = new ReturnStmt(NodePtr((yyvsp[-1].node)));
      }
#line 1886 "src/parser/parser.tab.cc"
    break;

  case 48: /* Exp: AddExp  */
#line 262 "src/parser/parser.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 1892 "src/parser/parser.tab.cc"
    break;

  case 49: /* Cond: LOrExp  */
#line 265 "src/parser/parser.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1898 "src/parser/parser.tab.cc"
    break;

  case 50: /* LVal: IDENT  */
#line 269 "src/parser/parser.y"
             {
          (yyval.node) = new LVal(take_string((yyvsp[0].str_val)));
      }
#line 1906 "src/parser/parser.tab.cc"
    break;

  case 51: /* LVal: LVal "[" Exp "]"  */
#line 272 "src/parser/parser.y"
                       {
          static_cast<LVal*>((yyvsp[-3].node))->add_index(NodePtr((yyvsp[-1].node)));
          (yyval.node) = (yyvsp[-3].node);
      }
#line 1915 "src/parser/parser.tab.cc"
    break;

  case 52: /* PrimaryExp: "(" Exp ")"  */
#line 278 "src/parser/parser.y"
                         { (yyval.node) = (yyvsp[-1].node); }
#line 1921 "src/parser/parser.tab.cc"
    break;

  case 53: /* PrimaryExp: LVal  */
#line 279 "src/parser/parser.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 1927 "src/parser/parser.tab.cc"
    break;

  case 54: /* PrimaryExp: IntConst  */
#line 280 "src/parser/parser.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1933 "src/parser/parser.tab.cc"
    break;

  case 55: /* IntConst: INTCONST  */
#line 283 "src/parser/parser.y"
                    { (yyval.node) = new IntConst((yyvsp[0].int_val)); }
#line 1939 "src/parser/parser.tab.cc"
    break;

  case 56: /* UnaryExp: PrimaryExp  */
#line 286 "src/parser/parser.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1945 "src/parser/parser.tab.cc"
    break;

  case 57: /* UnaryExp: IDENT "(" ")"  */
#line 287 "src/parser/parser.y"
                    {
          (yyval.node) = new FuncCall(take_string((yyvsp[-2].str_val)));
      }
#line 1953 "src/parser/parser.tab.cc"
    break;

  case 58: /* UnaryExp: IDENT "(" FuncRParams ")"  */
#line 290 "src/parser/parser.y"
                                {
          static_cast<FuncCall*>((yyvsp[-1].node))->name = take_string((yyvsp[-3].str_val));
          (yyval.node) = (yyvsp[-1].node);
      }
#line 1962 "src/parser/parser.tab.cc"
    break;

  case 59: /* UnaryExp: UnaryOp UnaryExp  */
#line 294 "src/parser/parser.y"
                       {
          (yyval.node) = new UnaryExp((yyvsp[-1].op), NodePtr((yyvsp[0].node)));
      }
#line 1970 "src/parser/parser.tab.cc"
    break;

  case 60: /* UnaryOp: "+"  */
#line 299 "src/parser/parser.y"
              { (yyval.op) = UnaryOp::Pos; }
#line 1976 "src/parser/parser.tab.cc"
    break;

  case 61: /* UnaryOp: "-"  */
#line 300 "src/parser/parser.y"
          { (yyval.op) = UnaryOp::Neg; }
#line 1982 "src/parser/parser.tab.cc"
    break;

  case 62: /* UnaryOp: "!"  */
#line 301 "src/parser/parser.y"
          { (yyval.op) = UnaryOp::Not; }
#line 1988 "src/parser/parser.tab.cc"
    break;

  case 63: /* FuncRParams: Exp  */
#line 304 "src/parser/parser.y"
                  { (yyval.node) = new FuncCall(NodePtr((yyvsp[0].node))); }
#line 1994 "src/parser/parser.tab.cc"
    break;

  case 64: /* FuncRParams: FuncRParams "," Exp  */
#line 305 "src/parser/parser.y"
                          { static_cast<FuncCall*>((yyvsp[-2].node))->add_arg(NodePtr((yyvsp[0].node))); (yyval.node) = (yyvsp[-2].node); }
#line 2000 "src/parser/parser.tab.cc"
    break;

  case 65: /* MulExp: UnaryExp  */
#line 308 "src/parser/parser.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 2006 "src/parser/parser.tab.cc"
    break;

  case 66: /* MulExp: MulExp "*" UnaryExp  */
#line 309 "src/parser/parser.y"
                          { (yyval.node) = new BinaryExp(BinaryOp::Mul, NodePtr((yyvsp[-2].node)), NodePtr((yyvsp[0].node))); }
#line 2012 "src/parser/parser.tab.cc"
    break;

  case 67: /* MulExp: MulExp "/" UnaryExp  */
#line 310 "src/parser/parser.y"
                          { (yyval.node) = new BinaryExp(BinaryOp::Div, NodePtr((yyvsp[-2].node)), NodePtr((yyvsp[0].node))); }
#line 2018 "src/parser/parser.tab.cc"
    break;

  case 68: /* MulExp: MulExp "%" UnaryExp  */
#line 311 "src/parser/parser.y"
                          { (yyval.node) = new BinaryExp(BinaryOp::Mod, NodePtr((yyvsp[-2].node)), NodePtr((yyvsp[0].node))); }
#line 2024 "src/parser/parser.tab.cc"
    break;

  case 69: /* AddExp: MulExp  */
#line 314 "src/parser/parser.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2030 "src/parser/parser.tab.cc"
    break;

  case 70: /* AddExp: AddExp "+" MulExp  */
#line 315 "src/parser/parser.y"
                        { (yyval.node) = new BinaryExp(BinaryOp::Add, NodePtr((yyvsp[-2].node)), NodePtr((yyvsp[0].node))); }
#line 2036 "src/parser/parser.tab.cc"
    break;

  case 71: /* AddExp: AddExp "-" MulExp  */
#line 316 "src/parser/parser.y"
                        { (yyval.node) = new BinaryExp(BinaryOp::Sub, NodePtr((yyvsp[-2].node)), NodePtr((yyvsp[0].node))); }
#line 2042 "src/parser/parser.tab.cc"
    break;

  case 72: /* RelExp: AddExp  */
#line 319 "src/parser/parser.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2048 "src/parser/parser.tab.cc"
    break;

  case 73: /* RelExp: RelExp "<" AddExp  */
#line 320 "src/parser/parser.y"
                        { (yyval.node) = new BinaryExp(BinaryOp::Lt, NodePtr((yyvsp[-2].node)), NodePtr((yyvsp[0].node))); }
#line 2054 "src/parser/parser.tab.cc"
    break;

  case 74: /* RelExp: RelExp ">" AddExp  */
#line 321 "src/parser/parser.y"
                        { (yyval.node) = new BinaryExp(BinaryOp::Gt, NodePtr((yyvsp[-2].node)), NodePtr((yyvsp[0].node))); }
#line 2060 "src/parser/parser.tab.cc"
    break;

  case 75: /* RelExp: RelExp "<=" AddExp  */
#line 322 "src/parser/parser.y"
                         { (yyval.node) = new BinaryExp(BinaryOp::Le, NodePtr((yyvsp[-2].node)), NodePtr((yyvsp[0].node))); }
#line 2066 "src/parser/parser.tab.cc"
    break;

  case 76: /* RelExp: RelExp ">=" AddExp  */
#line 323 "src/parser/parser.y"
                         { (yyval.node) = new BinaryExp(BinaryOp::Ge, NodePtr((yyvsp[-2].node)), NodePtr((yyvsp[0].node))); }
#line 2072 "src/parser/parser.tab.cc"
    break;

  case 77: /* EqExp: RelExp  */
#line 326 "src/parser/parser.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2078 "src/parser/parser.tab.cc"
    break;

  case 78: /* EqExp: EqExp "==" RelExp  */
#line 327 "src/parser/parser.y"
                        { (yyval.node) = new BinaryExp(BinaryOp::Eq, NodePtr((yyvsp[-2].node)), NodePtr((yyvsp[0].node))); }
#line 2084 "src/parser/parser.tab.cc"
    break;

  case 79: /* EqExp: EqExp "!=" RelExp  */
#line 328 "src/parser/parser.y"
                        { (yyval.node) = new BinaryExp(BinaryOp::Ne, NodePtr((yyvsp[-2].node)), NodePtr((yyvsp[0].node))); }
#line 2090 "src/parser/parser.tab.cc"
    break;

  case 80: /* LAndExp: EqExp  */
#line 331 "src/parser/parser.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2096 "src/parser/parser.tab.cc"
    break;

  case 81: /* LAndExp: LAndExp "&&" EqExp  */
#line 332 "src/parser/parser.y"
                         { (yyval.node) = new BinaryExp(BinaryOp::LAnd, NodePtr((yyvsp[-2].node)), NodePtr((yyvsp[0].node))); }
#line 2102 "src/parser/parser.tab.cc"
    break;

  case 82: /* LOrExp: LAndExp  */
#line 335 "src/parser/parser.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2108 "src/parser/parser.tab.cc"
    break;

  case 83: /* LOrExp: LOrExp "||" LAndExp  */
#line 336 "src/parser/parser.y"
                          { (yyval.node) = new BinaryExp(BinaryOp::LOr, NodePtr((yyvsp[-2].node)), NodePtr((yyvsp[0].node))); }
#line 2114 "src/parser/parser.tab.cc"
    break;


#line 2118 "src/parser/parser.tab.cc"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 339 "src/parser/parser.y"


void yyerror(const char *s) {
    std::cerr << "Error at line " << yylineno << ": " << s << std::endl;
}
