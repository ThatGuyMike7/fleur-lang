 # Fleur Language Specification

## Introduction

This document specifies the **Fleur** programming language.

## Source Code Representation
Source Code is text encoded in UTF-8 with LF line breaks.

## Notation
The syntax is specified using Extended Backus-Naur Form (eBNF).

```ebnf
(*
    Important notes:
    - Whitespace and Block Comments are allowed between tokens (see the definition of "token"),
      this has been left out of the productions for readability.
    - /* */ pairs match like brackets (inner first) which allows them to be nested.
*)

token = identifier | number |
        "//" | "/*" | "*/" | ";" | ":" | "." | "," | "(" | ")" | "[" | "]" | "{" | "}" | "^" | "?" |
        "=" | "+" | "-" | "*" | "/" | "&" | "|" | "!" | "<" | ">" |
        "&&" | "||" | "!=" | "==" | "<=" | ">=" | "::" | "->" |
        "---" ;

any_char = ? any character ? ;

newline = "\n" ; (* Unix, Linux, and Windows all consider \n a newline character. *)

letter = "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" | "J" | "K" | "L" | "M" | "N" |
         "O" | "P" | "Q" | "R" | "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z" | "a" | "b" |
         "c" | "d" | "e" | "f" | "g" | "h" | "i" | "j" | "k" | "l" | "m" | "n" | "o" | "p" |
         "q" | "r" | "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z" | "_" ;

digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;

(* "-" is a unary operator and thus not part of the literal. *)
signed_integer_literal_10 = "0" | (digit - "0") , { digit } ;

unsigned_integer_literal_10 = signed_integer_literal_10 , "u" ;

float_literal_10 = (integer , "f") | (integer , "." , { digit } , ["f"]) ;

number = signed_integer_literal_10 | unsigned_integer_literal_10 | float_literal_10 ;

struct_literal = identifier , "{" , { expr , "," } , expr , "}" ;

uninitialized_literal = "---" ;

literal = unsigned_integer_literal_10 |
            signed_integer_literal_10 |
                     float_literal_10 |
                       struct_literal |
                uninitialized_literal ;

line_comment = "//" , { any_char } - newline , newline ;

(* The compiler must properly match /* */ pairs, thus the token */ is allowed inside block comments. *)
block_comment = "/*" , { any_char } , "*/" ;

whitespace = " "  |    (* Space *)
             "\r" |    (* Carriage Return *)
             "\t" |    (* Horizontal Tab *)
             "\v" |    (* Vertical Tab *)
             "\f" |    (* Form Feed *)
          newline ;    (* Newline *)

identifier = letter , { letter | digit } ;

specifier = "const" ;

qualifier = "mut" ;

unqualified_type = { "^" | "?" } , identifier ;

type = { [qualifier] , ("^" | "?") } , [qualifier] , identifier ;

struct_field = { specifier } , identifier , ":" , unqualified_type ;

struct_definition = "struct" , identifier ,
                    "{" ,
                    ([struct_field , [","]]) | (* One or no fields *)
                    (struct_field , "," , { struct_field , "," } , struct_field [","]) (* More than one fields *)
                    "}" ;

fn_call = identifier ,
          "(" ,
          ([expr] , ")") | (* One or no parameter *)
          (expr , "," { expr , "," } , expr , ")") ; (* More than one parameters *)

binary_op = "+" | "-" | "*" | "/" ;

lunary_op = "-" | "&" ;

runary_op = "^" | "?";

(* An expression is something that can potentially return a value. *)
expr = primary_expr |
        binary_expr |
   parenthesis_expr |
   lunary_op , expr |
   expr , runary_op ;

member_access_expr = (expr - member_access_expr) , "." , identifier , { "." , identifier };

parenthesis_expr = "(" , expr , ")" ;

primary_expr = identifier | (* Reference *)
                  fn_call |
                  literal ;

binary_expr = expr , binary_op , expr ;

(* This should be enough to define the syntax of an assignment,
   obviously not every expression is an lvalue / can be assigned to. *)
assignment_statement = expr , "=" , expr ;

let_binding = "let" , { specifier } , identifier ,
              [":" , type] , (* Optional type *)
              ["=" , expr] ; (* Optional assignment *)

fn_parameter = identifier , ":" , type ;

fn_prototype = "fn" ,
               identifier , (* Function name *)
               "(" ,

               ([fn_parameter] , ")") | (* One or no parameter *)
               (fn_parameter , "," , { fn_parameter , "," } , fn_parameter , ")") , (* More than one parameters *)

               ["->", type] ; (* Optional return type *)

fn_body = "{" , fn_level_grammar , "}" ;

fn_definition = fn_prototype , fn_body ;

return_statement = "ret" , expr ;



(* A statement is something that can potentially have a side-effect
   (that includes e.g. allocating stack memory for a variable). *)
fn_level_statement = (let_binding | fn_call | return_statement) , ";" ;

fn_level_grammar =
{
    {
        line_comment | block_comment | fn_level_statement
    }
} ;

top_level_statement = ? nothing, for now ? ;

top_level_grammar =
{
    {
        line_comment | block_comment | top_level_statement |
        ( fn_prototype , ";" ) | fn_definition
    }
} ;
```

## Sample Code

```
struct vec3
{
    x: f32,
    y: f32,
    z: f32,
}

fn mul_vec3(a: vec3, b: vec3) -> vec3
{
    ret vec3{ a.x * b.x, a.y * b.y, a.z * b.z };
}

fn mul_vec3(a: s32, b: vec3) -> vec3
{
    ret vec3{ a * b.x, a * b.y, a * b.z };
}

fn zero_vec3(a: ^mut vec3)
{
    a^ = vec3{ 0f, 0f, 0.0 };
}
```

## Definitions

### Function

### Let Binding

### lvalue

An **lvalue** is a mutable, identifiable location in memory. In simpler terms: LHS of an assignment.

### Labels and goto

### defer
**defer** statements are executed at the end of the current scope, at a **continue** statement in the current scope,
or at a **return** statement in the current or any of the current's child scopes.

**goto** statements that follow a **defer** statement in the current or any of the current's child scopes
trigger a compile error that can be silenced using the **[skip_defer]** attribute.
This is to avoid unintentionally skipping execution of **defer** statements
but can be done using **goto** with the aforementioned attribute, if necessary.