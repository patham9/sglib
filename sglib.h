/*

  This is SGLIB version 1.0.4

  (C) by Marian Vittek, Bratislava, http://www.xref-tech.com/sglib, 2003-5

  License Conditions: You can use a verbatim copy (including this
  copyright notice) of sglib freely in any project, commercial or not.
  You can also use derivative forms freely under terms of Open Source
  Software license or under terms of GNU Public License.  If you need
  to use a derivative form in a commercial project, or you need sglib
  under any other license conditions, contact the author.



*/


#ifndef _SGLIB__h_
#define _SGLIB__h_

/* the assert is used exclusively to write unexpected error messages */
#include <assert.h>

/* ---------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------- */
/* -                            LEVEL - 0  INTERFACE                          - */
/* ---------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------- */
/* ------------------------------ STATIC ARRAYS ------------------------------- */
/* ---------------------------------------------------------------------------- */

/*

  Basic algorithms  for sorting arrays. Multiple  depending arrays can
  be rearranged using user defined 'elem_exchangers'

*/

/*               HEAP - SORT  (level 0)           */

#define SGLIB_ARRAY_SINGLE_HEAP_SORT(type, a, max, comparator) {\
  SGLIB_ARRAY_HEAP_SORT(type, a, max, comparator, SGLIB_ARRAY_ELEMENTS_EXCHANGER);\
}

#define SGLIB_ARRAY_HEAP_SORT(type, a, max, comparator, elem_exchanger) {\
  int   _k_;\
  for(_k_=(max)/2; _k_>=0; _k_--) {\
    SGLIB___ARRAY_HEAP_DOWN(type, a, _k_, max, comparator, elem_exchanger);\
  }\
  for(_k_=(max)-1; _k_>=0; _k_--) {\
    elem_exchanger(type, a, 0, _k_);\
    SGLIB___ARRAY_HEAP_DOWN(type, a, 0, _k_, comparator, elem_exchanger);\
  }\
}

#define SGLIB___ARRAY_HEAP_DOWN(type, a, ind, max, comparator, elem_exchanger) {\
  int   _m_, _l_, _r_, _i_;\
  _i_ = (ind);\
  _m_ = _i_;\
  do {\
    _i_ = _m_;\
    _l_ = 2*_i_+1;\
    _r_ = _l_+1;\
    if (_l_ < (max)){\
      if (comparator(((a)[_m_]), ((a)[_l_])) < 0) _m_ = _l_;\
      if (_r_ < (max)) {\
        if (comparator(((a)[_m_]), ((a)[_r_])) < 0) _m_ = _r_;\
      }\
    }\
    if (_m_ != _i_) {\
     elem_exchanger(type, a, _i_, _m_);\
    }\
  } while (_m_ != _i_);\
}


/*             QUICK - SORT   (level 0)          */

#define SGLIB_ARRAY_SINGLE_QUICK_SORT(type, a, max, comparator) {\
  SGLIB_ARRAY_QUICK_SORT(type, a, max, comparator, SGLIB_ARRAY_ELEMENTS_EXCHANGER);\
}

#define SGLIB_ARRAY_QUICK_SORT(type, a, max, comparator, elem_exchanger) {\
  int   _i_, _j_, _p_, _stacki_, _start_, _end_;\
  /* can sort up to 2^64 elements */\
  int   _startStack_[64];\
  int   _endStack_[64];\
  _startStack_[0] = 0;\
  _endStack_[0] = (max);\
  _stacki_ = 1;\
  while (_stacki_ > 0) {\
    _stacki_ --;\
    _start_ = _startStack_[_stacki_];\
    _end_ = _endStack_[_stacki_];\
    while (_end_ - _start_ > 2) {\
      _p_ = _start_;\
      _i_ = _start_ + 1;\
      _j_ = _end_ - 1;\
      while (_i_<_j_) {\
        for(; _i_<=_j_ && comparator(((a)[_i_]),((a)[_p_]))<=0; _i_++) ;\
        if (_i_ > _j_) {\
          /* all remaining elements lesseq than pivot */\
          elem_exchanger(type, a, _j_, _p_);\
          _i_ = _j_;\
        } else {\
          for(; _i_<=_j_ && comparator(((a)[_j_]),((a)[_p_]))>=0; _j_--) ;\
          if (_i_ > _j_) {\
            /* all remaining elements greater than pivot */\
            elem_exchanger(type, a, _j_, _p_);\
            _i_ = _j_;\
          } else if (_i_ < _j_) {\
            elem_exchanger(type, a, _i_, _j_);\
            if (_i_+2 < _j_) {_i_++; _j_--;}\
            else if (_i_+1 < _j_) _i_++;\
          }\
        }\
      }\
      /* O.K. i==j and pivot is on a[i] == a[j] */\
      /* handle recursive calls without recursion */\
      if (_i_-_start_ > 1 && _end_-_j_ > 1) {\
        /* two recursive calls, use array-stack */\
        if (_i_-_start_ < _end_-_j_-1) {\
          _startStack_[_stacki_] = _j_+1;\
          _endStack_[_stacki_] = _end_;\
          _stacki_ ++;\
          _end_ = _i_;\
        } else {\
          _startStack_[_stacki_] = _start_;\
          _endStack_[_stacki_] = _i_;\
          _stacki_ ++;\
          _start_ = _j_+1;\
        }\
      } else {\
        if (_i_-_start_ > 1) {\
          _end_ = _i_;\
        } else {\
          _start_ = _j_+1;\
        }\
      }\
    }\
    if (_end_ - _start_ == 2) {\
      if (comparator(((a)[_start_]),((a)[_end_-1])) > 0) {\
        elem_exchanger(type, a, _start_, _end_-1);\
      }\
    }\
  }\
}

/*             BINARY SEARCH (level 0)          */

#define SGLIB_ARRAY_BINARY_SEARCH(type, a, start_index, end_index, key, comparator, found, result_index) {\
  int _kk_, _cc_, _ii_, _jj_, _ff_;\
  _ii_ = (start_index);\
  _jj_ = (end_index);\
  _ff_ = 0;\
  while (_ii_ <= _jj_ && _ff_==0) {\
    _kk_ = (_jj_+_ii_)/2;\
    _cc_ = comparator(((a)[_kk_]), (key));\
    if (_cc_ == 0) {\
      (result_index) = _kk_;\
      _ff_ = 1;\
    } else if (_cc_ < 0) {\
      _ii_ = _kk_+1;\
    } else {\
      _jj_ = _kk_-1;\
    }\
  }\
  if (_ff_ == 0) {\
    /* not found, but set its resulting place in the array */\
    (result_index) = _jj_+1;\
  }\
  (found) = _ff_;\
}

/* -------------------------------- queue (in an array) ------------------ */
/* queue is a quadruple (a,i,j,dim) such that:                             */
/*  a is the array storing values                                          */
/*  i is the index of the first used element in the array                  */
/*  j is the index of the first free element in the array                  */
/*  dim is the size of the array a                                         */
/* !!!!!!! This data structure is NOT documented, do not use it !!!!!!!!!! */

#define SGLIB_QUEUE_INIT(type, a, i, j) { i = j = 0; }
#define SGLIB_QUEUE_IS_EMPTY(type, a, i, j) ((i)==(j))
#define SGLIB_QUEUE_IS_FULL(type, a, i, j, dim) ((i)==((j)+1)%(dim))
#define SGLIB_QUEUE_FIRST_ELEMENT(type, a, i, j) (a[i])
#define SGLIB_QUEUE_ADD_NEXT(type, a, i, j, dim) {\
  if (SGLIB_QUEUE_IS_FULL(type, a, i, j, dim)) assert(0 && "the queue is full");\
  (j) = ((j)+1) % (dim);\
}
#define SGLIB_QUEUE_ADD(type, a, elem, i, j, dim) {\
  a[j] = (elem);\
  SGLIB_QUEUE_ADD_NEXT(type, a, i, j, dim);\
}
#define SGLIB_QUEUE_DELETE_FIRST(type, a, i, j, dim) {\
  if (SGLIB_QUEUE_IS_EMPTY(type, a, i, j)) assert(0 && "the queue is empty");\
  (i) = ((i)+1) % (dim);\
}
#define SGLIB_QUEUE_DELETE(type, a, i, j, dim) {\
  SGLIB_QUEUE_DELETE_FIRST(type, a, i, j, dim);\
}

/* ----------------- priority queue (heap) (in an array) -------------------- */
/* heap is a triple (a,i,dim) such that:                                      */
/*  a is the array storing values                                             */
/*  i is the index of the first free element in the array                     */
/*  dim is the size of the array a                                            */
/* !!!!!!! This data structure is NOT documented, do not use it !!!!!!!!!!    */

#define SGLIB_HEAP_INIT(type, a, i) { i = 0; }
#define SGLIB_HEAP_IS_EMPTY(type, a, i) ((i)==0)
#define SGLIB_HEAP_IS_FULL(type, a, i, dim) ((i)==(dim))
#define SGLIB_HEAP_FIRST_ELEMENT(type, a, i) (a[0])
#define SGLIB_HEAP_ADD_NEXT(type, a, i, dim, comparator, elem_exchanger) {\
  int _i_;\
  if (SGLIB_HEAP_IS_FULL(type, a, i, dim)) assert(0 && "the heap is full");\
  _i_ = (i)++;\
  while (_i_ > 0 && comparator(a[_i_/2], a[_i_]) < 0) {\
    elem_exchanger(type, a, (_i_/2), _i_);\
    _i_ = _i_/2;\
  }\
}
#define SGLIB_HEAP_ADD(type, a, elem, i, dim, comparator) {\
  if (SGLIB_HEAP_IS_FULL(type, a, i, dim)) assert(0 && "the heap is full");\
  a[i] = (elem);\
  SGLIB_HEAP_ADD_NEXT(type, a, i, dim, comparator, SGLIB_ARRAY_ELEMENTS_EXCHANGER);\
}
#define SGLIB_HEAP_DELETE_FIRST(type, a, i, dim, comparator, elem_exchanger) {\
  if (SGLIB_HEAP_IS_EMPTY(type, a, i)) assert(0 && "the heap is empty");\
  (i)--;\
  a[0] = a[i];\
  SGLIB___ARRAY_HEAP_DOWN(type, a, 0, i, comparator, elem_exchanger);\
}
#define SGLIB_HEAP_DELETE(type, a, i, dim, comparator) {\
  SGLIB_HEAP_DELETE_FIRST(type, a, i, dim, comparator, SGLIB_ARRAY_ELEMENTS_EXCHANGER);\
}


/* ----------------- hashed table of pointers (in an array) -------------------- */

/*

  This hashed table is storing pointers to objects (not containers).
  In this table there is a one-to-one mapping between 'objects' stored
  in the table and indexes where they are placed. Each index is
  pointing to exactly one 'object' and each 'object' stored in the
  table occurs on exactly one index.  Once an object is stored in the
  table, it can be represented via its index.

  In case of collision while adding an object the index shifted
  by SGLIB_HASH_TAB_SHIFT_CONSTANT (constant can be redefined)

  You can NOT delete an element from such hash table. The only
  justification (I can see) for this data structure is an exchange
  file format, having an index table at the beginning and then
  refering objects via indexes.

  !!!!!!! This data structure is NOT documented, do not use it !!!!!!!!!!

*/

#define SGLIB_HASH_TAB_INIT(type, table, dim) {\
  int _i_;\
  for(_i_ = 0; _i_ < (dim); _i_++) (table)[_i_] = NULL;\
}

#define SGLIB_HASH_TAB_ADD_IF_NOT_MEMBER(type, table, dim, elem, hash_function, comparator, member){\
  unsigned _pos_;\
  type     *_elem_;\
  SGLIB_HASH_TAB_FIND_MEMBER(type, table, dim, elem, _pos_, _elem_);\
  (member) = (table)[_pos_];\
  if (_elem_ == NULL) {\
    if ((table)[_pos_] != NULL) assert(0 && "the hash table is full");\
    (table)[_pos_] = (elem);\
  }\
}

#define SGLIB_HASH_TAB_FIND_MEMBER(type, table, dim, elem, hash_function, comparator, resultIndex, resultMember) {\
  unsigned _i_;\
  int      _count_;\
  type     *_e_;\
  _count = 0;\
  _i_ = hash_function(elem);\
  _i_ %= (dim);\
  while ((_e_=(table)[_i_])!=NULL && comparator(_e_, (elem))!=0 && _count_<(dim)) {\
    _count_ ++;\
    _i_ = (_i_ + SGLIB_HASH_TAB_SHIFT_CONSTANT) % (dim);\
  }\
  (resultIndex) = _i_;\
  if (_count_ < (dim)) (resultMember) = _e_;\
  else (resultMember) = NULL;\
}

#define SGLIB_HASH_TAB_IS_MEMBER(type, table, dim, elem, hash_function, resultIndex) {\
  unsigned _i_;\
  int      _c_;\
  type     *_e_;\
  _count = 0;\
  _i_ = hash_function(elem);\
  _i_ %= (dim);\
  while ((_e_=(table)[_i_])!=NULL && _e_!=(elem) && _c_<(dim)) {\
    _c_ ++;\
    _i_ = (_i_ + SGLIB_HASH_TAB_SHIFT_CONSTANT) % (dim);\
  }\
  if (_e_==(elem)) (resultIndex) = _i_;\
  else (resultIndex) = -1;\
}

#define SGLIB_HASH_TAB_MAP_ON_ELEMENTS(type, table, dim, iteratedIndex, iteratedVariable, command) {\
  unsigned  iteratedIndex;\
  type      *iteratedVariable;\
  for(iteratedIndex=0; iteratedIndex < (dim); iteratedIndex++) {\
    iteratedVariable = (table)[iteratedIndex];\
    if (iteratedVariable != NULL) {command;}\
  }\
}


/* ---------------------------------------------------------------------------- */
/* ------------------------- DYNAMIC DATA STRUCTURES -------------------------- */
/* ---------------------------------------------------------------------------- */

/* ------------------------------------ lists (level 0) --------------------- */

#define SGLIB_LIST_ADD(type, list, elem, next) {\
  (elem)->next = (list);\
  (list) = (elem);\
}

#define SGLIB_LIST_CONCAT(type, first, second, next) {\
  if ((first)==NULL) {\
    (first) = (second);\
  } else {\
    type *_p_;\
    for(_p_ = (first); _p_->next!=NULL; _p_=_p_->next) ;\
    _p_->next = (second);\
  }\
}

#define SGLIB_LIST_DELETE(type, list, elem, next) {\
  type **_p_;\
  for(_p_ = &(list); *_p_!=NULL && *_p_!=(elem); _p_= &(*_p_)->next) ;\
  assert(*_p_!=NULL && "element is not member of the container, use DELETE_IF_MEMBER instead"!=NULL);\
  *_p_ = (*_p_)->next;\
}

#define SGLIB_LIST_ADD_IF_NOT_MEMBER(type, list, elem, comparator, next, member) {\
  type *_p_;\
  for(_p_ = (list); _p_!=NULL && comparator(_p_, (elem)) != 0; _p_= _p_->next) ;\
  (member) = _p_;\
  if (_p_ == NULL) {\
    SGLIB_LIST_ADD(type, list, elem, next);\
  }\
}

#define SGLIB_LIST_DELETE_IF_MEMBER(type, list, elem, comparator, next, member) {\
  type **_p_;\
  for(_p_ = &(list); *_p_!=NULL && comparator((*_p_), (elem)) != 0; _p_= &(*_p_)->next) ;\
  (member) = *_p_;\
  if (*_p_ != NULL) {\
    *_p_ = (*_p_)->next;\
  }\
}

#define SGLIB_LIST_IS_MEMBER(type, list, elem, next, result) {\
  type *_p_;\
  for(_p_ = (list); _p_!=NULL && _p_ != (elem); _p_= _p_->next) ;\
  (result) = (_p_!=NULL);\
}

#define SGLIB_LIST_FIND_MEMBER(type, list, elem, comparator, next, member) {\
  type *_p_;\
  for(_p_ = (list); _p_!=NULL && comparator(_p_, (elem)) != 0; _p_= _p_->next) ;\
  (member) = _p_;\
}

#define SGLIB_LIST_MAP_ON_ELEMENTS(type, list, iteratedVariable, next, command) {\
  type *_ne_;\
  type *iteratedVariable;\
  (iteratedVariable) = (list);\
  while ((iteratedVariable)!=NULL) {\
    _ne_ = (iteratedVariable)->next;\
    {command;};\
    (iteratedVariable) = _ne_;\
  }\
}

#define SGLIB_LIST_LEN(type, list, next, result) {\
  type *_ce_;\
  (void)(_ce_);\
  (result) = 0;\
  SGLIB_LIST_MAP_ON_ELEMENTS(type, list, _ce_, next, (result)++);\
}

#define SGLIB_LIST_REVERSE(type, list, next) {\
  type *_list_,*_tmp_,*_res_;\
  _list_ = (list);\
  _res_ = NULL;\
  while (_list_!=NULL) {\
    _tmp_ = _list_->next; _list_->next = _res_;\
    _res_ = _list_;   _list_ = _tmp_;\
  }\
  (list) = _res_;\
}

#define SGLIB_LIST_SORT(type, list, comparator, next) {\
  /* a non-recursive merge sort on lists */\
  type *_r_;\
  type *_a_, *_b_, *_todo_, *_t_, **_restail_;\
  int _i_, _n_, _contFlag_;\
  _r_ = (list);\
  _contFlag_ = 1;\
  for(_n_ = 1; _contFlag_; _n_ = _n_+_n_) {\
    _todo_ = _r_; _r_ = NULL; _restail_ = &_r_; _contFlag_ =0;\
    while (_todo_!=NULL) {\
      _a_ = _todo_;\
      for(_i_ = 1, _t_ = _a_;  _i_ < _n_ && _t_!=NULL;  _i_++, _t_ = _t_->next) ;\
      if (_t_ ==NULL) {\
        *_restail_ = _a_;\
        break;\
      }\
      _b_ = _t_->next; _t_->next=NULL;\
      for(_i_ =1, _t_ = _b_;  _i_<_n_ && _t_!=NULL;  _i_++, _t_ = _t_->next) ;\
      if (_t_ ==NULL) {\
        _todo_ =NULL;\
      } else {\
        _todo_ = _t_->next; _t_->next=NULL;\
      }\
      /* merge */\
      while (_a_!=NULL && _b_!=NULL) {\
        if (comparator(_a_, _b_) < 0) {\
          *_restail_ = _a_;  _restail_ = &(_a_->next); _a_ = _a_->next;\
        } else {\
          *_restail_ = _b_;  _restail_ = &(_b_->next); _b_ = _b_->next;\
        }\
      }\
      if (_a_!=NULL) *_restail_ = _a_;\
      else *_restail_ = _b_;\
      while (*_restail_!=NULL) _restail_ = &((*_restail_)->next);\
      _contFlag_ =1;\
    }\
  }\
  (list) = _r_;\
}

/* --------------------------------- sorted list (level 0) --------------------- */
/*
  All operations suppose that the list is sorted and they preserve
  this property.
*/


#define SGLIB_SORTED_LIST_ADD(type, list, elem, comparator, next) {\
  type **_e_;\
  int  _cmpres_;\
  SGLIB_SORTED_LIST_FIND_MEMBER_OR_PLACE(type, list, elem, comparator, next, _cmpres_, _e_);\
  (elem)->next = *_e_;\
  *_e_ = (elem);\
}

#define SGLIB_SORTED_LIST_ADD_IF_NOT_MEMBER(type, list, elem, comparator, next, member) {\
  type **_e_;\
  int _cmp_res_;\
  SGLIB_SORTED_LIST_FIND_MEMBER_OR_PLACE(type, list, elem, comparator, next, _cmp_res_, _e_);\
  if (_cmp_res_ != 0) {\
    (elem)->next = *_e_;\
    *_e_ = (elem);\
    (member) = NULL;\
  } else {\
    (member) = *_e_;\
  }\
}

#define SGLIB_SORTED_LIST_DELETE(type, list, elem, next) {\
  SGLIB_LIST_DELETE(type, list, elem, next);\
}

#define SGLIB_SORTED_LIST_DELETE_IF_MEMBER(type, list, elem, comparator, next, member) {\
  type **_e_;\
  int _cmp_res_;\
  SGLIB_SORTED_LIST_FIND_MEMBER_OR_PLACE(type, list, elem, comparator, next, _cmp_res_, _e_);\
  if (_cmp_res_ == 0) {\
    (member) = *_e_;\
    *_e_ = (*_e_)->next;\
  } else {\
    (member) = NULL;\
  }\
}

#define SGLIB_SORTED_LIST_FIND_MEMBER(type, list, elem, comparator, next, member) {\
  type *_p_;\
  int _cmpres_ = 1;\
  for(_p_ = (list); _p_!=NULL && (_cmpres_=comparator(_p_, (elem))) < 0; _p_=_p_->next) ;\
  if (_cmpres_ != 0) (member) = NULL;\
  else (member) = _p_;\
}

#define SGLIB_SORTED_LIST_IS_MEMBER(type, list, elem, comparator, next, result) {\
  type *_p_;\
  for(_p_ = (list); _p_!=NULL && comparator(_p_, (elem)) < 0; _p_=_p_->next) ;\
  while (_p_ != NULL && _p_ != (elem) && comparator(_p_, (elem)) == 0) _p_=_p_->next;\
  (result) = (_p_ == (elem));\
}

#define SGLIB_SORTED_LIST_FIND_MEMBER_OR_PLACE(type, list, elem, comparator, next, comparator_result, member_ptr) {\
  (comparator_result) = -1;\
  for((member_ptr) = &(list);\
      *(member_ptr)!=NULL && ((comparator_result)=comparator((*member_ptr), (elem))) < 0;\
      (member_ptr) = &(*(member_ptr))->next) ;\
}

#define SGLIB_SORTED_LIST_LEN(type, list, next, result) {\
  SGLIB_LIST_LEN(type, list, next, result);\
}

#define SGLIB_SORTED_LIST_MAP_ON_ELEMENTS(type, list, iteratedVariable, next, command) {\
  SGLIB_LIST_MAP_ON_ELEMENTS(type, list, iteratedVariable, next, command);\
}


/* ------------------------------- double linked list (level 0) ------------------------- */
/*
  Lists with back pointer to previous element. Those lists implements deletion
  of an element in a constant time.
*/

#define SGLIB___DL_LIST_CREATE_SINGLETON(type, list, elem, previous, next) {\
  (list) = (elem);\
  (list)->next = (list)->previous = NULL;\
}

#define SGLIB_DL_LIST_ADD_AFTER(type, place, elem, previous, next) {\
  if ((place) == NULL) {\
    SGLIB___DL_LIST_CREATE_SINGLETON(type, place, elem, previous, next);\
  } else {\
    (elem)->next = (place)->next;\
    (elem)->previous = (place);\
    (place)->next = (elem);\
    if ((elem)->next != NULL) (elem)->next->previous = (elem);\
  }\
}

#define SGLIB_DL_LIST_ADD_BEFORE(type, place, elem, previous, next) {\
  if ((place) == NULL) {\
    SGLIB___DL_LIST_CREATE_SINGLETON(type, place, elem, previous, next);\
  } else {\
    (elem)->next = (place);\
    (elem)->previous = (place)->previous;\
    (place)->previous = (elem);\
    if ((elem)->previous != NULL) (elem)->previous->next = (elem);\
  }\
}

#define SGLIB_DL_LIST_ADD(type, list, elem, previous, next) {\
  SGLIB_DL_LIST_ADD_BEFORE(type, list, elem, previous, next)\
  (list) = (elem);\
}

#define SGLIB___DL_LIST_GENERIC_ADD_IF_NOT_MEMBER(type, list, elem, comparator, previous, next, member, the_add_operation) {\
  type *_dlp_;\
  for(_dlp_ = (list); _dlp_!=NULL && comparator(_dlp_, (elem)) != 0; _dlp_= _dlp_->previous) ;\
  if (_dlp_ == NULL && (list) != NULL) {\
    for(_dlp_ = (list)->next; _dlp_!=NULL && comparator(_dlp_, (elem)) != 0; _dlp_= _dlp_->next) ;\
  }\
  (member) = _dlp_;\
  if (_dlp_ == NULL) {\
    the_add_operation(type, list, elem, previous, next);\
  }\
}

#define SGLIB_DL_LIST_ADD_BEFORE_IF_NOT_MEMBER(type, list, elem, comparator, previous, next, member) {\
  SGLIB___DL_LIST_GENERIC_ADD_IF_NOT_MEMBER(type, list, elem, comparator, previous, next, member, SGLIB_DL_LIST_ADD_BEFORE);\
}

#define SGLIB_DL_LIST_ADD_AFTER_IF_NOT_MEMBER(type, list, elem, comparator, previous, next, member) {\
  SGLIB___DL_LIST_GENERIC_ADD_IF_NOT_MEMBER(type, list, elem, comparator, previous, next, member, SGLIB_DL_LIST_ADD_AFTER);\
}

#define SGLIB_DL_LIST_ADD_IF_NOT_MEMBER(type, list, elem, comparator, previous, next, member) {\
  SGLIB___DL_LIST_GENERIC_ADD_IF_NOT_MEMBER(type, list, elem, comparator, previous, next, member, SGLIB_DL_LIST_ADD);\
}

#define SGLIB_DL_LIST_CONCAT(type, first, second, previous, next) {\
  if ((first)==NULL) {\
    (first) = (second);\
  } else if ((second)!=NULL) {\
    type *_dlp_;\
    for(_dlp_ = (first); _dlp_->next!=NULL; _dlp_=_dlp_->next) { };\
    SGLIB_DL_LIST_ADD_AFTER(type, _dlp_, second, previous, next);\
  }\
}

#define SGLIB_DL_LIST_DELETE(type, list, elem, previous, next) {\
  type *_l_;\
  _l_ = (list);\
  if (_l_ == (elem)) {\
    if ((elem)->previous != NULL) _l_ = (elem)->previous;\
    else _l_ = (elem)->next;\
  }\
  if ((elem)->next != NULL) (elem)->next->previous = (elem)->previous;\
  if ((elem)->previous != NULL) (elem)->previous->next = (elem)->next;\
  (list) = _l_;\
}

#define SGLIB_DL_LIST_DELETE_IF_MEMBER(type, list, elem, comparator, previous, next, member) {\
  type *_dlp_;\
  for(_dlp_ = (list); _dlp_!=NULL && comparator(_dlp_, (elem)) != 0; _dlp_= _dlp_->previous) ;\
  if (_dlp_ == NULL && (list) != NULL) {\
    for(_dlp_ = (list)->next; _dlp_!=NULL && comparator(_dlp_, (elem)) != 0; _dlp_= _dlp_->next) ;\
  }\
  (member) = _dlp_;\
  if (_dlp_ != NULL) {\
    SGLIB_DL_LIST_DELETE(type, list, _dlp_, previous, next);\
  }\
}

#define SGLIB_DL_LIST_IS_MEMBER(type, list, elem, previous, next, result) {\
  type *_dlp_;\
  SGLIB_LIST_IS_MEMBER(type, list, elem, previous, result);\
  if (result == 0 && (list) != NULL) {\
    _dlp_ = (list)->next;\
    SGLIB_LIST_IS_MEMBER(type, _dlp_, elem, next, result);\
  }\
}

#define SGLIB_DL_LIST_FIND_MEMBER(type, list, elem, comparator, previous, next, member) {\
  type *_dlp_;\
  SGLIB_LIST_FIND_MEMBER(type, list, elem, comparator, previous, member);\
  if ((member) == NULL && (list) != NULL) {\
    _dlp_ = (list)->next;\
    SGLIB_LIST_FIND_MEMBER(type, _dlp_, elem, comparator, next, member);\
  }\
}

#define SGLIB_DL_LIST_MAP_ON_ELEMENTS(type, list, iteratedVariable, previous, next, command) {\
  type *_dl_;\
  type *iteratedVariable;\
  (void)(iteratedVariable);\
  if ((list)!=NULL) {\
    _dl_ = (list)->next;\
    SGLIB_LIST_MAP_ON_ELEMENTS(type, list, iteratedVariable, previous, command);\
    SGLIB_LIST_MAP_ON_ELEMENTS(type, _dl_, iteratedVariable, next, command);\
  }\
}

#define SGLIB_DL_LIST_SORT(type, list, comparator, previous, next) {\
  type *_dll_;\
  _dll_ = (list);\
  if (_dll_ != NULL) {\
    for(; _dll_->previous!=NULL; _dll_=_dll_->previous) { };\
    SGLIB_LIST_SORT(type, _dll_, comparator, next);\
    SGLIB___DL_LIST_CREATE_FROM_LIST(type, _dll_, previous, next);\
    (list) = _dll_;\
  }\
}

#define SGLIB_DL_LIST_GET_FIRST(type, list, previous, next, result) {\
  type *_dll_;\
  _dll_ = (list);\
  if (_dll_ != NULL) {\
    for(; _dll_->previous!=NULL; _dll_=_dll_->previous) ;\
  }\
  (result) = _dll_;\
}

#define SGLIB_DL_LIST_GET_LAST(type, list, previous, next, result) {\
  type *_dll_;\
  _dll_ = (list);\
  if (_dll_ != NULL) {\
    for(; _dll_->next!=NULL; _dll_=_dll_->next) ;\
  }\
  (result) = _dll_;\
}

#define SGLIB_DL_LIST_LEN(type, list, previous, next, result) {\
  type *_dl_;\
  int _r1_, _r2_;\
  if ((list)==NULL) {\
    (result) = 0;\
  } else {\
    SGLIB_LIST_LEN(type, list, previous, _r1_);\
    _dl_ = (list)->next;\
    SGLIB_LIST_LEN(type, _dl_, next, _r2_);\
    (result) = _r1_ + _r2_;\
  }\
}

#define SGLIB_DL_LIST_REVERSE(type, list, previous, next) {\
  type *_list_,*_nlist_,*_dlp_,*_dln_;\
  _list_ = (list);\
  if (_list_!=NULL) {\
    _nlist_ = _list_->next;\
    while (_list_!=NULL) {\
      _dln_ = _list_->next;\
      _dlp_ = _list_->previous;\
      _list_->next = _dlp_;\
      _list_->previous = _dln_;\
      _list_ = _dlp_;\
    }\
    while (_nlist_!=NULL) {\
      _dln_ = _nlist_->next;\
      _dlp_ = _nlist_->previous;\
      _nlist_->next = _dlp_;\
      _nlist_->previous = _dln_;\
      _nlist_ = _dln_;\
    }\
  }\
}

#define SGLIB___DL_LIST_CREATE_FROM_LIST(type, list, previous, next) {\
  type *_dlp_, *_dlt_;\
  _dlp_ = NULL;\
  for(_dlt_ = (list); _dlt_!=NULL; _dlt_ = _dlt_->next) {\
    _dlt_->previous = _dlp_;\
    _dlp_ = _dlt_;\
  }\
}


/* ------------------------------- binary tree traversal (level 0) -------------------- */


#define SGLIB___BIN_TREE_MAP_ON_ELEMENTS(type, tree, iteratedVariable, order, left, right, command) {\
  /* this is non-recursive implementation of tree traversal */\
  /* it maintains the path to the current node in the array '_path_' */\
  /* the _path_[0] contains the root of the tree; */\
  /* the _path_[_pathi_] contains the _current_element_ */\
  /* the macro does not use the _current_element_ after execution of command */\
  /* command can destroy it, it can free the element for example */\
  type *_path_[SGLIB_MAX_TREE_DEEP];\
  type *_right_[SGLIB_MAX_TREE_DEEP];\
  char _pass_[SGLIB_MAX_TREE_DEEP];\
  type *_cn_;\
  int _pathi_;\
  type *iteratedVariable;\
  (void)(iteratedVariable);\
  _cn_ = (tree);\
  _pathi_ = 0;\
  while (_cn_!=NULL) {\
    /* push down to leftmost innermost element */\
    while(_cn_!=NULL) {\
      _path_[_pathi_] = _cn_;\
      _right_[_pathi_] = _cn_->right;\
      _pass_[_pathi_] = 0;\
      _cn_ = _cn_->left;\
      if (order == 0) {\
        iteratedVariable = _path_[_pathi_];\
        {command;}\
      }\
      _pathi_ ++;\
      if (_pathi_ >= SGLIB_MAX_TREE_DEEP) assert(0 && "the binary_tree is too deep");\
    }\
    do {\
      _pathi_ --;\
      if ((order==1 && _pass_[_pathi_] == 0)\
          || (order == 2 && (_pass_[_pathi_] == 1 || _right_[_pathi_]==NULL))) {\
        iteratedVariable = _path_[_pathi_];\
        {command;}\
      }\
      _pass_[_pathi_] ++;\
    } while (_pathi_>0 && _right_[_pathi_]==NULL) ;\
    _cn_ = _right_[_pathi_];\
    _right_[_pathi_] = NULL;\
    _pathi_ ++;\
  }\
}

#define SGLIB_BIN_TREE_MAP_ON_ELEMENTS(type, tree, _current_element_, left, right, command) {\
  SGLIB___BIN_TREE_MAP_ON_ELEMENTS(type, tree, _current_element_, 1, left, right, command);\
}

#define SGLIB_BIN_TREE_MAP_ON_ELEMENTS_PREORDER(type, tree, _current_element_, left, right, command) {\
  SGLIB___BIN_TREE_MAP_ON_ELEMENTS(type, tree, _current_element_, 0, left, right, command);\
}

#define SGLIB_BIN_TREE_MAP_ON_ELEMENTS_POSTORDER(type, tree, _current_element_, left, right, command) {\
  SGLIB___BIN_TREE_MAP_ON_ELEMENTS(type, tree, _current_element_, 2, left, right, command);\
}

#define SGLIB___BIN_TREE_FIND_MEMBER(type, tree, elem, left, right, comparator, res) {\
  type *_s_;\
  int _c_;\
  _s_ = (tree);\
  while (_s_!=NULL) {\
    _c_ = comparator((elem), _s_);\
    if (_c_ < 0) _s_ = _s_->left;\
    else if (_c_ > 0) _s_ = _s_->right;\
    else break;\
  }\
  (res) = _s_;\
}


/* ---------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------- */
/* -                          SUPPLEMENTARY DEFINITIONS                       - */
/* ---------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------- */


#define SGLIB___GET_VALUE(x) (x)
#define SGLIB___SET_VALUE(x, value) {(x) = (value);}
#define SGLIB_ARRAY_ELEMENTS_EXCHANGER(type, a, i, j) {type _sgl_aee_tmp_; _sgl_aee_tmp_=(a)[(i)]; (a)[(i)]=(a)[(j)]; (a)[(j)]= _sgl_aee_tmp_;}


#define SGLIB_SAFE_NUMERIC_COMPARATOR(x, y) (((x)>(y)?1:((x)<(y)?-1:0)))
#define SGLIB_SAFE_REVERSE_NUMERIC_COMPARATOR(x, y) (((x)>(y)?-1:((x)<(y)?1:0)))
#define SGLIB_FAST_NUMERIC_COMPARATOR(x, y) ((int)((x) - (y)))
#define SGLIB_FAST_REVERSE_NUMERIC_COMPARATOR(x, y) ((int)((y) - (x)))
#define SGLIB_NUMERIC_COMPARATOR(x, y) SGLIB_SAFE_NUMERIC_COMPARATOR(x, y)
#define SGLIB_REVERSE_NUMERIC_COMPARATOR(x, y) SGLIB_SAFE_REVERSE_NUMERIC_COMPARATOR(x, y)

#ifndef SGLIB_MAX_TREE_DEEP
#define SGLIB_MAX_TREE_DEEP 128
#endif

#ifndef SGLIB_HASH_TAB_SHIFT_CONSTANT
#define SGLIB_HASH_TAB_SHIFT_CONSTANT 16381   /* should be a prime */
/* #define SGLIB_HASH_TAB_SHIFT_CONSTANT 536870912*/   /* for large tables :) */
#endif

#endif /* _SGLIB__h_ */
