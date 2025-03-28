# pensieve

A simple math interpreter written in C++ using the Shunting-yard algorithm.

Watch the [demo video](https://youtu.be/KPOjq7f3VuU).

### Working mechanism

pensieve first tokenizes the given input and classifies them into numbers, operators, parentheses and so on. During this operation, it also validates the input and checks for invalid expressions like missing operators/operands/parentheses and so on.

The next step is to use the [shunting yard algorithm](https://en.wikipedia.org/wiki/Shunting_yard_algorithm) to convert the infix expression to the reverse polish notation (aka postfix expression, eg. `1 + 2 - 3` => `1 2 + 3 -`). I've also added a slight modification by providing unary operators with the highest precedence and right associativity. The evaluation of the postfix expression is pretty easy.


### Build from source

```sh
make
```

### Usage

Just launch the executable file and you'd see a prompt. Enter your expressions here and press enter.

You can type `/q`, `exit` or `quit` to exit the application.

You can also toggle the debug mode using the `/debug` command. It will show your given expression in a parenthesized manner.

You can autocomplete these commands by pressing tab.


### Further Goals

- [x] Support for unary operators
- [x] Pretty output
- [x] Input history
- [x] Tab Completions
- [ ] Functions

### Acknowledgements

1. The awesome [linenoise](https://github.com/arangodb/linenoise-ng) library.