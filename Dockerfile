FROM alpine:latest
RUN apk add --no-cache clang libc-dev build-base
WORKDIR /app
COPY . .
CMD ["make"]
