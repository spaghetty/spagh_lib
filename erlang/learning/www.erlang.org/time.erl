-module(time).
-export([swedish_date/0]).

swedish_date()->
    Date = date(),
    [_,_|Year] = integer_to_list(element(1,Date)),
    string:join([Year,
		 string:right(integer_to_list(element(2,Date)),2,$0),
		 string:right(integer_to_list(element(3,Date)),2,$0)],"").
