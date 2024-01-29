# building rest api

- Download REST api jar from https://github.com/markjfisher/http-simple/releases/download/v0.1/http-simple-0.1-all.jar
- Run it with:

```shell
MICRONAUT_SERVER_HOST=0.0.0.0 MICRONAUT_SERVER_PORT=8080 java -jar /path/to/http-simple-0.1-all.jar
```

This will allow the server to respond to network requests on all bindings.
Change the port if you don't like the default, but you will also have to amend the netread src.
