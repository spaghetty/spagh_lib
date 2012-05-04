var ctx;
var ctx_height;
var ctx_width;
var gameOver = false
var survived_block = 0;
var rightDown=false;
var leftDown=false;
var mainIterval;
var components= new Array();

function circle(size) {
    ctx.beginPath();
    ctx.arc(200, 75, size, 0, Math.PI*2, true); 
    ctx.closePath();
    ctx.stroke();
}

function brick(x,y) {
    this.x = x;
    this.y = y;
    this.h = 15;
    this.w = 50;
    this.state = 1;
    this.is_paddle = false;
    this.is_brick = true;

    this.hit = function() {
	if(this.state>0) 
	    this.state = this.state-1;
    }

    this.act = function() {
    }
    
    this.collide = function(x,y,x1,y1) {
	if(this.state>0)
	    return ((x1>this.x-10  && x1<(this.x + this.w + 10)) &&
		    ( y1>this.y-10  && y1<(this.y + this.h + 10)));
	else
	    return false	    
    }

    this.collide_x = function(x) {
	return (x > this.x-10 && x < this.x + this.w + 10)
    }
    
    this.collide_y = function(y) {
	return ( y > this.y-10  && y < this.y + this.h + 10);
    }

    this.draw = function() {
	if (this.state>0) {
	    ctx.beginPath();
	    ctx.rect(this.x, this.y, this.w, this.h);
	    ctx.closePath();
	    ctx.fill();
	}
    }
}

function paddle(x) {
    this.position = x;
    this.h = 10;
    this.w = 75;
    this.is_brick = false;
    this.is_paddle = true;
    this.act = function() {
	this.move(6);
    }

    this.move = function(step) {
	if ( rightDown )
	    if( (this.position + step + this.w) <= ctx_width)
		this.position += step;
	    else 
		this.position;
	else if ( leftDown ) 
	    if ( this.position - step > 0)
		this.position -= step;
	    else
		this.position;
    }
    
    this.collide = function(x,y,x1,y1) {
	return (x1 > this.position && x1 < this.position + this.w)
    }

    this.draw = function() {
	ctx.beginPath();
	ctx.rect(this.position, ctx_height-this.h, this.w, this.h);
	ctx.closePath();
	ctx.fill();
    }
}

function ball(x, y, size) {
    this.x = x;
    this.y = y;
    this.dir_x = 1;
    this.dir_y = 1;
    this.size = size;
    this.is_brick = false;
    this.is_paddle = false;
    this.act = function() {
	this.move(2, 5);
    }

    this.move = function(x, y) {
	myx = this.dir_x * x;
	myy = this.dir_y * y;
	if ( this.x + myx > ctx_width || this.x + myx < 0)
	    this.dir_x = -this.dir_x;
	if ( this.y + myy < 0)
	    this.dir_y = -this.dir_y;
	else if ( this.y + myy > ctx_height )
	{
	    hit_paddle = false;
	    for(var i=0; i< components.length; i++) {
		if (components[i].is_paddle)
		if (components[i].collide(this.x, this.y, this.x + myx, this.y + myy))
		{
		    hit_paddle=true;
		    this.dir_y = -this.dir_y;
		    this.dir_x = dir_x * (this.x - ( components[i].position + components[i].w/2))/(components[i].w *2)
		    break;
		}
	    }
	    if ( !hit_paddle)
		endGame();
	}
	else {
	    for(var i=0; i< components.length; i++) {
		if (components[i].is_brick)
		if (components[i].collide(this.x, this.y, this.x + myx, this.y + myy))
		{
		    components[i].hit();
		    if (components[i].collide_y(this.y+myy))
			this.dir_y = -this.dir_y;
		    else if (components[i].collide_x(this.x+myx))
			this.dir_x = -this.dir_x;
		    //break;
		}
	    }
	}
	this.x = this.x + (this.dir_x * x);
	this.y = this.y + (this.dir_y * y);
    }

    this.collide = function(x, y, x1, y1) {
	return false;
    }

    this.draw = function() {
	ctx.beginPath();
	ctx.arc(this.x, this.y , size, 0, Math.PI*2, true); 
	ctx.closePath();
	ctx.fill();
    };
}

function endGame() {
    for(var i=0; i< components.length; i++) {
	if ( components[i].is_brick && components[i].state > 0)
	    survived_block += 1;
    }
    gameOver=true;
}

function onKeyDown(evt) {
    if (evt.keyCode == 39) rightDown = true;
    else if (evt.keyCode == 37) leftDown = true;
}

//and unset them when the right or left key is released
function onKeyUp(evt) {
    if (evt.keyCode == 39) rightDown = false;
    else if (evt.keyCode == 37) leftDown = false;
}


function clear_area() {
    ctx.clearRect(0,0,ctx_width,ctx_height);
}

function draw() {
    clear_area();
    if (!gameOver){
	for(var i=0; i< components.length; i++) {
	    components[i].draw();
	    components[i].act();
	}
    }
    else {
	ctx.beginPath();
	ctx.arc(ctx_width/2, ctx_height/2 , 40, 0, Math.PI*2, true); 
	ctx.closePath();
	if (survived_block > 0 )
	    ctx.stroke();
	else
	    ctx.fill();
	clearInterval(mainInterval);
    }	
}

function init() {
    ctx = $('#game-area')[0].getContext("2d");
    ctx_height = $('#game-area')[0].height;
    ctx_width = $('#game-area')[0].width;
    components[0] = new ball(200, 100, 10); 
    components[1] = new paddle(ctx_width/2.5);
    components[2] = new brick(220, 80); //first row
    components[3] = new brick(280, 80);
    components[4] = new brick(340, 80);
    components[5] = new brick(400, 80);
    components[6] = new brick(190, 60); // second row
    components[7] = new brick(250, 60);
    components[8] = new brick(310, 60);
    components[9] = new brick(370, 60);
    components[10] = new brick(430, 60);

    mainInterval = setInterval(draw, 13);
}


$(document).keydown(onKeyDown);
$(document).keyup(onKeyUp);
$(document).ready(init);