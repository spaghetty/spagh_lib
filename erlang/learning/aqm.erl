%% aqm -> quantitative analisis methods
-module(aqm).
-export([mape/1]).
-import(lists).



mape(L) ->
    Add = fun(A) -> ape(A) end,
    lists:map(Add,L).

ape([F,R]) ->
    ape(F,R).

ape(F,R) ->
    (abs(R-F)/R)*100.
