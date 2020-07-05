# Translator

## Лексичний аналізатор


## Граматика
```
 1. <signal-program> --> <program>
2. <program> --> PROCEDURE <procedure-identifier><parameters-list> ; <block> ;
3. <block> --> <declarations> BEGIN <statements-list> END
4. <statements-list> --> <empty>
5. <parameters-list> --> ( <declarations-list> ) | <empty>
6. <declarations> --> <variable-declarations>
7. <variable-declarations> --> VAR <declarations-list> | <empty>
8. <declarations-list> --> <declaration> <declarations-list> | <empty>
9. <declaration> --><variable-identifier><identifiers-list>:<attribute><attributes-list> ;
10. <identifiers-list> --> , <variable-identifier> <identifiers-list> | <empty>
11. <attributes-list> --> <attribute> <attributes-list> | <empty>
12. <attribute> --> SIGNAL | COMPLEX | INTEGER | FLOAT | BLOCKFLOAT | EXT
13. <variable-identifier> --> <identifier>
14. <procedure-identifier> --> <identifier>
15. <identifier> --> <letter><string>
16. <string> --> <letter><string> | <digit><string> | <empty>
17. <digit> --> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
18. <letter> --> A | B | C | D | ... | Z
```

##### Граф роботи лексичного аналізатора

![Граф](./opt_lab1/graf.png)
