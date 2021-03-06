# Overview

Minishell is the first 42 shell project, we had to build the minimum viable version of a shell, including a prompt waiting for user input, signal handling, command execution from $PATH or from an absolute path.. (more details in [the subject](https://gitlab.com/Bccyv/minishell/-/blob/master/subject.pdf)).

As good students, we did a little bit more:
- Key shortcuts for command line editing
- Command history
- Autocompletion
- Wildcards
- Dynamic git prompt
- Aliases (coming with alias / unalias builtins)
- 'Assistant' builtin that brings you coffe (Ok, maybe not)

## Shortcuts

 - <kbd>Ctrl-U</kbd>: erase command
 - <kbd>Ctrl-K</kbd>: erase command after cursor
 - <kbd>Ctrl-H</kbd>: erase command before cursor
 - <kbd>Ctrl-P</kbd> / ⬆: scroll up  history
 - <kbd>Ctrl-N</kbd> / ⬇: scroll down history
 - ⬅ / ➡: move cursor left / right
 - <kbd>Option</kbd> + ⬅ / ➡: move cursor left / right word by word
 - <kbd>Fn</kbd> + ⬅ / ➡: move cursor at the beginning / end of the line

# Installation & execution

    git clone https://gitlab.com/bccyv/minishell
    cd minishell
    make
    ./minishell

