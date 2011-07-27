%demo:double a function that doubles parameter

-module(demo).
-export([double/1]).

double(Num) ->
    Num * 2.
