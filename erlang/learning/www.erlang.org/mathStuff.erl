
-module(mathStuff).
-export([perimeter/1]).

perimeter({square, Side}) ->
    Side*4;

perimeter({circle, Radius}) ->
    (Radius*2)*3.14;

perimeter({triangle, A, B, C}) ->
    A+B+C.
