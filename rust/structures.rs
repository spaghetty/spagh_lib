
// A struct with two fields
#[derive(Debug)]
struct Point {
    x: f32,
    y: f32,
}

#[derive(Debug)]
struct Rectangle {
    // A rectangle can be specified by where the top left and bottom right
    // corners are in space.
    top_left: Point,
    bottom_right: Point,
}

fn square(corner: &Point, edge: &f32) -> Rectangle {
    Rectangle{top_left: Point{ x: corner.x, y: corner.y+edge}, bottom_right: Point{ x: corner.x+edge, y: corner.y}}
}

fn rect_area(r: Rectangle) -> f32 {
    match r {
        Rectangle{ top_left: Point{x: x1, y: y1}, bottom_right: Point{ x: x2, y: y2}} => (y1-y2)*(x2-x1)
    }
}

fn main() {
    let corner = Point{x: 7.0, y: 3.0};
    let edge = 5.0;
    let rect = square(&corner, &edge);
    println!("base point is {:?} with size of {} and the resulting rectangle is {:?}", corner, edge, rect);

    println!("the area of the rectangle is: {}", rect_area(rect))
}