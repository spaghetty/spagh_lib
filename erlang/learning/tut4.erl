%this is temperature conversion test

-module(tut4).
-export([format_temps/1,
	 max_list/1,
	 reverse_list/1]).

format_temps([])->
    ok;

format_temps([City | Rest]) ->
    print_temp(celsius_convert_temp(City)),
    format_temps(Rest).

celsius_convert_temp({Name, {c, Temp}}) ->
    {Name, {c, Temp}};
celsius_convert_temp({Name, {f, Temp}}) ->
    {Name, {c, (Temp - 32) * 5 / 9}}.

print_temp({Name, {c, Temp}}) ->
    io:format("The temperature in celsius for ~s is ~w~n",[Name, Temp]).


max_list([Head|Rest])->
    max_list(Rest,Head).

max_list([], Res) ->
    Res;

max_list([Head|Rest], Part_res) when Head > Part_res ->
    max_list( Rest, Head);

max_list([Head|Rest], Part_res) ->
    max_list(Rest, Part_res).

reverse_list([Head|Rest]) ->
    reverse_list(Rest, [Head]).

reverse_list([], List) ->
    List;

reverse_list([Head|Rest], OldHead ) ->
    reverse_list(Rest, [Head|OldHead]).
