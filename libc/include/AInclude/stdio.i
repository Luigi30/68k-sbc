    xref _simple_printf

NL  equ 10

* Format string usage:
*   PUSH format arguments in reverse order
*   PUSH string pointer
*   JSR _printf
_printf MACRO
    _simple_printf
    ENDM