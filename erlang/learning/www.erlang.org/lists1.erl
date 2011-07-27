-module(lists1).
-export([min/1,
	 max/1,
	 min_max/1<]).

min([Head|Rest]) ->
    min(Rest, Head).

min([], Min) ->
    Min;
min([Head|Rest], Tmp_min) when Head < Tmp_min ->
    min(Rest, Head);
min([_|Rest], Tmp_min) ->
    min(Rest, Tmp_min).

max([Head|Rest]) ->
    max(Rest,Head).

max([],Max)->
    Max;
max([Head|Tail], Tmp_max) when Head > Tmp_max ->
    max(Tail, Head);
max([_|Tail], Tmp_max) ->
    max(Tail, Tmp_max).

min_max(List) ->
    {min(List),max(List)}.
