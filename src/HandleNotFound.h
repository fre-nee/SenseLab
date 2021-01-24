void handleNotFound() {
  String message = "404 - NOT FOUND";
  server.send(404, "text/plain", message);
}