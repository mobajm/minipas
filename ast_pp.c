# 1 "ast.c"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "ast.c"

# 1 "ast.h" 1





typedef struct prog_s prog_t;

typedef struct declseq_s declseq_t;
typedef struct constseq_s constseq_t;
typedef struct typeseq_s typeseq_t;
typedef struct varseq_s varseq_t;
typedef struct decl_s decl_t;

typedef struct statseq_s statseq_t;
typedef struct assign_s assign_t;
typedef struct implyseq_s implyseq_t;
typedef struct if_s if_t;
typedef struct while_s while_t;
typedef struct stat_s stat_t;

typedef struct exprseq_s exprseq_t;
typedef struct expr_s expr_t;
typedef struct simplexpr_s simplexpr_t;
typedef struct simplexprseq_s simplexprseq_t;
typedef struct term_s term_t;
typedef struct termseq_s termseq_t;
typedef struct factor_s factor_t;


typedef enum {PLUS, MINUS, OR, A_NONE} addop_t;
typedef enum {POS, NEG, S_NONE} sign_t;
typedef enum {TIMES, SLASH, DIV, MOD, AMP, M_NONE} multop_t;
typedef enum {EQ, NEQ, LT, LE, GT, GE, IS, R_NONE} rel_t;


typedef struct arrty_s arrty_t;
typedef struct pterty_s pterty_t;
typedef struct type_s type_t;

typedef struct number_s number_t;

typedef struct identseq_s identseq_t;
typedef char* ident_t;

typedef struct infos_s infos_t;


struct prog_s {
  declseq_t* decls;
  statseq_t* stats;
  ident_t ident;
};


struct declseq_s {
  decl_t* decl;
  declseq_t* next;
};

struct constseq_s {
  infos_t* infos;
  ident_t ident;
  expr_t* expr;
  constseq_t* next;
};

struct typeseq_s {
  infos_t* infos;
  ident_t ident;
  type_t* ty;
  typeseq_t* next;
};

struct varseq_s {
  infos_t* infos;
  identseq_t* idlist;
  type_t* ty;
  varseq_t* next;
};

struct decl_s {
  enum {CONST, TYPE, VAR} k;
  union {
    constseq_t* cseq;
    typeseq_t* tseq;
    varseq_t* vseq;
  } u;
  infos_t* infos;
};


struct statseq_s {
  infos_t* infos;
  stat_t* stat;
  statseq_t* next;
};

struct assign_s {
  infos_t* infos;
  ident_t desig;
  expr_t* expr;
};

struct implyseq_s {
  infos_t* infos;
  expr_t* cond;
  statseq_t* sseq;
  struct implyseq_s* next;
};

struct if_s {
  infos_t* infos;
  implyseq_t* branches;
  statseq_t* else_branch;
};

struct while_s {
  infos_t* infos;
  expr_t* cond;
  statseq_t* body;
};

struct stat_s {
  enum {ASSIGN, IF, WHILE, EXIT} k;
  union {
    assign_t* assign;
    if_t* if_n;
    while_t* while_n;
  } u;
  infos_t* infos;
};


struct exprseq_s {
  expr_t* e;
  exprseq_t* next;
};

struct expr_s {
  infos_t* infos;
  simplexpr_t* e1;
  rel_t op;
  simplexpr_t* e2;
};

struct simplexpr_s {
  infos_t* infos;
  sign_t sign;
  term_t* term;
  simplexprseq_t* rest;
};

struct simplexprseq_s {
  infos_t* infos;
  addop_t op;
  term_t* term;
  simplexprseq_t* next;
};

struct term_s {
  infos_t* infos;
  factor_t* fact;
  termseq_t* rest;
};

struct termseq_s {
  infos_t* infos;
  multop_t op;
  factor_t* fact;
  termseq_t* next;
};

struct factor_s {
  enum {NUM, NIL, CHAR, EXPR, IDENT, BOOL} k;
  union {
    number_t* num;
    char* c;
    expr_t* expr;
    ident_t id;
    int bool_val;
  } u;
  infos_t* infos;
};


typedef enum {INTEGER=0, REAL, BOOLEAN, CHARACTER, NB_SMPLTY} smplty_t;

struct arrty_s {
  type_t* ty;
  exprseq_t* exprseq;
};

struct pterty_s {
  type_t* ty;
};

struct type_s {
  enum {SMPL_TY, ARRAY_TY, POINTER_TY} k;
  union {
    smplty_t ty;
    arrty_t* arr;
    pterty_t* pter;
  } u;
};


struct number_s {
  enum {INT, DOUBLE} k;
  union {
    long i;
    double d;
  } u;
  infos_t* infos;
};




struct identseq_s {
  infos_t* infos;
  ident_t ident;
  identseq_t* next;
};

struct infos_s {
  int line;
  int col;
  type_t* ty;
};




prog_t* make_prog(ident_t, declseq_t*, statseq_t*);

declseq_t* make_declseq(decl_t*, declseq_t*);

decl_t* from_constseq(constseq_t*);
constseq_t* add_to_constseq(ident_t, expr_t*, constseq_t*);

decl_t* from_typeseq(typeseq_t*);
typeseq_t* add_to_typeseq(ident_t, type_t*, typeseq_t*);

decl_t* from_varseq(varseq_t*);
varseq_t* add_to_varseq(identseq_t*, type_t*, varseq_t*);

statseq_t* make_statseq(stat_t*, statseq_t*);
stat_t* from_assign(ident_t, expr_t*);
implyseq_t* make_implyseq(expr_t*, statseq_t*, implyseq_t*);
stat_t* from_if(implyseq_t*, statseq_t*);
stat_t* from_while(expr_t*, statseq_t*);
stat_t* from_exit(void);


exprseq_t* make_exprseq(expr_t*, exprseq_t*);
expr_t* make_expr(simplexpr_t*, rel_t, simplexpr_t*);
simplexpr_t* make_simplexpr(sign_t, term_t*, simplexprseq_t*);
simplexprseq_t* make_simplexprseq(addop_t, term_t*, simplexprseq_t*);
term_t* make_term(factor_t*, termseq_t*);
termseq_t* make_termseq(multop_t, factor_t*, termseq_t*);
factor_t* from_number(number_t*);
factor_t* from_character(char*);
factor_t* from_expr(expr_t*);
factor_t* from_nil(void);
factor_t* from_ident(ident_t);
factor_t* from_bool(int);

type_t* from_smplty(smplty_t);
type_t* from_arrty(type_t*, exprseq_t*);
type_t* from_pterty(type_t*);
const char* pptype(type_t*);

number_t* from_int(char*, int);
number_t* from_double(double);

identseq_t* make_identseq(ident_t, identseq_t*);
ident_t from_string(char*);
# 3 "ast.c" 2
# 1 "utils.h" 1



# 1 "/usr/include/stdlib.h" 1 3 4
# 24 "/usr/include/stdlib.h" 3 4
# 1 "/usr/include/features.h" 1 3 4
# 375 "/usr/include/features.h" 3 4
# 1 "/usr/include/sys/cdefs.h" 1 3 4
# 385 "/usr/include/sys/cdefs.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 386 "/usr/include/sys/cdefs.h" 2 3 4
# 376 "/usr/include/features.h" 2 3 4
# 399 "/usr/include/features.h" 3 4
# 1 "/usr/include/gnu/stubs.h" 1 3 4






# 1 "/usr/include/gnu/stubs-32.h" 1 3 4
# 8 "/usr/include/gnu/stubs.h" 2 3 4
# 400 "/usr/include/features.h" 2 3 4
# 25 "/usr/include/stdlib.h" 2 3 4







# 1 "/usr/lib/gcc/i686-pc-linux-gnu/4.8.0/include/stddef.h" 1 3 4
# 212 "/usr/lib/gcc/i686-pc-linux-gnu/4.8.0/include/stddef.h" 3 4
typedef unsigned int size_t;
# 324 "/usr/lib/gcc/i686-pc-linux-gnu/4.8.0/include/stddef.h" 3 4
typedef long int wchar_t;
# 33 "/usr/include/stdlib.h" 2 3 4








# 1 "/usr/include/bits/waitflags.h" 1 3 4
# 42 "/usr/include/stdlib.h" 2 3 4
# 1 "/usr/include/bits/waitstatus.h" 1 3 4
# 64 "/usr/include/bits/waitstatus.h" 3 4
# 1 "/usr/include/endian.h" 1 3 4
# 36 "/usr/include/endian.h" 3 4
# 1 "/usr/include/bits/endian.h" 1 3 4
# 37 "/usr/include/endian.h" 2 3 4
# 60 "/usr/include/endian.h" 3 4
# 1 "/usr/include/bits/byteswap.h" 1 3 4
# 27 "/usr/include/bits/byteswap.h" 3 4
# 1 "/usr/include/bits/types.h" 1 3 4
# 27 "/usr/include/bits/types.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 28 "/usr/include/bits/types.h" 2 3 4


typedef unsigned char __u_char;
typedef unsigned short int __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;


typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;




__extension__ typedef signed long long int __int64_t;
__extension__ typedef unsigned long long int __uint64_t;







__extension__ typedef long long int __quad_t;
__extension__ typedef unsigned long long int __u_quad_t;
# 130 "/usr/include/bits/types.h" 3 4
# 1 "/usr/include/bits/typesizes.h" 1 3 4
# 131 "/usr/include/bits/types.h" 2 3 4


__extension__ typedef __u_quad_t __dev_t;
__extension__ typedef unsigned int __uid_t;
__extension__ typedef unsigned int __gid_t;
__extension__ typedef unsigned long int __ino_t;
__extension__ typedef __u_quad_t __ino64_t;
__extension__ typedef unsigned int __mode_t;
__extension__ typedef unsigned int __nlink_t;
__extension__ typedef long int __off_t;
__extension__ typedef __quad_t __off64_t;
__extension__ typedef int __pid_t;
__extension__ typedef struct { int __val[2]; } __fsid_t;
__extension__ typedef long int __clock_t;
__extension__ typedef unsigned long int __rlim_t;
__extension__ typedef __u_quad_t __rlim64_t;
__extension__ typedef unsigned int __id_t;
__extension__ typedef long int __time_t;
__extension__ typedef unsigned int __useconds_t;
__extension__ typedef long int __suseconds_t;

__extension__ typedef int __daddr_t;
__extension__ typedef int __key_t;


__extension__ typedef int __clockid_t;


__extension__ typedef void * __timer_t;


__extension__ typedef long int __blksize_t;




__extension__ typedef long int __blkcnt_t;
__extension__ typedef __quad_t __blkcnt64_t;


__extension__ typedef unsigned long int __fsblkcnt_t;
__extension__ typedef __u_quad_t __fsblkcnt64_t;


__extension__ typedef unsigned long int __fsfilcnt_t;
__extension__ typedef __u_quad_t __fsfilcnt64_t;


__extension__ typedef int __fsword_t;

__extension__ typedef int __ssize_t;


__extension__ typedef long int __syscall_slong_t;

__extension__ typedef unsigned long int __syscall_ulong_t;



typedef __off64_t __loff_t;
typedef __quad_t *__qaddr_t;
typedef char *__caddr_t;


__extension__ typedef int __intptr_t;


__extension__ typedef unsigned int __socklen_t;
# 28 "/usr/include/bits/byteswap.h" 2 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 29 "/usr/include/bits/byteswap.h" 2 3 4






# 1 "/usr/include/bits/byteswap-16.h" 1 3 4
# 36 "/usr/include/bits/byteswap.h" 2 3 4
# 44 "/usr/include/bits/byteswap.h" 3 4
static __inline unsigned int
__bswap_32 (unsigned int __bsx)
{
  return __builtin_bswap32 (__bsx);
}
# 108 "/usr/include/bits/byteswap.h" 3 4
static __inline __uint64_t
__bswap_64 (__uint64_t __bsx)
{
  return __builtin_bswap64 (__bsx);
}
# 61 "/usr/include/endian.h" 2 3 4
# 65 "/usr/include/bits/waitstatus.h" 2 3 4

union wait
  {
    int w_status;
    struct
      {

 unsigned int __w_termsig:7;
 unsigned int __w_coredump:1;
 unsigned int __w_retcode:8;
 unsigned int:16;







      } __wait_terminated;
    struct
      {

 unsigned int __w_stopval:8;
 unsigned int __w_stopsig:8;
 unsigned int:16;






      } __wait_stopped;
  };
# 43 "/usr/include/stdlib.h" 2 3 4
# 67 "/usr/include/stdlib.h" 3 4
typedef union
  {
    union wait *__uptr;
    int *__iptr;
  } __WAIT_STATUS __attribute__ ((__transparent_union__));
# 95 "/usr/include/stdlib.h" 3 4


typedef struct
  {
    int quot;
    int rem;
  } div_t;



typedef struct
  {
    long int quot;
    long int rem;
  } ldiv_t;







__extension__ typedef struct
  {
    long long int quot;
    long long int rem;
  } lldiv_t;


# 139 "/usr/include/stdlib.h" 3 4
extern size_t __ctype_get_mb_cur_max (void) __attribute__ ((__nothrow__ , __leaf__)) ;




extern double atof (const char *__nptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) ;

extern int atoi (const char *__nptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) ;

extern long int atol (const char *__nptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) ;





__extension__ extern long long int atoll (const char *__nptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) ;





extern double strtod (const char *__restrict __nptr,
        char **__restrict __endptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));





extern float strtof (const char *__restrict __nptr,
       char **__restrict __endptr) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));

extern long double strtold (const char *__restrict __nptr,
       char **__restrict __endptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));





extern long int strtol (const char *__restrict __nptr,
   char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));

extern unsigned long int strtoul (const char *__restrict __nptr,
      char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));




__extension__
extern long long int strtoq (const char *__restrict __nptr,
        char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));

__extension__
extern unsigned long long int strtouq (const char *__restrict __nptr,
           char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));





__extension__
extern long long int strtoll (const char *__restrict __nptr,
         char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));

__extension__
extern unsigned long long int strtoull (const char *__restrict __nptr,
     char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));

# 314 "/usr/include/stdlib.h" 3 4
# 1 "/usr/include/sys/types.h" 1 3 4
# 27 "/usr/include/sys/types.h" 3 4






typedef __u_char u_char;
typedef __u_short u_short;
typedef __u_int u_int;
typedef __u_long u_long;
typedef __quad_t quad_t;
typedef __u_quad_t u_quad_t;
typedef __fsid_t fsid_t;




typedef __loff_t loff_t;



typedef __ino_t ino_t;
# 60 "/usr/include/sys/types.h" 3 4
typedef __dev_t dev_t;




typedef __gid_t gid_t;




typedef __mode_t mode_t;




typedef __nlink_t nlink_t;




typedef __uid_t uid_t;





typedef __off_t off_t;
# 98 "/usr/include/sys/types.h" 3 4
typedef __pid_t pid_t;





typedef __id_t id_t;




typedef __ssize_t ssize_t;





typedef __daddr_t daddr_t;
typedef __caddr_t caddr_t;
# 132 "/usr/include/sys/types.h" 3 4
# 1 "/usr/include/time.h" 1 3 4
# 57 "/usr/include/time.h" 3 4


typedef __clock_t clock_t;



# 73 "/usr/include/time.h" 3 4


typedef __time_t time_t;



# 91 "/usr/include/time.h" 3 4
typedef __clockid_t clockid_t;
# 103 "/usr/include/time.h" 3 4
typedef __timer_t timer_t;
# 133 "/usr/include/sys/types.h" 2 3 4
# 146 "/usr/include/sys/types.h" 3 4
# 1 "/usr/lib/gcc/i686-pc-linux-gnu/4.8.0/include/stddef.h" 1 3 4
# 147 "/usr/include/sys/types.h" 2 3 4



typedef unsigned long int ulong;
typedef unsigned short int ushort;
typedef unsigned int uint;
# 194 "/usr/include/sys/types.h" 3 4
typedef int int8_t __attribute__ ((__mode__ (__QI__)));
typedef int int16_t __attribute__ ((__mode__ (__HI__)));
typedef int int32_t __attribute__ ((__mode__ (__SI__)));
typedef int int64_t __attribute__ ((__mode__ (__DI__)));


typedef unsigned int u_int8_t __attribute__ ((__mode__ (__QI__)));
typedef unsigned int u_int16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int u_int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int u_int64_t __attribute__ ((__mode__ (__DI__)));

typedef int register_t __attribute__ ((__mode__ (__word__)));
# 219 "/usr/include/sys/types.h" 3 4
# 1 "/usr/include/sys/select.h" 1 3 4
# 30 "/usr/include/sys/select.h" 3 4
# 1 "/usr/include/bits/select.h" 1 3 4
# 22 "/usr/include/bits/select.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 23 "/usr/include/bits/select.h" 2 3 4
# 31 "/usr/include/sys/select.h" 2 3 4


# 1 "/usr/include/bits/sigset.h" 1 3 4
# 23 "/usr/include/bits/sigset.h" 3 4
typedef int __sig_atomic_t;




typedef struct
  {
    unsigned long int __val[(1024 / (8 * sizeof (unsigned long int)))];
  } __sigset_t;
# 34 "/usr/include/sys/select.h" 2 3 4



typedef __sigset_t sigset_t;





# 1 "/usr/include/time.h" 1 3 4
# 120 "/usr/include/time.h" 3 4
struct timespec
  {
    __time_t tv_sec;
    __syscall_slong_t tv_nsec;
  };
# 44 "/usr/include/sys/select.h" 2 3 4

# 1 "/usr/include/bits/time.h" 1 3 4
# 30 "/usr/include/bits/time.h" 3 4
struct timeval
  {
    __time_t tv_sec;
    __suseconds_t tv_usec;
  };
# 46 "/usr/include/sys/select.h" 2 3 4


typedef __suseconds_t suseconds_t;





typedef long int __fd_mask;
# 64 "/usr/include/sys/select.h" 3 4
typedef struct
  {






    __fd_mask __fds_bits[1024 / (8 * (int) sizeof (__fd_mask))];


  } fd_set;






typedef __fd_mask fd_mask;
# 96 "/usr/include/sys/select.h" 3 4

# 106 "/usr/include/sys/select.h" 3 4
extern int select (int __nfds, fd_set *__restrict __readfds,
     fd_set *__restrict __writefds,
     fd_set *__restrict __exceptfds,
     struct timeval *__restrict __timeout);
# 118 "/usr/include/sys/select.h" 3 4
extern int pselect (int __nfds, fd_set *__restrict __readfds,
      fd_set *__restrict __writefds,
      fd_set *__restrict __exceptfds,
      const struct timespec *__restrict __timeout,
      const __sigset_t *__restrict __sigmask);
# 131 "/usr/include/sys/select.h" 3 4

# 220 "/usr/include/sys/types.h" 2 3 4


# 1 "/usr/include/sys/sysmacros.h" 1 3 4
# 29 "/usr/include/sys/sysmacros.h" 3 4


__extension__
extern unsigned int gnu_dev_major (unsigned long long int __dev)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));
__extension__
extern unsigned int gnu_dev_minor (unsigned long long int __dev)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));
__extension__
extern unsigned long long int gnu_dev_makedev (unsigned int __major,
            unsigned int __minor)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));
# 63 "/usr/include/sys/sysmacros.h" 3 4

# 223 "/usr/include/sys/types.h" 2 3 4





typedef __blksize_t blksize_t;






typedef __blkcnt_t blkcnt_t;



typedef __fsblkcnt_t fsblkcnt_t;



typedef __fsfilcnt_t fsfilcnt_t;
# 270 "/usr/include/sys/types.h" 3 4
# 1 "/usr/include/bits/pthreadtypes.h" 1 3 4
# 21 "/usr/include/bits/pthreadtypes.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 22 "/usr/include/bits/pthreadtypes.h" 2 3 4
# 60 "/usr/include/bits/pthreadtypes.h" 3 4
typedef unsigned long int pthread_t;


union pthread_attr_t
{
  char __size[36];
  long int __align;
};

typedef union pthread_attr_t pthread_attr_t;
# 81 "/usr/include/bits/pthreadtypes.h" 3 4
typedef struct __pthread_internal_slist
{
  struct __pthread_internal_slist *__next;
} __pthread_slist_t;





typedef union
{
  struct __pthread_mutex_s
  {
    int __lock;
    unsigned int __count;
    int __owner;





    int __kind;





    unsigned int __nusers;
    __extension__ union
    {
      int __spins;
      __pthread_slist_t __list;
    };

  } __data;
  char __size[24];
  long int __align;
} pthread_mutex_t;

typedef union
{
  char __size[4];
  int __align;
} pthread_mutexattr_t;




typedef union
{
  struct
  {
    int __lock;
    unsigned int __futex;
    __extension__ unsigned long long int __total_seq;
    __extension__ unsigned long long int __wakeup_seq;
    __extension__ unsigned long long int __woken_seq;
    void *__mutex;
    unsigned int __nwaiters;
    unsigned int __broadcast_seq;
  } __data;
  char __size[48];
  __extension__ long long int __align;
} pthread_cond_t;

typedef union
{
  char __size[4];
  int __align;
} pthread_condattr_t;



typedef unsigned int pthread_key_t;



typedef int pthread_once_t;





typedef union
{
# 185 "/usr/include/bits/pthreadtypes.h" 3 4
  struct
  {
    int __lock;
    unsigned int __nr_readers;
    unsigned int __readers_wakeup;
    unsigned int __writer_wakeup;
    unsigned int __nr_readers_queued;
    unsigned int __nr_writers_queued;


    unsigned char __flags;
    unsigned char __shared;
    unsigned char __pad1;
    unsigned char __pad2;
    int __writer;
  } __data;

  char __size[32];
  long int __align;
} pthread_rwlock_t;

typedef union
{
  char __size[8];
  long int __align;
} pthread_rwlockattr_t;





typedef volatile int pthread_spinlock_t;




typedef union
{
  char __size[20];
  long int __align;
} pthread_barrier_t;

typedef union
{
  char __size[4];
  int __align;
} pthread_barrierattr_t;
# 271 "/usr/include/sys/types.h" 2 3 4



# 315 "/usr/include/stdlib.h" 2 3 4






extern long int random (void) __attribute__ ((__nothrow__ , __leaf__));


extern void srandom (unsigned int __seed) __attribute__ ((__nothrow__ , __leaf__));





extern char *initstate (unsigned int __seed, char *__statebuf,
   size_t __statelen) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));



extern char *setstate (char *__statebuf) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));







struct random_data
  {
    int32_t *fptr;
    int32_t *rptr;
    int32_t *state;
    int rand_type;
    int rand_deg;
    int rand_sep;
    int32_t *end_ptr;
  };

extern int random_r (struct random_data *__restrict __buf,
       int32_t *__restrict __result) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));

extern int srandom_r (unsigned int __seed, struct random_data *__buf)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));

extern int initstate_r (unsigned int __seed, char *__restrict __statebuf,
   size_t __statelen,
   struct random_data *__restrict __buf)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2, 4)));

extern int setstate_r (char *__restrict __statebuf,
         struct random_data *__restrict __buf)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));






extern int rand (void) __attribute__ ((__nothrow__ , __leaf__));

extern void srand (unsigned int __seed) __attribute__ ((__nothrow__ , __leaf__));




extern int rand_r (unsigned int *__seed) __attribute__ ((__nothrow__ , __leaf__));
# 463 "/usr/include/stdlib.h" 3 4


extern void *malloc (size_t __size) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) ;

extern void *calloc (size_t __nmemb, size_t __size)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) ;










extern void *realloc (void *__ptr, size_t __size)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__warn_unused_result__));

extern void free (void *__ptr) __attribute__ ((__nothrow__ , __leaf__));




extern void cfree (void *__ptr) __attribute__ ((__nothrow__ , __leaf__));



# 1 "/usr/include/alloca.h" 1 3 4
# 24 "/usr/include/alloca.h" 3 4
# 1 "/usr/lib/gcc/i686-pc-linux-gnu/4.8.0/include/stddef.h" 1 3 4
# 25 "/usr/include/alloca.h" 2 3 4







extern void *alloca (size_t __size) __attribute__ ((__nothrow__ , __leaf__));






# 492 "/usr/include/stdlib.h" 2 3 4





extern void *valloc (size_t __size) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) ;




extern int posix_memalign (void **__memptr, size_t __alignment, size_t __size)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;
# 512 "/usr/include/stdlib.h" 3 4


extern void abort (void) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__noreturn__));



extern int atexit (void (*__func) (void)) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));
# 529 "/usr/include/stdlib.h" 3 4





extern int on_exit (void (*__func) (int __status, void *__arg), void *__arg)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));






extern void exit (int __status) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__noreturn__));













extern void _Exit (int __status) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__noreturn__));






extern char *getenv (const char *__name) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;

# 583 "/usr/include/stdlib.h" 3 4
extern int setenv (const char *__name, const char *__value, int __replace)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));


extern int unsetenv (const char *__name) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));






extern int clearenv (void) __attribute__ ((__nothrow__ , __leaf__));
# 605 "/usr/include/stdlib.h" 3 4
extern char *mktemp (char *__template) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));
# 619 "/usr/include/stdlib.h" 3 4
extern int mkstemp (char *__template) __attribute__ ((__nonnull__ (1))) ;
# 641 "/usr/include/stdlib.h" 3 4
extern int mkstemps (char *__template, int __suffixlen) __attribute__ ((__nonnull__ (1))) ;
# 662 "/usr/include/stdlib.h" 3 4
extern char *mkdtemp (char *__template) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;
# 711 "/usr/include/stdlib.h" 3 4





extern int system (const char *__command) ;

# 733 "/usr/include/stdlib.h" 3 4
extern char *realpath (const char *__restrict __name,
         char *__restrict __resolved) __attribute__ ((__nothrow__ , __leaf__)) ;






typedef int (*__compar_fn_t) (const void *, const void *);
# 751 "/usr/include/stdlib.h" 3 4



extern void *bsearch (const void *__key, const void *__base,
        size_t __nmemb, size_t __size, __compar_fn_t __compar)
     __attribute__ ((__nonnull__ (1, 2, 5))) ;



extern void qsort (void *__base, size_t __nmemb, size_t __size,
     __compar_fn_t __compar) __attribute__ ((__nonnull__ (1, 4)));
# 770 "/usr/include/stdlib.h" 3 4
extern int abs (int __x) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;
extern long int labs (long int __x) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;



__extension__ extern long long int llabs (long long int __x)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;







extern div_t div (int __numer, int __denom)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;
extern ldiv_t ldiv (long int __numer, long int __denom)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;




__extension__ extern lldiv_t lldiv (long long int __numer,
        long long int __denom)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;

# 856 "/usr/include/stdlib.h" 3 4



extern int mblen (const char *__s, size_t __n) __attribute__ ((__nothrow__ , __leaf__)) ;


extern int mbtowc (wchar_t *__restrict __pwc,
     const char *__restrict __s, size_t __n) __attribute__ ((__nothrow__ , __leaf__)) ;


extern int wctomb (char *__s, wchar_t __wchar) __attribute__ ((__nothrow__ , __leaf__)) ;



extern size_t mbstowcs (wchar_t *__restrict __pwcs,
   const char *__restrict __s, size_t __n) __attribute__ ((__nothrow__ , __leaf__));

extern size_t wcstombs (char *__restrict __s,
   const wchar_t *__restrict __pwcs, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__));

# 895 "/usr/include/stdlib.h" 3 4
extern int getsubopt (char **__restrict __optionp,
        char *const *__restrict __tokens,
        char **__restrict __valuep)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2, 3))) ;
# 947 "/usr/include/stdlib.h" 3 4
extern int getloadavg (double __loadavg[], int __nelem)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));


# 1 "/usr/include/bits/stdlib-float.h" 1 3 4
# 952 "/usr/include/stdlib.h" 2 3 4
# 964 "/usr/include/stdlib.h" 3 4

# 5 "utils.h" 2

void* checked_malloc(size_t);
# 4 "ast.c" 2

# 1 "/usr/include/string.h" 1 3 4
# 27 "/usr/include/string.h" 3 4





# 1 "/usr/lib/gcc/i686-pc-linux-gnu/4.8.0/include/stddef.h" 1 3 4
# 33 "/usr/include/string.h" 2 3 4









extern void *memcpy (void *__restrict __dest, const void *__restrict __src,
       size_t __n) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));


extern void *memmove (void *__dest, const void *__src, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));






extern void *memccpy (void *__restrict __dest, const void *__restrict __src,
        int __c, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));





extern void *memset (void *__s, int __c, size_t __n) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));


extern int memcmp (const void *__s1, const void *__s2, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));
# 92 "/usr/include/string.h" 3 4
extern void *memchr (const void *__s, int __c, size_t __n)
      __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));


# 123 "/usr/include/string.h" 3 4


extern char *strcpy (char *__restrict __dest, const char *__restrict __src)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));

extern char *strncpy (char *__restrict __dest,
        const char *__restrict __src, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));


extern char *strcat (char *__restrict __dest, const char *__restrict __src)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));

extern char *strncat (char *__restrict __dest, const char *__restrict __src,
        size_t __n) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));


extern int strcmp (const char *__s1, const char *__s2)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));

extern int strncmp (const char *__s1, const char *__s2, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));


extern int strcoll (const char *__s1, const char *__s2)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));

extern size_t strxfrm (char *__restrict __dest,
         const char *__restrict __src, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));






# 1 "/usr/include/xlocale.h" 1 3 4
# 27 "/usr/include/xlocale.h" 3 4
typedef struct __locale_struct
{

  struct __locale_data *__locales[13];


  const unsigned short int *__ctype_b;
  const int *__ctype_tolower;
  const int *__ctype_toupper;


  const char *__names[13];
} *__locale_t;


typedef __locale_t locale_t;
# 160 "/usr/include/string.h" 2 3 4


extern int strcoll_l (const char *__s1, const char *__s2, __locale_t __l)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2, 3)));

extern size_t strxfrm_l (char *__dest, const char *__src, size_t __n,
    __locale_t __l) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2, 4)));





extern char *strdup (const char *__s)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) __attribute__ ((__nonnull__ (1)));






extern char *strndup (const char *__string, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) __attribute__ ((__nonnull__ (1)));
# 207 "/usr/include/string.h" 3 4

# 232 "/usr/include/string.h" 3 4
extern char *strchr (const char *__s, int __c)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));
# 259 "/usr/include/string.h" 3 4
extern char *strrchr (const char *__s, int __c)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));


# 278 "/usr/include/string.h" 3 4



extern size_t strcspn (const char *__s, const char *__reject)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));


extern size_t strspn (const char *__s, const char *__accept)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));
# 311 "/usr/include/string.h" 3 4
extern char *strpbrk (const char *__s, const char *__accept)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));
# 338 "/usr/include/string.h" 3 4
extern char *strstr (const char *__haystack, const char *__needle)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));




extern char *strtok (char *__restrict __s, const char *__restrict __delim)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));




extern char *__strtok_r (char *__restrict __s,
    const char *__restrict __delim,
    char **__restrict __save_ptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2, 3)));

extern char *strtok_r (char *__restrict __s, const char *__restrict __delim,
         char **__restrict __save_ptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2, 3)));
# 393 "/usr/include/string.h" 3 4


extern size_t strlen (const char *__s)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));





extern size_t strnlen (const char *__string, size_t __maxlen)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));





extern char *strerror (int __errnum) __attribute__ ((__nothrow__ , __leaf__));

# 423 "/usr/include/string.h" 3 4
extern int strerror_r (int __errnum, char *__buf, size_t __buflen) __asm__ ("" "__xpg_strerror_r") __attribute__ ((__nothrow__ , __leaf__))

                        __attribute__ ((__nonnull__ (2)));
# 441 "/usr/include/string.h" 3 4
extern char *strerror_l (int __errnum, __locale_t __l) __attribute__ ((__nothrow__ , __leaf__));





extern void __bzero (void *__s, size_t __n) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));



extern void bcopy (const void *__src, void *__dest, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));


extern void bzero (void *__s, size_t __n) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));


extern int bcmp (const void *__s1, const void *__s2, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));
# 485 "/usr/include/string.h" 3 4
extern char *index (const char *__s, int __c)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));
# 513 "/usr/include/string.h" 3 4
extern char *rindex (const char *__s, int __c)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));




extern int ffs (int __i) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));
# 532 "/usr/include/string.h" 3 4
extern int strcasecmp (const char *__s1, const char *__s2)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));


extern int strncasecmp (const char *__s1, const char *__s2, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));
# 555 "/usr/include/string.h" 3 4
extern char *strsep (char **__restrict __stringp,
       const char *__restrict __delim)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));




extern char *strsignal (int __sig) __attribute__ ((__nothrow__ , __leaf__));


extern char *__stpcpy (char *__restrict __dest, const char *__restrict __src)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));
extern char *stpcpy (char *__restrict __dest, const char *__restrict __src)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));



extern char *__stpncpy (char *__restrict __dest,
   const char *__restrict __src, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));
extern char *stpncpy (char *__restrict __dest,
        const char *__restrict __src, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));
# 642 "/usr/include/string.h" 3 4

# 6 "ast.c" 2


extern int line, col;
# 25 "ast.c"
prog_t* make_prog(ident_t name, declseq_t* dseq, statseq_t* sseq)
{
  prog_t* p = checked_malloc(sizeof *p);
  p->decls = dseq;
  p->stats = sseq;
  p->ident = name;
  return p;
}


declseq_t* make_declseq(decl_t* d, declseq_t* ds)
{
  declseq_t* dseq = ds;
  declseq_t* dcl = checked_malloc(sizeof *dseq);

  dcl->decl = d;
  dcl->next = ((void *)0);

  do { if ((ds) == ((void *)0)) return (dcl); else { while ((ds)->next != ((void *)0)) (ds) = (ds)->next; (ds)->next = (dcl); return (dseq); } } while (0);
}

decl_t* from_constseq(constseq_t* cs)
{
  decl_t* decl = checked_malloc(sizeof *decl);

  decl->k = CONST;
  decl->u.cseq = cs;

  if (cs)
    ((decl))->infos = checked_malloc(sizeof *(((decl))->infos)); ((decl))->infos->line = (((cs)->infos->line)); ((decl))->infos->col = (((cs)->infos->col));;

  return decl;
}

constseq_t* add_to_constseq(ident_t id, expr_t* e, constseq_t* cs)
{
  constseq_t* cs_start = cs;
  constseq_t* new = checked_malloc(sizeof *new);

  new->ident = id;
  new->expr = e;
  new->next = ((void *)0);

  if (e && cs)
((cs))->infos = checked_malloc(sizeof *(((cs))->infos)); ((cs))->infos->line = (((e)->infos->line)); ((cs))->infos->col = (((e)->infos->col));;

  do { if ((cs) == ((void *)0)) return (new); else { while ((cs)->next != ((void *)0)) (cs) = (cs)->next; (cs)->next = (new); return (cs_start); } } while (0);
}

decl_t* from_typeseq(typeseq_t* ts)
{
  decl_t* decl = checked_malloc(sizeof *decl);

  decl->k = TYPE;
  decl->u.tseq = ts;

  if (ts)
    ((decl))->infos = checked_malloc(sizeof *(((decl))->infos)); ((decl))->infos->line = (((ts)->infos->line)); ((decl))->infos->col = (((ts)->infos->col));;

  return decl;
}

typeseq_t* add_to_typeseq(ident_t id, type_t* t, typeseq_t* ts)
{
  typeseq_t* ts_start = ts;
  typeseq_t* new = checked_malloc(sizeof *new);

  new->ident = id;
  new->ty = t;
  new->next = ((void *)0);

  ((ts))->infos = checked_malloc(sizeof *(((ts))->infos)); ((ts))->infos->line = ((line)); ((ts))->infos->col = ((col));;

  do { if ((ts) == ((void *)0)) return (new); else { while ((ts)->next != ((void *)0)) (ts) = (ts)->next; (ts)->next = (new); return (ts_start); } } while (0);
}

decl_t* from_varseq(varseq_t* vs)
{
  decl_t* decl = checked_malloc(sizeof *decl);

  decl->k = VAR;
  decl->u.vseq = vs;

  if (vs)
    ((decl))->infos = checked_malloc(sizeof *(((decl))->infos)); ((decl))->infos->line = (((vs)->infos->line)); ((decl))->infos->col = (((vs)->infos->col));;

  return decl;
}

varseq_t* add_to_varseq(identseq_t* idseq, type_t* t, varseq_t* vs)
{
  varseq_t* new = checked_malloc(sizeof *new);
  varseq_t* vs_start = vs;

  new->idlist = idseq;
  new->ty = t;
  new->next = ((void *)0);

  if (idseq && vs)
((vs))->infos = checked_malloc(sizeof *(((vs))->infos)); ((vs))->infos->line = (((idseq)->infos->line)); ((vs))->infos->col = (((idseq)->infos->col));;

  do { if ((vs) == ((void *)0)) return (new); else { while ((vs)->next != ((void *)0)) (vs) = (vs)->next; (vs)->next = (new); return (vs_start); } } while (0);
}


statseq_t* make_statseq(stat_t* s, statseq_t* ss)
{
  statseq_t* sseq = checked_malloc(sizeof *sseq);
  statseq_t* ss_start = ss;

  sseq->stat = s;
  sseq->next = ((void *)0);

  do { if ((ss) == ((void *)0)) return (sseq); else { while ((ss)->next != ((void *)0)) (ss) = (ss)->next; (ss)->next = (sseq); return (ss_start); } } while (0);
}

stat_t* from_assign(ident_t id, expr_t* e)
{
  stat_t* s = checked_malloc(sizeof *s);
  s->k = ASSIGN;

  s->u.assign = checked_malloc(sizeof *(s->u.assign));
  s->u.assign->desig = id;
  s->u.assign->expr = e;

  if (e)
    ((s))->infos = checked_malloc(sizeof *(((s))->infos)); ((s))->infos->line = (((e)->infos->line)); ((s))->infos->col = (((e)->infos->col));;

  return s;
}

implyseq_t* make_implyseq(expr_t* e, statseq_t* sseq, implyseq_t* ims)
{
  implyseq_t* new = checked_malloc(sizeof *new);
  implyseq_t* ims_start = ims;

  new->cond = e;
  new->sseq = sseq;
  new->next = ((void *)0);

  if (e && ims)
((ims))->infos = checked_malloc(sizeof *(((ims))->infos)); ((ims))->infos->line = (((e)->infos->line)); ((ims))->infos->col = (((e)->infos->col));;

  do { if ((ims) == ((void *)0)) return (new); else { while ((ims)->next != ((void *)0)) (ims) = (ims)->next; (ims)->next = (new); return (ims_start); } } while (0);
}

stat_t* from_if(implyseq_t* brches, statseq_t* el)
{
  stat_t* s = checked_malloc(sizeof *s);
  s->k = IF;

  s->u.if_n = checked_malloc(sizeof *(s->u.if_n));
  s->u.if_n->branches = brches;
  s->u.if_n->else_branch = el;

  if (brches->cond)
    ((s))->infos = checked_malloc(sizeof *(((s))->infos)); ((s))->infos->line = (((brches->cond)->infos->line)); ((s))->infos->col = (((brches->cond)->infos->col));;

  return s;
}

stat_t* from_while(expr_t* pred, statseq_t* b)
{
  stat_t* s = checked_malloc(sizeof *s);
  s->k = WHILE;

  s->u.while_n = checked_malloc(sizeof *(s->u.while_n));
  s->u.while_n->cond = pred;
  s->u.while_n->body = b;

  if (pred)
    ((s))->infos = checked_malloc(sizeof *(((s))->infos)); ((s))->infos->line = (((pred)->infos->line)); ((s))->infos->col = (((pred)->infos->col));;

  return s;
}

stat_t* from_exit(void)
{
  stat_t* s = checked_malloc(sizeof *s);
  s->k = EXIT;
  return s;
}
# 217 "ast.c"
exprseq_t* make_exprseq(expr_t* e, exprseq_t* es)
{
  exprseq_t* eseq = checked_malloc(sizeof *eseq);
  exprseq_t* es_start = es;

  eseq->e = e;
  eseq->next = ((void *)0);

  do { if ((es) == ((void *)0)) return (eseq); else { while ((es)->next != ((void *)0)) (es) = (es)->next; (es)->next = (eseq); return (es_start); } } while (0);
}

expr_t* make_expr(simplexpr_t* e1, rel_t op, simplexpr_t* e2)
{
  expr_t* e = checked_malloc(sizeof *e);
  e->e1 = e1;
  e->op = op;
  e->e2 = e2;

  if (e1)
    ((e))->infos = checked_malloc(sizeof *(((e))->infos)); ((e))->infos->line = (((e1)->infos->line)); ((e))->infos->col = (((e1)->infos->col));;
  return e;
}

simplexpr_t* make_simplexpr(sign_t s, term_t* t, simplexprseq_t* sexpseq)
{
  simplexpr_t* se = checked_malloc(sizeof *se);
  se->sign = s;
  se->term = t;
  se->rest = sexpseq;

  if (t)
    ((se))->infos = checked_malloc(sizeof *(((se))->infos)); ((se))->infos->line = (((t)->infos->line)); ((se))->infos->col = (((t)->infos->col));;
  return se;
}

simplexprseq_t* make_simplexprseq(addop_t op, term_t* t, simplexprseq_t* sexps)
{
  simplexprseq_t* ses = checked_malloc(sizeof *ses);
  simplexprseq_t* ses_start = sexps;

  ses->op = op;
  ses->term = t;
  ses->next = ((void *)0);

  if (t)
    ((ses))->infos = checked_malloc(sizeof *(((ses))->infos)); ((ses))->infos->line = (((t)->infos->line)); ((ses))->infos->col = (((t)->infos->col));;
  do { if ((sexps) == ((void *)0)) return (ses); else { while ((sexps)->next != ((void *)0)) (sexps) = (sexps)->next; (sexps)->next = (ses); return (ses_start); } } while (0);
}

term_t* make_term(factor_t* f, termseq_t* ts)
{
  term_t* t = checked_malloc(sizeof *t);
  t->fact = f;
  t->rest = ts;

  if (f)
    ((t))->infos = checked_malloc(sizeof *(((t))->infos)); ((t))->infos->line = (((f)->infos->line)); ((t))->infos->col = (((f)->infos->col));;
  return t;
}

termseq_t* make_termseq(multop_t op, factor_t* f, termseq_t* ts)
{
  termseq_t* tseq = ts;
  termseq_t* t = checked_malloc(sizeof *t);

  t->op = op;
  t->fact = f;
  t->next = ((void *)0);

  if (f)
    ((t))->infos = checked_malloc(sizeof *(((t))->infos)); ((t))->infos->line = (((f)->infos->line)); ((t))->infos->col = (((f)->infos->col));;
  do { if ((ts) == ((void *)0)) return (t); else { while ((ts)->next != ((void *)0)) (ts) = (ts)->next; (ts)->next = (t); return (tseq); } } while (0);
}

factor_t* from_number(number_t* n)
{
  factor_t* f = checked_malloc(sizeof *f);
  f->k = NUM;
  f->u.num = n;

  ((f))->infos = checked_malloc(sizeof *(((f))->infos)); ((f))->infos->line = ((line)); ((f))->infos->col = ((col));;
  return f;
}

factor_t* from_character(char* c)
{
  factor_t* f = checked_malloc(sizeof *f);
  f->k = CHAR;
  f->u.c = c;
  ((f))->infos = checked_malloc(sizeof *(((f))->infos)); ((f))->infos->line = ((line)); ((f))->infos->col = ((col));;
  return f;
}

factor_t* from_expr(expr_t* e)
{
  factor_t* f = checked_malloc(sizeof *f);
  f->k = EXPR;
  f->u.expr = e;

  ((f))->infos = checked_malloc(sizeof *(((f))->infos)); ((f))->infos->line = ((line)); ((f))->infos->col = ((col));;
  return f;
}

factor_t* from_nil(void)
{
  factor_t* f = checked_malloc(sizeof *f);
  f->k = NIL;

  ((f))->infos = checked_malloc(sizeof *(((f))->infos)); ((f))->infos->line = ((line)); ((f))->infos->col = ((col));;
  return f;
}

factor_t* from_ident(ident_t id)
{
  factor_t* f = checked_malloc(sizeof *f);
  f->k = IDENT;
  f->u.id = id;

  ((f))->infos = checked_malloc(sizeof *(((f))->infos)); ((f))->infos->line = ((line)); ((f))->infos->col = ((col));;
  return f;
}

factor_t* from_bool(int b)
{
  factor_t* f = checked_malloc(sizeof *f);
  f->k = BOOL;
  f->u.bool_val = b;

  ((f))->infos = checked_malloc(sizeof *(((f))->infos)); ((f))->infos->line = ((line)); ((f))->infos->col = ((col));;
  return f;
}


type_t* from_smplty(smplty_t ty)
{
  type_t* t = checked_malloc(sizeof *t);
  t->k = SMPL_TY;
  t->u.ty = ty;
  return t;
}

type_t* from_arrty(type_t* ty, exprseq_t* es)
{
  type_t* t = checked_malloc(sizeof *t);
  t->k = ARRAY_TY;

  t->u.arr = checked_malloc(sizeof *(t->u.arr));
  t->u.arr->ty = ty;
  t->u.arr->exprseq = es;
  return t;
}

type_t* from_pterty(type_t* ty)
{
  type_t* t = checked_malloc(sizeof *t);
  t->k = POINTER_TY;

  t->u.pter = checked_malloc(sizeof *(t->u.pter));
  t->u.pter->ty = ty;
  return t;
}

const char* pptype(type_t* ty)
{
  char smpltypes_str[NB_SMPLTY][15] = {
    "INTEGER", "REAL", "BOOLEAN", "CHARACTER"
  };

  if (ty->k == SMPL_TY)
    return strdup(smpltypes_str[ty->u.ty]);

  return "";
}


number_t* from_int(char* i, int base)
{
  number_t* n = checked_malloc(sizeof *n);
  n->k = INT;
  n->u.i = strtol(i, ((void *)0), base);

  ((n))->infos = checked_malloc(sizeof *(((n))->infos)); ((n))->infos->line = ((line)); ((n))->infos->col = ((col));;
  return n;
}

number_t* from_double(double d)
{
  number_t* n = checked_malloc(sizeof *n);
  n->k = DOUBLE;
  n->u.d = d;
  ((n))->infos = checked_malloc(sizeof *(((n))->infos)); ((n))->infos->line = ((line)); ((n))->infos->col = ((col));;
  return n;
}


identseq_t* make_identseq(ident_t id, identseq_t* idseq)
{
  identseq_t* is = checked_malloc(sizeof *is);
  identseq_t* idseq_start = idseq;

  is->ident = id;
  is->next = ((void *)0);

  ((is))->infos = checked_malloc(sizeof *(((is))->infos)); ((is))->infos->line = ((line)); ((is))->infos->col = ((col));;
  do { if ((idseq) == ((void *)0)) return (is); else { while ((idseq)->next != ((void *)0)) (idseq) = (idseq)->next; (idseq)->next = (is); return (idseq_start); } } while (0);
}

ident_t from_string(char* s)
{
  return (ident_t) s;
}
