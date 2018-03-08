
# Latex2Grammarly #

## Description ##

Deform the latex text file to the form compatible with the Grammarly input.

## Function ##

 * Erace comment line % ~
 * Erace \begin{align} ~ \end{align}
 * Replace $ ~ $ with [expression]
 * Replace \cite{ ~ } with [citation]
 * Erace \rem{ ~ }

## Known issues ##

 * We cannot insert empty line between for example \rem{ ~ }.
 * Do not work well if line feed code is Windows-like `\r\n`.
   * Use `$ tr -d \\r <old >new` to fix the issue.
 * How to treat `\eqref` etc..??
