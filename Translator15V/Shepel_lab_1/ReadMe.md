## Shepel Konstantin Sviatoslavovich

### variant 15

![Machine graph](graph.jpg "graph")

#### Grammar

1.  <signal-program> --> <program>
2.  <program> --> PROGRAM <procedure-identifier> ;
<block> ;
3.  <block> --> BEGIN <statements-list> END
4.  <statements-list> --> <statement> <statements-
list> |
<empty>
5.  <statement> --> <unsigned-integer> :
<statement> |
<variable-identifier> := <unsigned-
integer> ; |
<procedure-identifier><actual-
arguments> ; |
GOTO <unsigned-integer> ; |
LINK <variable-identifier> , <unsigned-
integer> ; |
IN <unsigned-integer>; |
OUT <unsigned-integer>; |
RETURN ; |
; |
($ <assembly-insert-file-identifier> $)
6.  <actual-arguments> --> ( <variable-identifier>
<actual-arguments-list> ) |
<empty>
7.  <actual-arguments-list> --> ,<variable-
identifier> <actual-arguments-list> |
<empty>
8.  <variable-identifier> --> <identifier>
9.  <procedure-identifier> --> <identifier>
10.  <assembly-insert-file-identifier> -->
<identifier>
11.  <identifier> --> <letter><string>
12.  <string> --> <letter><string> |
<digit><string> |
<empty>
13.  <unsigned-integer> --> <digit><digits-string>
14.  <digits-string> --> <digit><digits-string> |
<empty>
15.  <digit> --> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
16.  <letter> --> A | B | C | D | ... | Z
