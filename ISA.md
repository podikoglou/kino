# kino ISA

| opcode | name    | description               | operands |
| ------ | ------- | ------------------------- | -------- |
| 0x00   | `PUSHC` | load immediate value      | `value`  |
| 0x01   | `PUSHV` | load a variable's value   | `name`   |
| 0x02   | `STORE` | pop and store to variable | `store`  |
| 0x06   | `ADD`   | pop, pop, add, push       | -        |
| 0x07   | `SUB`   | pop, pop, subtract, push  | -        |
| 0x08   | `MULT`  | pop, pop, multiply, push  | -        |
| 0x09   | `DIV`   | pop, pop, divide, push    | -        |
