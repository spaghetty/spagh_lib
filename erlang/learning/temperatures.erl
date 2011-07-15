-module(temperatures).
-export([temperatures_to_celsius/1,
	 print_temp_list/1
	 ]).

temperatures_to_celsius(L) ->
    l_conv_to_celsius(L,[]).

l_conv_to_celsius([], Final) ->
    Final;

l_conv_to_celsius([H|Rest],Temp) ->
    l_conv_to_celsius(Rest,[convert_to_celsius(H)|Temp]).

convert_to_celsius({Name, {c, Value}}) ->
    {Name, {c, Value}};

convert_to_celsius({Name, {f, Value}}) ->
    {Name, {c, (Value - 32) * 5 / 9}}.

print_temp_list([H|Rest]) ->
    print_temp_list(H, Rest).

print_temp_list(City,[]) ->
    render_city_temp(City);

print_temp_list(City, [H | Rest]) ->
    render_city_temp(City),
    print_temp_list(H,Rest).


render_city_temp({Name, {Type, Value}}) ->
    io:format("The temperature at ~s in ~w is:  ~w~n",[Name, Type, Value]).
