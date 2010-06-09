-module(dummy).
-export([factorial/1, double/1, area/1]).

factorial(0) ->
    1;

factorial(N) ->
    N * factorial(N-1).


double(N) ->
    times(N, 2).

times(X, N) ->
    X * N.

area({square, Side}) ->
    Side * Side;

area({rectangle, X, Y}) ->
    X * Y;

area({circle, Radius}) ->
    Radius * Radius * 3.14159;

area({triangle, A, B, C}) ->
    S = ( A + B + C)/2,
    math:sqrt(S*(S-A)*(S-B) *(S-C)).
