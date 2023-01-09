use actix_web::{get, App, HttpResponse, HttpServer, Responder};

#[get("/")]
async fn base_path() -> impl Responder {
    HttpResponse::Ok().body("hello world")
}


#[actix_web::main]
async fn main() -> std::io::Result<()> {
    HttpServer::new(|| {
        App::new()
            .service(base_path)
    })
    .bind(("127.0.0.1", 8080))?
    .run()
    .await
}