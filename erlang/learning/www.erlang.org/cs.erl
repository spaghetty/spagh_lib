-module(cs).
-export([start/1,
	 up_slave/1,
	 to_slave/2,
	 stop/0]).


start(SlaveNum)->
    build_up(SlaveNum),
    put(slavenum,SlaveNum), 
    io:format("il processo numer 3 e' ~w~n",[get(3)]).


to_slave(Msg, Slave)->
    Pid = get(Slave),
    Pid ! Msg.

stop() ->
    stop(get(slavenum)).

stop(0) ->
    ok;
stop(SlaveNum) ->
    to_slave(die,SlaveNum),
    stop(SlaveNum-1).

build_up(0) ->
    ok;

build_up(SlaveNum) ->
    Pid = spawn(cs, up_slave, [SlaveNum]),
    io:format("sto creando il processo ~B ed ha come pid ~w~n", [ SlaveNum, Pid ]),
    put(SlaveNum,Pid),
    build_up(SlaveNum-1).

    
up_slave(SNum) ->
    receive
	die ->
	    io:format("I had been killed ~B~n", [ SNum ]),
	    ok;
	Msg ->
	    io:format("slave ~B got a message ~s~n", [ SNum, Msg ]),
	    up_slave(SNum)
    end.
