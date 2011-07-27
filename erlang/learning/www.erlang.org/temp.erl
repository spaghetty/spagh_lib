%Temperature conversions.

-module(temp).
-export([f2c/1,
	 c2f/1,
	 convert/1]).

%the formula c = ((f - 32)*5)/9

f2c(Temp) ->
    ((Temp - 32)*5)/9.

%the formula f = ((c * 9)/5) + 32

c2f(Temp) ->
    ((Temp * 9)/5)+32.

%convert wants tuple of type and value, like { c, 43 }

convert({c, Value}) ->
    {f, c2f(Value)};

convert({f, Value}) ->
    {c, f2c(Value)}.
