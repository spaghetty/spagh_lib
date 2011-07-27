
-module(multiprocess).
-export([go1/0,
	 loop1/0,
	 go2/0,
	 ring_spawn/2,
	 ring_loop/1,
	 go3/0,
	 star_receiver/1]).


go1() ->
    Pid2 = spawn(multiprocess, loop1, []),
    msg_count(Pid2, 10).

msg_count(Pid, 0) ->
    Pid ! hangout;

msg_count(Pid, Count) ->
    Pid ! {self(), hello},
    io:format("~w: Msg Sent ",[Pid]),
    receive
	{_ , hello} ->
	    msg_count(Pid, Count-1)
    end.

loop1() ->
    receive
	{From, hello} ->
	    From ! {self(), hello},
	    io:format("~w: Msg Received~n",[From]),
	    loop1();
	hangout ->
	    true
    end.
	    
go2() ->
    Dest = spawn(multiprocess, ring_spawn, [10, self()]),
    send_counted(Dest,fanculo,10).

send_counted(Pid, _, 0) ->
    Pid ! {self(), hangout};
send_counted(Pid, Msg, Num) ->
    Pid ! {self(), Msg},
    receive
	{From, Msg} -> 
	    io:format("~w I'm the first and i got msg ~w from ~w~n",[self(), Msg, From])
    end,
    send_counted(Pid, Msg, Num-1).

ring_spawn(1, First) ->
    ring_loop(First);
ring_spawn(Num, First ) ->
    Dest = spawn(multiprocess, ring_spawn, [Num-1, First]),
    ring_loop(Dest).

ring_loop(Dest) ->
    receive
	{From, Msg} ->
	    io:format("~w got msg ~w from ~w~n",[self(), Msg, From]),
	    Dest ! { self(), Msg },
	    ring_loop(Dest);	    
	hangout -> 
	    true
    end.

go3() ->
    star_spawn(5,self()).

star_spawn(Count, _) ->
    Tips = star_tips(self(),Count),
    send_counted_msg(fanculo,5, Tips).
    
send_msg_to(Pid, Msg) ->
    Pid ! { self(), Msg }.

send_counted_msg(_, 0, Tips) ->
    Fun = fun(T) -> send_msg_to(T, hangout) end,
    lists:foreach(Fun,Tips);
send_counted_msg(Msg, Count, Tips) ->
    Fun = fun(T) -> send_msg_to(T, Msg) end,
    lists:foreach(Fun,Tips),
    send_counted_msg(Msg, Count-1, Tips).

star_tips(_, 0) ->
    [];
star_tips(Center, Count) ->
    Tips = star_tips(Center, Count-1),
    Tip = spawn(multiprocess, star_receiver, [Center]),
    [Tip | Tips ].
    
star_receiver(Center) ->
    receive
	{From, Msg} ->
	    io:format("~w got msg ~w from ~w~n",[self(), Msg, From]),
	    Center ! {self(), "got_it"},
	    star_receiver(Center);
	hangout ->
	    io:format("~w is dying",[self()])
    end.
