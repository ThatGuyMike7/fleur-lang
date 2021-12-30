## tokenizer-01
Testing if tokenizer can detect a token at end of file correctly, even if more tokens could match if there wasn't end of file.

For example `/` at end of file: `/`, `//` and `/*` are candidates, but we reached the end of the character stream, thus `/` must be selected.

## tokenizer-02
Testing if tokenizer can detect an integer at end of file correctly.

## tokenizer-03
Testing if tokenizer can detect identifiers, integers and symbols correctly with whitespace between them.