# pensieve

![demo](/assets/demo.png)

A simple truth-table generator written in C++. Expression parsing is performed using the Shunting-yard algorithm.

### Working mechanism

pensieve first tokenizes the given input and classifies them into variables, operators, and parentheses. During this operation, it also validates the input and checks for invalid expressions like missing operators/operands/parentheses and so on.

The next step is to use the [shunting yard algorithm](https://en.wikipedia.org/wiki/Shunting_yard_algorithm) to convert the infix expression to the reverse polish notation (aka postfix expression, eg. `!a | b & c` => `a ! b c & |`). I've also added a slight modification by providing unary operators with the highest precedence and right associativity. The evaluation of the postfix expression is pretty easy.

Once the expression is parsed and the reverse polish notation is generated, the final step is to evaluate the expression and buid a matrix that will repesent the table and render it on the console.

A detailed version of this excerpt can be found on this [report](/pensieve_dm_report.pdf).

### Symbol Guide

| Symbol | Meaning       | Pensieve | 
|--------|---------------|----------|
| ¬      | NOT           |    !     |
| ∧      | AND           |    &     |
| ∨      | OR            |   \|     |
| →      | IMPLICATION   |    >     |
| ↔      | BICONDITIONAL |    =     |
| ⊕      | XOR           |    ^     |

### Build from source

```sh
make
```

Run the program:

```sh
make run
```

### Usage

Just launch the executable file and you'd see a prompt. Enter your expressions here and press enter.

You can type `/q`, `exit` or `quit` to exit the application.

You can also toggle the debug mode using the `/debug` command. It will show your given expression in the reverse polish notation and the given variables, in order.

You can autocomplete these commands by pressing tab.


### Acknowledgements

1. The awesome [linenoise](https://github.com/arangodb/linenoise-ng) library.

2. The supercool [tabulate](https://github.com/p-ranav/tabulate) library.


### Future Goals

- [ ] Export tables into multiple formats (CSV, Markdown)
- [ ] Support multi-character symbols (better tokenizer)